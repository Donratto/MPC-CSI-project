clear all;
close all;
clc;
addpath("src");

% Parameters
T = 2;
f = 1000; % Frequency
Fs = 4096; % Sampling frequency
N = T*Fs;
freq=Fs/N*(0:N-1);
% t = (0:N-1) / Fs; % Time vector
t = linspace(0,T,N);
sig_test = sin(2 * pi * f * t); % Test signal
opakovani = 10;

% MATLAB FFT
matlab_fft = fft(sig_test);
tic;
for i = 1:opakovani
    matlab_fft = fft(sig_test);
end
matlab_fft_time = toc/opakovani;

% Custom Recursive FFT
fft_recur = mex_moj_fft(sig_test, 'recur');
tic;
for i = 1:opakovani
    fft_recur = mex_moj_fft(sig_test, 'recur');
end
fft_recur_time = toc/opakovani;

% Custom Iterative FFT
fft_iter = mex_moj_fft(sig_test, 'iter');
tic;
for i = 1:opakovani
    fft_iter = mex_moj_fft(sig_test, 'iter');
end
fft_iter_time = toc/opakovani;

% MATLAB IFFT
matlab_ifft = ifft(matlab_fft);
tic;
for i = 1:opakovani
    matlab_ifft = ifft(matlab_fft);
end
matlab_ifft_time = toc/opakovani;

% Custom Recursive IFFT
ifft_recur = mex_moj_fft(fft_recur, 'recur_ifft');
tic;
for i = 1:opakovani
    ifft_recur = mex_moj_fft(fft_recur, 'recur_ifft');
end
ifft_recur_time = toc/opakovani;

% Custom Iterative IFFT
ifft_iter = mex_moj_fft(fft_iter, 'iter_ifft');
tic;
for i = 1:opakovani
    ifft_iter = mex_moj_fft(fft_iter, 'iter_ifft');
end
ifft_iter_time = toc/opakovani;






% Display Execution Times
% fprintf('Custom Recursive FFT Time: %f seconds\n', custom_fft_recur_time);
% fprintf('Custom Iterative FFT Time: %f seconds\n', custom_fft_iter_time);
% fprintf('MATLAB FFT Time: %f seconds\n', matlab_fft_time);
% fprintf('Custom Recursive IFFT Time: %f seconds\n', custom_ifft_recur_time);
% fprintf('Custom Iterative IFFT Time: %f seconds\n', custom_ifft_iter_time);
% fprintf('MATLAB IFFT Time: %f seconds\n', matlab_ifft_time);

times_table = table(...
    {'Recursive FFT'; 'Iterative FFT';'MATLAB FFT';'Recursive IFFT';'Iterative IFFT';'MATLAB IFFT'}, ... % Column 1: Names
    [fft_recur_time; fft_iter_time;matlab_fft_time; ifft_recur_time; ifft_iter_time; matlab_ifft_time], ... % Column 2: Times
    'VariableNames', {'Implementation', 'ExecutionTime'}); % Column names

times_table.ExecutionTime = times_table.ExecutionTime * 1000;
% disp(times_table);
labels = times_table.Implementation;
values = times_table.ExecutionTime;


screenSize = get(0, 'ScreenSize'); % Get screen resolution
% disp(['Screen resolution: ', num2str(screenSize)]);

% Example: Set figure to cover the left side of the screen
figure("Name","Porovnání FFT");
set(gcf, 'Position', [0, 0, screenSize(3)/2, screenSize(4)]); % Cover the left half of the screen
subplot(2, 1, 1)
hold on
plot(freq, abs(fft_recur),'r-')
plot(freq, abs(fft_iter),'g--')
plot(freq, abs(matlab_fft),'b-.');
hold off
legend('Rekurzivní FFT', 'Iterativní FFT','MATLAB FFT');
title('Modul diskrétní Fourierovy transformace (DFT)')
xlabel('Frekvence [Hz]')
ylabel('Modul [-]')
xlim([0 max(freq)]);
legend('Location','northeast','Orientation','vertical');

subplot(2, 1, 2)
hold on
plot(freq, angle(fft_recur),'r-')
plot(freq, angle(fft_iter),'g--')
plot(freq, angle(matlab_fft),'b-.')
hold off
legend('Rekurzivní FFT', 'Iterativní FFT','MATLAB FFT');
title('Fáze diskrétní Fourierovy transformace (DFT)')
xlabel('Frekvence [Hz]')
ylabel('Fáze [rad]')
xlim([0 max(freq)]);
legend('Location','northeast','Orientation','vertical');


% Plot the results
figure("Name","Porovnání časů zpracování FFT/IFFT a porovnání IFFT");
set(gcf, 'OuterPosition', [screenSize(3)/2, 0, screenSize(3)/2, screenSize(4)]); % Cover the right half of the screen
subplot(2, 1, 1)
bar(values);
set(gca, 'XTickLabel', labels, 'XTick', 1:length(labels));
xlabel('FFT implementace');
ylabel('Čas zpracování [ms]');
title('Porovnání časů FFT/IFFT');
grid on;

t_ms = t*1000; % Convert time to milliseconds

subplot(2, 1, 2)
hold on
plot(t_ms, sig_test,'k')
plot(t_ms, ifft_recur,'r-')
plot(t_ms, ifft_iter,'g--')
plot(t_ms, matlab_ifft,'b-.');
hold off
legend('testovací signál','Rekurzivní IFFT', 'Iterativní IFFT','MATLAB IFFT');
title('Porovnání původního signálu s obnovenými pomocí zpětných FFT')
xlabel('Čas [ms]')
ylabel('Amplituda [-]')
xlim([0 max(t_ms)/(f)]);
legend('Location','northeast','Orientation','vertical');