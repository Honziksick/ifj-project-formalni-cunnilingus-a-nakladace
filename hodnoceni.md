# Hodnocení

## Celkové hodnocení

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

## Výsledek testů

Procentuální hodnocení modulů překladače (NOVĚ přidán u každé kategorie testů i zaokrouhlený procentní poměr bodů ztracených kvůli špatnému návratovému kódu (včetně chybě při interpretaci vygenerovaného kódu) a neshodě ve výpisech při správném návratovém kódu):

### Testy základního zadání

- **Lexikální analýza (detekce chyb): 82 %** (174/210 mb, špatné kódy 17 %)
- **Syntaktická analýza (detekce chyb): 96 %** (258/267 mb, špatné kódy 3 %)
- **Sémantická analýza (detekce chyb): 92 %** (372/402 mb, špatné kódy 7 %)
- **Interpretace přeloženého kódu (základní): 90 %** (324/360 mb, špatné kódy 10 %, nesouhlasné výpisy 0 %)
- **Interpretace přeloženého kódu (výrazy, vest. funkce): 100 %** (170/170 mb, špatné kódy 0 %, nesouhlasné výpisy 0 %)
- **Interpretace přeloženého kódu (komplexní): 71 %** (299/421 mb, špatné kódy 15 %, nesouhlasné výpisy 13 %)

**Celkem bez rozšíření: 87 % (1597/1830 mb)**

### Testy rozšíření

#### Implementovaná rozšíření

- **FUNEXP 100 %** (150/150 mb, špatné kódy 0 %, nesouhlasné výpisy 0 %)

#### NEimplementovaná rozšíření

- **ORELSE 0 %** (0/50 mb, špatné kódy 100 %, nesouhlasné výpisy 0 %)
- **UNREACHABLE 0 %** (0/50 mb, špatné kódy 100 %, nesouhlasné výpisy 0 %)
- **BOOLTHEN 0 %** (0/100 mb, špatné kódy 100 %, nesouhlasné výpisy 0 %)
- **FOR 0 %** (0/25 mb, špatné kódy 100 %, nesouhlasné výpisy 0 %)
- **WHILE 0 %** (0/100 mb, špatné kódy 100 %, nesouhlasné výpisy 0 %)

## Pravidla hodnocení jednotlivých částí

### 1) Automatické testy a manuální hodnocení implementace:
- Zahrnuje automatické testy včetně testů registrovaných rozšíření.
- Hodnocení implementace předepsané metody syntaktické analýzy.
- Dodržení varianty zadání v implementaci (symtable.c).
- Přehlednost a kvalita kódu.

#### Orientační ceník za nejčastější chyby a prohřešky včetně několika zkratek, které se mohou vyskytovat v komentářích hodnocení implementace:

- **NOHEAD** = chybí záhlaví ve zdroj. souborech: -0,50 b
- **NODESC** = chybí stručný popis významu souboru v záhlavích (s přidanou hodnotou, tj. poskytující víc informací, než poví název souboru): bez postihu
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
- **RELOP** = relační výrazy nejsou řešeny pomocí precedenční SA: -0,20 b
- **IAL+** = TS v jiném souboru než symtable.(c|h): -0,10 b
- **IZP** = TS neimplementována jako TRP s implicitně zřetězenými položkami: -1,00 b
- **VV** = TS neimplementována jako výškově vyvážený BVS: -1,00 b
- **VV-** = výškově vyvážený strom není implementován správně (nevyvažuje správně, ve všech případech atp.): -0,50 b
- **VVHASH** = jako klíč stromu používáte jednoduchý hash řetězce, ale neřešíte možné kolize: -0,30 b
- **RED** = zbytečné/redundantní operace (typicky v datových strukturách, např. opakované volání strcmp na stejné dvojici řetězců): -0,05 b
- **MEM** = nebezpečná/nesprávná práce s pamětí (např. často chybí kontroly malloc, alokace paměti o velikosti ukazatele místo velikosti struktury, opomenutí znaku \0 při alokaci místa pro řetězec atp.): -0,05 b

