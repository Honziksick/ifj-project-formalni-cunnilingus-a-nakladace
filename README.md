<h1>Projekt z předmětů IFJ a IAL - Implementace překladače imperativního jazyka IFJ24</h1>

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

## O projektu

Podrobné informace o projektu jsou k nahlédnutí ve složce `info/` ve formátu **PDF**. Tato složka obsahuje mimo jiné zadání projektu, plán projektu, návrhy gramatik a tipy & triky k tvorbě projektu od vůdce týmu.

## Autoři

**Tým xkalinj00 alias _"Formální gramatiky a parsovače"_**
| **Jméno**             | **xlogin** | **Přezdívka** | **Kontakt**                 |
| --------------------- | ---------- | ------------- | --------------------------- |
| **Farkašovský Lukáš** | xfarkal00  | NormREE       | xfarkal00@stud.fit.vutbr.cz |
| **Hýža Pavel**        | xhyzapa00  | RyzaGrey      | xhyzapa00@stud.fit.vutbr.cz |
| **Kalina Jan**        | xkalinj00  | Honziksick    | xkalinj00@stud.fit.vutbr.cz |
| **Krejčí David**      | xkrejcd00  | DJmDavidus    | xkrejcd00@stud.fit.vutbr.cz |

<center><img src="doc\resources\logo_upscale.png" alt="Logo" width=170></center>

## Hodnocení

**Celkově: 23 + 4 + 5 bodů** *(implementace + dokumentace + obhajoby)*

1) **Implementace: 19,75 bodů** (z 18 b.)
   - základ za implementaci +18 b.
   - rozšíření FUNEXP +1,5 b.
   - GCOM -0,10 b.
   - další +0,35 b.
2) **Prémie: 3 body**
   - rychlostní soutěž +3 b.
3) **Dokumentace: 4 body** (z 5 b.)
   - konečný automat, tabulka symbolů -1 b.
4) **Obhajoby: 5 bodů** (z 5 b.)

## Prostředí

Ubuntu 64bit

## Licence

Projekt je distribuován s otevřenými zdrojovými texty pod licencí `GNU GPL v.3`

## Vůdcův týmový motivační program _aka Clash of Stars ⭐_

| **Jméno**             | **Zlaté hvězdičky ⭐** | **Černé puntíky ⚫** | **Celkové skóre**                 |
| --------------------- | :-:           | :-: | :-: |
| **Farkašovský Lukáš** | ⭐⭐⭐⭐⭐  |  -  | +5  |
| **Hýža Pavel**        | ⭐⭐⭐⭐⭐  |  ⚫⚫  | +3  |
| **Kalina Jan**        | ⭐⭐⭐⭐⭐ <br> ⭐⭐⭐⭐⭐ <br> ⭐⭐⭐⭐⭐|  -  | +15 |
| **Krejčí David**      | ⭐⭐⭐⭐⭐ <br> ⭐⭐⭐⭐⭐ <br> ⭐          |  -  | +11 |

### Vůdcův log
- **30\.09\.2024:** <br>
  *Všem členům týmu byla udělena jedna zlatá hvězdička ⭐ za splnění vůdcova domácího úkolu a aktivní přístup k diskusi na týmovém meetingu dne 30\. 09\. 2024 v čase 14:00-15:50.*
