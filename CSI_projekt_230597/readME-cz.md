# Vlastní implementace FFT v jazyce C
🏴󠁧󠁢󠁥󠁮󠁧󠁿[English](./readME.md)
## Zadání projektu
```
2.2 DFT/FFT
Vytvořte vlastní implementaci FFT nebo DFT. Výsledek vhodným způsobem
vykreslete a porovnejte s nativní MATLABovou implementací. Porovnejte
paměťovou a výpočetní náročnost obou implementací.
```

## Popis projektu
Tento projekt implementuje Fast Fourier Transform (FFT) a její inverzi (IFFT) pomocí rekurzivního a iterativního přístupu v jazyce C. Implementace je integrována s MATLABem prostřednictvím MEX rozhraní pro porovnání výkonu s vestavěnými funkcemi MATLABu.

## Funkce
- Rekurzivní a iterativní implementace FFT/IFFT v jazyce C.
- Automatické doplnění vstupního signálu na nejbližší mocninu dvou pro kompatibilitu s FFT.
- Integrace s MATLABem pomocí MEX funkce (`mex_moj_fft`).
- Porovnání výkonu vlastních implementací FFT/IFFT s vestavěnými funkcemi MATLABu.
- Vizualizace výsledků FFT a porovnání doby výpočtu.
- Testovací signál o délce `T=2 s` je sinusová vlna o frekvenci `f=1000 Hz` vzorkovaná frekvencí `fs=4096 Hz`.

## Nedostatky
- Nepodařilo se implementovat porovnání paměťové náročnosti.

## Struktura souborů
- **`src/`**: Obsahuje zdrojové soubory v jazyce C pro implementaci FFT a MEX rozhraní.
  - `moj_fft.c`: Hlavní implementace FFT/IFFT.
  - `moj_fft.h`: Hlavičkový soubor pro FFT funkce.
  - `mex_moj_fft.c`: MEX rozhraní pro MATLAB.
- **`out/`**: Složka pro generované výstupní soubory.
- **`fft_comp.m`**: MATLAB skript pro porovnání implementací FFT. Pro lepší porovnání několikrát opakuje FFT a výsledné časy průměruje. 
- **`fft_help.m`**: MATLAB skript pro načítání a vizualizaci výsledků FFT.
- **`Makefile`**: Skript pro kompilaci kódu v jazyce C.

## Použití
Je třeba se nacházet v kořenové složce projektu, tedy výstup z&nbsp;`pwd` bude končit takto `...\CSI_projekt_230597`.

### 1. **[fft_comp.m](fft_comp.m)**
```matlab
fft_komp
```
Skript prostřednictvím **[MEX rozhraní](#kompilace-a-spuštění-maincsrcmainc)** volá funkce knihovny a provádí porovnání výkonu a přesnosti různých implementací FFT/IFFT (rekurzivní, iterativní a MATLAB vestavěné). Generuje testovací sinusový signál, měří časy výpočtu jednotlivých implementací a vizualizuje výsledky FFT (modul a fázi) i IFFT (rekonstruovaný signál). Výsledky jsou zobrazeny v grafech a tabulce s časy výpočtu.

### 2. **[fft_help.m](fft_help.m)**
```matlab
fft_help
```
Skript načítá uložené výsledky FFT/IFFT z výstupních souborů, které byly vygenerovány `src/main.c`, provádí jejich vizualizaci a porovnání s MATLAB vestavěnými funkcemi FFT/IFFT. Zobrazuje grafy modulů a fází FFT, porovnává časy výpočtu různých implementací a vykresluje sloupcový graf srovnávající výpočetní časy.

### Kompilace MEX rozhraní v MATLABu
Je potřeba mít v MATLABu nainstalovaný Add-On [MATLAB Support for MinGW-w64 C/C++/Fortran Compiler](https://www.mathworks.com/matlabcentral/fileexchange/52848-matlab-support-for-mingw-w64-c-c-fortran-compiler). 
Pak lze zkompilovat MEX rozhraní:
```matlab
cd src
mex mex_moj_fft.c moj_fft.c
cd ..
```

Soubor zkompilovaného MEX rozhraní vypadá rozdílně podle operačního systému.
- macOS: `mex_moj_fft.mexmaca64`
- Windows: `mex_moj_fft.mexw64`

### Kompilace a spuštění [main.c](src/main.c)

Je potřeba mít nainstalovaný GCC.

Windows
```bash
gcc src/main.c -o out/main
./out/main.exe
```

macOS/Linux
```bash
make
```

## Prohlášení
Tento projekt byl vypracován s využitím asistence nástroje GitHub Copilot, který poskytoval návrhy a podporu při psaní kódu a dokumentace.

Zdroje: 
- [wikipedie](https://fr.wikipedia.org/wiki/Transformation_de_Fourier_rapide)
- [cp-algorithms.com](https://cp-algorithms.com/algebra/fft.html)
- [lloydrochester.com](https://lloydrochester.com/post/c/example-fft/)