#### Mezi časté neduhy pro 2024/2025 cvičící uvedli:
- **Obecně:**
  - TRP-izp: Někteří z vás místo tabulky s implicitním zřetězením položek implementovali tabulku s explicitním zřetězením položek. Osvěžte si patřičnou přednášku z IAL.
   K úpravě kódu – ve zkratce, při týmové práci jste byli velmi NEKONZISTENTNÍ:
  - V komentáři v záhlaví souboru je vhodné stručně popsat, co daný soubor obsahuje/definuje/implementuje. (Zejména fascinující je, když implementujete nějaký konkrétní typ BVS, ale nikam nenapíšete, že to je třeba AVL.) Ve všech souborech by měla ta záhlaví vypadat stejně.
  - V týmových projektech je zásadní určit si na začátku konvenci pojmenovávání proměnných, funkcí, struktur, modulů, výčtových typů atp. a tuto konvenci poté dodržovat! Ve většině projektů jste míchali konvence jako snake_case, camelCase, PascalCase atd., mnohdy i uvnitř jedné struktury / jednoho modulu...
  - V projektech tohoto rozsahu je velmi žádoucí používat nástroje, které automaticky formátují kód, resp. použít je alespoň jednou před odevzdáním. Umí to každé normální IDE, umí to různé pluginy do vimu, dají se používat externí nástroje. Samozřejmě je všichni členové týmu musí mít nastavené stejně, ale to jde typicky nějakým souborem, který je ve složce s projektem (i v repozitáři). Formátováním kódu se myslí například mezery a odřádkování kolem závorek (vč. složených) a operátorů. Taky je vhodné držet délku řádků na nějaké rozumné hodnotě (třeba +- 120 znaků), aby se vešly na průměrný monitor...
  - Dokumentace funkcí patří k deklaracím do hlavičkových souborů (jde především o popis „rozhraní“ té funkce).
  - Při používání zkratek je vhodné někdy uvést celý název (alespoň komentářem v záhlaví). Např. DLL může znamenat Double Linked List, nebo Dynamic Link Library. Taky je vhodné pojmenovávat soubory (moduly) celými slovy – těch pár písmen navíc ničemu neuškodí a člověk pak nemusí dlouho přemýšlet, co znamená dll.c nebo symanal.c.
  - Před odevzdáním projektu si projeďte svá vlastní TODO.
