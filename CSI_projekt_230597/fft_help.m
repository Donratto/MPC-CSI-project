clear all;
close all;
clc;

data = load('./out/gen.txt');
sig_test = data(:, 1);

data = load('./out/fft_recur.txt');
real_part = data(:, 1);
imag_part = data(:, 2);
fft_recur = real_part + 1i * imag_part;

data = load('./out/fft_iter.txt');
real_part = data(:, 1);
imag_part = data(:, 2);
fft_iter = real_part + 1i * imag_part;

data = load('./out/ifft_recur.txt');
real_part = data(:, 1);
imag_part = data(:, 2);
ifft_recur = real_part + 1i * imag_part;

data = load('./out/ifft_iter.txt');
real_part = data(:, 1);
imag_part = data(:, 2);
ifft_iter = real_part + 1i * imag_part;

Fvz = 4096;
T = 2;
N = T*Fvz;
% N = size(sig_test, 1);
t = linspace(0,T,N);
f=1000;
% sig_test = sin(2*pi*f*t);
freq=Fvz/N*(0:N-1);

tic;
matlab_fft = fft(sig_test);
matlab_fft_time = toc;

tic;
matlab_ifft = ifft(matlab_fft);
matlab_ifft_time = toc;

stats = readtable('./out/stats.txt','Delimiter', ',', 'ReadVariableNames', false);
new_rows = {'matlab FFT', matlab_fft_time;'matlab IFFT', matlab_ifft_time}; % New rows to append
new_stats = cell2table(new_rows, 'VariableNames', {'Var1', 'Var2'}); % Convert the new rows to a table
stats = [stats; new_stats]; % Append the new rows to the existing stats table

% Add a new column for sorting
stats.SortOrder = contains(stats.Var1, 'IFFT') + 1; % 1 for FFT, 2 for IFFT

% Sort the table by the new column
stats = sortrows(stats, 'SortOrder');

% Remove the sorting column (optional)
stats.SortOrder = [];

labels = stats.Var1;
values = stats.Var2;

values = values * 1000; % Convert to milliseconds

% Display the timing data
% disp('Timing Data:');
% disp(table(labels, values));


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