- **01\.10\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi za založení a nachystání týmového repozitáře a light verze testovacího frameworku GoogleTest.*
- **08\.10\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi za vytvoření konfiguračních souborů pro VS Code, `.gitignore`, `.editorconfig` a `Doxyfile`.*
- **14\.10\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Lukášovi za implementaci knihovny pro dynamický string a konzulatci dalších možných úprav knihovny.*
- **15\.10\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Davidovi za implementaci beta verze dynamické tabulky symbolů izp-TRP.*
- **15\.10\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi za kompletní návrh LL-gramatiky pro syntaktický analyzátor.*
- **21\.10\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Davidovi za hodiny času, které vložil do tvorby C++ generátoru LL-tabulky včetně její C implementace.*
- **25\.10\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi a Davidovi za několikahodinnový brainstorm meeting, jehož tématem byl počáteční návrh rozhraní překladače, návrh alpha verze abstraktního syntaktického stromu, komunikace mezi moduly a celkově dalšího postupu při vývoji.*
- **28\.10\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Pavlovi za návrh FSM lexikálního analyzátoru, který by snad konečně mohl fungovat, a typů tokenů.*
- **06\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Lukášovi za pomoc s testováním různých částí projektu (lexikálního analyzátoru a tabulky symbolů).*
- **09\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Davidovi za implementaci modulu FrameStack a kompletaci testů na FrameStack a Symtable.*
- **10\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi za kompletní návrh precedenčního syntaktického analyzátoru, precedenční tabulky a redukčních pravidel.*
- **13\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi za implementaci LL-tabulky (a testů) a precedenční tabulky, za návrh a implementaci implementační verze terminálů pro oba parsery a návrh komunikace mezi jendotlivými parsery.*
- **12\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi za vytvoření test-utils souboru s doplňkovými funkcemi, zejména pak vizualizací vygenerovaného AST.*
- **14\.11\.2024:** <br>
  *Uděluje se černý puntík ⚫ Pavlovi za to, že stále nedokončil lexikální analyzátor, nedodal ani zdánlivě fungující verzi či verzi, která by šla přeložit.*
- **15\.11\.2024:** <br>
  *Udělují se dvě zlaté hvězdičky ⭐⭐ vůdci týmu Honzovi za implementaci celého precedenčního parseru a speciálního precedenčního zásobníku, dále implementace FUNEXP rozšíření a potřebného zásobníku precedenčních zásobníků a kombinované rekurze mezi LL a precedenčním parserem při parsování libovolně komplikovaných argumentů funkcí, korektní sledování závorek.*
- **18\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi za implementaci celého LL-parseru a návrh jeho dalšího rozšíření (propagace chybových stavů static proměnnými, goto kaskáda k řízení chybových stavů, ...).*
- **19\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Davidovi za aktivní účast v diskuzi týkající se implementace parseru a řešení menších problémů s rozhraním mezi moduly v důsledku provedených změn.*
- **20\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Davidovi za implementaci celého sémantického analyzátoru a testů na sémantický analyzátor.*
- **22\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi za implementaci velkého množství testů parseru a mnoha dalších testů sémantického analyzátoru, dále opravy řady memmory leaků a bugů (zejména napříč moduly parseru, ale také v sémantickém analyzátoru).*
- **25\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Lukášovi za vytvoření návrhu a alpha verze generátoru mezikódu, čímž významně přispěl k vývoji jeho finální verze pro nasazení; dále za pomoc s tvorbů testů na kontrolu korektnosti generovaného kódu.*
- **27\.11\.2024:** <br>
  *Uděluje se černý puntík ⚫ Pavlovi za... to bych radši ani nevytahoval.*
- **28\.11\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Davidovi za implementaci verze generátoru mezikódu k nasazení.*
- **01\.12\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Davidovi za implementaci řady optimalizačních technik do generátoru mezikódu a sémantického analyzátoru.*
- **02\.12\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi za implementaci God-Like sebedokumentujícího se Makefilu (viz `make help`).*
- **03\.12\.2024:** <br>
  *Všem členům týmu byla udělena jedna zlatá hvězdička ⭐ za refaktorizaci svých modulů, sjednocení názvů proměnných, splnění všech formálních požadavků a doplnění doxygen dokumentace.*
- **04\.12\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Pavlovi za tvorbu dokumentace projektu.*
- **04\.12\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ vůdci týmu Honzovi a Davidovi za jednoznačně největší přínos k celému vývoji projektu, obětovanému času a spánku a za noční můry o abstraktních syntaktických stromech.*
- **11\.12\.2024:** <br>
  *Uděluje se zlatá hvězdička ⭐ Pavlovi za tvorbu prezentace k obhajobám a referenčního textu k jednotlivým snímkům.*

---

*Datum poslední úpravy: 01.02.2025, 01:46*
