# Hodnocené části:

## 1) Automatické testy a manuální hodnocení implementace (body jsou rozděleny dle rozdělení):
- Zahrnuje automatické testy včetně testů registrovaných rozšíření.
- Hodnocení implementace předepsané metody syntaktické analýzy.
- Dodržení varianty zadání v implementaci (symtable.c).
- Přehlednost a kvalita kódu.

### Orientační ceník za nejčastější chyby a prohřešky včetně několika zkratek, které se mohou vyskytovat v komentářích hodnocení implementace:
- **NOHEAD** = chybí záhlaví ve zdroj. souborech: -0,50 b
- **NOAUTHORS** = chybí jména autorů v záhlaví: -0,20 b
- **PMGP** = nadměrné množství globálních proměnných: -0,20 b (tolerována globální proměnná pro chybu, tabulku symbolů a seznam instrukcí)
- **HEADCODE** = kód v hlavičkových souborech místo v .c souborech: -0,15 b
- **LCOM++** = téměř žádné komentáře: -0,30 b
- **LCOM** = málo komentářů: -0,10 b
- **GCOM** = příliš mnoho komentářů: -0,10 b
- **NOCOMH** = chybí komentáře u deklarací důležitých funkcí v .h: -0,10 b
- **FUNCDECLH** = chybí deklarace funkcí v .h: -0,10 b
- **MIXENCZ** = kombinování anglického a českého jazyka v komentářích: -0,10 b
- **INDENTATION** = špatné/nevhodné odsazení bloků: bez postihu
- **TOOLONGFUNCS** = příliš dlouhé funkce (lépe provést dekompozici): bez postihu
- **CLEAN** = celková úprava kódu (více prohřešků): -0,50 b (odsazení, zakomentované zbytky kódu, ...)
- **COPYPASTE** = kopírování kódu místo vytvoření vhodné funkce: -0,10 b
- **VUL** = vulgarismy: -0,30 b
- **NORS/PA** = SA analýza chybí: -0,60 b
- **NOPSA** = SA analýza výrazů chybí: -0,60 b
- **IAL+** = TS v jiném souboru než symtable.(c|h): -0,10 b
- **IZP** = TS neimplementována jako TRP s implicitně zřetězenými položkami: -1,00 b
- **VV** = TS neimplementována jako výškově vyvážený BVS: -1,00 b

### Mezi časté neduhy pro 2023/2024 cvičící uvedli:
- V komentáři v hlavičce souboru je vhodné stručně popsat, co daný soubor obsahuje/definuje/implementuje.
- Studenti nevěnovali dostatek pozornosti bližší specifikaci požadavků na tabulku symbolů (zkratky IZP a VV).
- Při používání zkratek je vhodné někdy uvést celý název (např. komentář v hlavičce). Např. DLL může znamenat Double Linked List, nebo Dynamic Link Library.
- Snažte se dodržovat jednotnou konvenci pojmenovávání proměnných, funkcí, modulů. Často byly míchány konvence jako snake_case, camelCase, PascalCase, atd.

## 2) Prémiové body, korekce, malus:
- Aktivita na fóru, rychlostní soutěž, pozdní odevzdání, formální nedostatky.

## 3) Dokumentace (viz zadání; body jsou korelovány implementací a rozděleny dle rozdělení; seznam zkratek níže):
### Seznam zkratek, které se mohou vyskytnout v komentářích k hodnocení dokumentace:
- **KA** = chyby v KA, především větší míra nedeterminismu, chybějící důležité lexémy (např. ošetření komentářů atp.)
- **strukt.** = nedostatky ve struktuře dokumentu (nejčastěji chybějící závěr)
- **PSA** = chybějící nebo nedostatečný popis precedenční syntaktické analýzy
- **RP** = chybějící popis rozdělení práce
- **GK** = chybějící/nedostatečný popis generování kódu
- **1. strana** = chybějící informace na 1. straně dokumentace
- **LL** = chyby a nedostatky v LL tabulce
- **LLT** = chybějící nebo chybná LL tabulka
- **typ.** = typografické nedostatky (nejčastěji text nebyl zarovnán do bloku)
- **term.** = nepřesná terminologie
- **SA** = nedostatečný/chybějící popis syntaktické analýzy
- **SAV** = nedostatečný popis syntaktické analýzy výrazů
- **SéA** = chybějící/nedostatečný popis sémantické analýzy (typicky není dobře popsáno, co všechno se kontroluje)
- **PT** = chybějící, chybná nebo nečitelná precedenční tabulka
- **TS** = chybějící/nedostatečný popis tabulky symbolů či její implementace
- **SHORT** = nedostatečný, nekonkrétní popis řešení
- **STYLE** = nesrozumitelné věty, dlouhá či těžko pochopitelná souvětí
- **gram.** = gramatické chyby
- **LANG** = míchání jazyků v rámci dokumentace (nejčastěji češtiny a slovenštiny)
- **HOV** = použití hovorových výrazů, zbytečné používání anglických výrazů (např. pushnout, symtable, list, array)
- **BLOK** = text není zarovnán do bloku
- **KAPTXT** = dokumentace obsahuje více nadpisů hned za sebou
- **MEZ** = špatné psaní mezer (např. kolem závorek a interpunkce)
- **SAZBA** = alespoň identifikátory proměnných a funkcí se patří sázet písmem s jednotnou šířkou písmen (např. font Courier)

### Za většinu prohřešků bylo podle závažnosti strhnuto 0,1 až 0,7 bodu.
Poznámky k hodnocení dokumentací jsou vesměs řazeny dle klesající závažnosti.

### Pár poznámek k typickým chybám v dokumentaci pro 2023/2024:
- **LLT** - letos poměrně často studentům nějaká pravidla chyběla nebo řešitelé ignorovali množiny FOLLOW ve spojení s epsilon pravidly. Srážky cca 0,3 bodu.
- **SA** - dostkrát se objevilo tvrzení, že rekurzivní sestup implementují tak, že mají funkci pro každé pravidlo - bylo strženo cca 0,2 bodu.
- **SéA** - obvykle málo konkrétní popis toho, co kontrolují.
- **TS** - chybí nějaká část popisu, nejčastěji podrobnější popis struktury tabulky (např. zda máte více tabulek nebo jednu).