- **K jazyku C:**
  - Když v jednom hlavičkovém souboru používáte nějaký typ odjinud, musíte v něm vložit (#include) ten jiný hlavičkový soubor, ve kterém se typ nachází! Platí to i o standardních typech (třeba size_t, FILE, bool). Jinak je úspěch překladu spíš šťastnou náhodou. Naopak je vhodné nevkládat soubory, které pak nevyužíváte!
  - V generátorech kódu máte rádi dlouhé sekvence samostatných volání printf pro každý řádek výstupu, a to často pro výpis zcela konstantních řetězců bez formátovacích značek. Dopad na výkon asi bude (téměř) neměřitelný, přesto by asi bylo hezčí ten literál jednoduše rozbít na více řádků – mrkněte na https://stackoverflow.com/a/1135862.
  - Máte přespříliš rádi dynamickou paměť. Viděl jsem spoustu obratů stylu „alokuji paměť pro token – vrátím ukazatel – volající se podívá na typ tokenu, občas z něj vytáhne nějaká data a paměť uvolní“. Malloc není zadarmo a mj. každý zvyšuje riziko, že zapomenete na free nebo naopak něco uvolníte dvakrát. Struktury se dají normálně předávat hodnotou! (A když jsou dost malé, což vaše často jsou, bude to mnohem rychlejší.)
  - Pokud definujete makro, které se má „tvářit jako funkce“, tj. uvnitř něj je víc příkazů a pak někde v kódu používáte MAKRO(něco), je vhodné použít konstrukci:
  ```
  #define MAKRO(x) do {    ...    } while(0)
  ```
    <ul style="list-style-type: none;">
    <li> Pro vysvětlení vizte třeba  <a href="https://stackoverflow.com/a/1067238">https://stackoverflow.com/a/1067238</a>. Pozor, nemá smysl tuhle konstrukci použít, když pak na konec makra dáte středník! I pokud děláte makro bez parametrů, které pak používáte někde jako příkaz, vypadá trochu lépe, když do definice makra na konci neuvedete středník – pak musíte ten středník napsat v kódu za to makro, což vypadá konzistentněji s jakýmkoliv jiným příkazem jazyka C.</li>
    </ul>

  - TRP-izp: Velmi často vypadala základní struktura vašich TRP asi takhle:

  ```
  typedef struct {
      hashtable_item **items;
      // různá metadata
  } hashtable;
  ```

  <ul style="list-style-type: none;">
    <li>Při použití takové struktury musíte někde alokovat paměť pro hashtable, pak někde alokovat paměť pro pole ukazatelů a pak ještě alokovat každou položku hashtable_item. Všechno to přitom leží někde na různých místech haldy (heapu) – alokace jsou drahé! Pro přístup k položce je nutné v paměti skočit na tu instanci hashtable, z ní skočit na pole a pak skočit na ukazatel – takové adresování je taky drahé! Ve strukturách tohoto typu může být vhodnější použít „flexible array member“ na konci:</li>
  </ul>

  ```
  typedef struct {
      // různá metadata
      hashtable_item items[];
  } better_hashtable;
  ```
  <ul style="list-style-type: none;">
    <li>Pak stačí alokovat celou TRP pomocí:</li>
  </ul>

  ```
  malloc(sizeof(better_hashtable) + NUMBER_OF_ITEMS * sizeof(hashtable_item));
  ```
  <ul style="list-style-type: none;">
    <li>a máte to v paměti všechno pěkně za sebou, pro přístup k položce tabulky v podstatě stačí jediná dereference.</li>
  </ul>

### 2) Prémiové body, korekce, malus:
- Aktivita na fóru, rychlostní soutěž, pozdní odevzdání, formální nedostatky.

### 3) Dokumentace:

- Za většinu prohřešků bylo podle závažnosti strhnuto 0,1 až 0,7 bodu.
- Poznámky k hodnocení dokumentací jsou vesměs řazeny dle klesající závažnosti.

#### Seznam zkratek, které se mohou vyskytnout v komentářích k hodnocení dokumentace:

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
- **STYLE** - nesrozumitelné věty, dlouhá či těžko pochopitelná souvětí
- **gram.** = gramatické chyby
- **LANG** = míchání jazyků v rámci dokumentace (nejčastěji češtiny a slovenštiny)
- **HOV** = použití hovorových výrazů, zbytečné používání anglických výrazů (např. pushnout, symtable, list, array)
- **BLOK** = text není zarovnán do bloku
- **KAPTXT** = dokumentace obsahuje více nadpisů hned za sebou
- **MEZ** = špatné psaní mezer (např. kolem závorek a interpunkce)
- **SAZBA** = alespoň identifikátory proměnných a funkcí se patří sázet písmem s jednotnou šířkou písmen (např. font Courier)

#### Pár poznámek k typickým chybám v dokumentaci pro 2024/2025:
- **LLT** - letos poměrně často studentům nějaká pravidla chyběla nebo řešitelé ignorovali množiny FOLLOW ve spojení s epsilon pravidly. Srážky cca 0,3 bodu.
- **SA** - dostkrát se objevilo tvrzení, že rekurzivní sestup implementují tak, že mají funkci pro každé pravidlo - bylo strženo cca cca 0,2 bodu.
- **SéA** - obvykle málo konkrétní popis toho, co kontrolují.
- **TS** - chybí nějaká část popisu, nejčastěji podrobnější popis struktury tabulky (např. zda máte více tabulek nebo jednu).