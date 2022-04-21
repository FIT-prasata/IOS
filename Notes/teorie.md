# Basics

## Operační systém

- kolekce programů
- vytváří spojující vrstvu mezi HW výpočetního systému (může být i virtuální), uživatelem a aplikačním SW
- Cíl: co nejlepší poměr maximální využití zdrojů a jednoduchosti použití
- Role: správce prostředků, tvůrce prostředí pro uživatele --> UI + abstrakce
- Zahrnuje: JÁDRO, systémový SW (utility, knihovny), rozhraní

## Jádro

- nejnižší a nejzákladnější část OS
- běží neustále po zapnutí PC
- uživatel do něj nevidí - zapouzdření
- reaktivní program běžící v privilegovaném režimu
- správa prsotředků a tvorba prostředí pro vyšší vrstvy

### Monolity

- těsně provázané (spadne 1 subsystém --> spadne všechno)
- velký rozsah služeb a abstrakcí   
- dobrá rychlost X špatná rozšiřitelnost
- méně bezpečné
- Monolitcká jádra s modulářní strukturou --> lepší, pouze používané moduly v jádře

### Mikrojádra

- cíl minimalizovat rozsah jádra
- většina služeb v serverech mimo privilegovaný režim
- menší rozsah, bezpečnější
- lepší rozšiřitelnost

### Preempce

přerušení právě vykonávaného procesu bez spolupráce s ním --> koncept HW přerušení 

### POSIX

standard definující jednotné rozhraní OS pro programátory (API) a uživatele (programy, utility)

### UNIX

- 2 základní abstrakce: procesy a soubory
- IPC (Inter Process Communication) - roury, signály, semafory, sockety, ...
- komunikace s jádrem pomocí systémových volání (syscalls)

### HW přerušení

- asynchronní elektrický signál jdoucí z periferie do jádra, kde vyvolá obsluhu přerušení
- každé přerušení má prioritu, podle které se oznamuje CPU
- lze naprogramovat řadič na maskování některých přerušení až na NMI (non-maskable interrupt)

### Komunikace s jádrem

- synchronní --> proces - jádro
- asynchronní --> hardware - jádro

## Souborové systémy (FS)

