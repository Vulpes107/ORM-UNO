# ORM-UNO Projekat

## Pregled
ORM-UNO je projekat koji se sastoji od serverske i klijentske komponente.

## Pokretanje projekta

### Metod 1: Manuelno pokretanje

Da biste manuelno pokrenuli projekat, pratite sledeće korake:

1. **Kompajliranje projekta**
   ```bash
   make clean  # Opciono, ako želite početi od nule
   make
   ```

2. **Pokretanje servera**
   
   Otvorite novi terminal i izvršite:
   ```bash
   ./bin/build/server
   ```

3. **Pokretanje klijenata**
   
   Otvorite jedan ili više novih terminala za pokretanje klijenata:
   ```bash
   ./bin/build/client
   ```

   Možete pokrenuti više klijenata istovremeno u različitim terminalima.

### Metod 2: Korištenje skripte start_env.py

Za brže pokretanje, možete koristiti priloženu Python skriptu:

1. **Standardno pokretanje**
   
   Ova komanda će pokrenuti server i dva klijenta u odvojenim terminalima:
   ```bash
   python3 start_env.py
   ```

2. **Pokretanje uz rekompajliranje**
   
   Ako želite prvo rekompajlirati projekat, a zatim pokrenuti sve komponente:
   ```bash
   python3 start_env.py --rebuild
   ```

## Struktura projekta

- `server/` - Kod serverske aplikacije
- `client/` - Kod klijentske aplikacije
- `common/` - Zajednički kod koji koriste i server i klijent
- `bin/` - Direktorij sa izvršnim fajlovima nakon kompajliranja
  - `build/` - Izvršne datoteke
  - `obj/` - Objektni fajlovi

## Zahtjevi

- C++ kompajler sa podrškom za C++23
- Make
- Python 3 (za korištenje start_env.py skripte)
- GNOME Terminal (za korištenje start_env.py skripte)
