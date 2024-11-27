#!/bin/bash

# Cesty
EXECUTABLE="../src/ifj24_compiler"
INTERPRET="../test/ic24int"
BASE_PATH="../test/test_examples/ifj24code_examples"
EXAMPLES_PATH="$BASE_PATH/programs"
OUTPUT_DIR="$BASE_PATH/ifj24code_output"
BUILD_DIR="../src"

# Počítadla testů
testCnt=1
passed=0
failed=0
failedTests=()

# Barvy pro výpis
GREEN='\033[0;32m'
RED='\033[0;31m'
PINK='\033[0;35m'
YELLOW='\033[38;5;220m'
BLUE='\033[38;5;14m'
NORMAL='\033[0m'

##
#  Funkce pro spuštění jednoho testu
#  Tato funkce spustí testovací program s vstupem zadaným v souboru a porovná 
#  jeho výstup s očekávaným výstupem. Pokud se výstupy shodují, test je 
#  považován za úspěšný. Pokud se výstupy neshodují, test je považován za 
#  neúspěšný a je přidán do seznamu neúspěšných testů.
#
#  inputFile:   Vstupní soubor
#  expectedOut: Očekávaný výstup
#  stdInInput:  Vstupní data rpo STDIN (volitelné)
##
run_test() {
    local inputFile=$1
    local expectedOut=$2
    local stdinInput=${3:-""}
    local base_name=$(basename "$inputFile" .zig)
    local output_file="$OUTPUT_DIR/$base_name.ifj24code"

    # Výpis označení testu
    echo -e "${PINK}"$testCnt") TESTING: ${NORMAL}$inputFile"

    # Vytvoření výstupního adresáře, pokud neexistuje
    mkdir -p "$OUTPUT_DIR"

    # Kontrola, zda existuje spustitelný soubor, pokud ne, zavolá make
    if [ ! -f "$EXECUTABLE" ]; then
        echo -e "\n$EXECUTABLE not found, running make in $BUILD_DIR"
        (cd "$BUILD_DIR" && make)
    fi

    # Kompilace
    "$EXECUTABLE" < "$inputFile" > "$output_file"

    # Spuštění interpretu a zachycení výstupu
    local actualOut
    if [ -n "$stdinInput" ]; then
        actualOut=$(echo -e "$stdinInput" | "$INTERPRET" "$output_file" 2>&1)
    else
        actualOut=$("$INTERPRET" "$output_file" 2>&1)
    fi

    echo -e "${YELLOW}Expected Output:${NORMAL}"
    echo -e "$expectedOut"
    echo -e "${YELLOW}Actual Output:${NORMAL}"
    echo -e "$actualOut"

    # Porovnání výstupu interpretu s očekávaným výstupem
    if [[ "$actualOut" == "$expectedOut" ]]; then
        echo -e "${GREEN}[PASSED]${NORMAL}\n"
        passed=$((passed + 1))
    else
        failed=$((failed + 1))
        failedTests+=("$testCnt) $inputFile")
        echo -e "${RED}[FAILED]${NORMAL}\n"
    fi

    testCnt=$((testCnt + 1))  # inkrementace počítadla testů
}

# Seznam jednotlivých testů
run_test "$EXAMPLES_PATH/example1.zig" $'Zadejte cislo pro vypocet faktorialu\nVysledek: 0x1.ep6 = 120' "5"
run_test "$EXAMPLES_PATH/example2.zig" $'Zadejte cislo pro vypocet faktorialu: Vysledek: 720' "6" 
run_test "$EXAMPLES_PATH/example3.zig" "$(cat <<'END_EXPECTED'
Toto je nejaky text v programu jazyka IFJ24
Toto je nejaky text v programu jazyka IFJ24, ktery jeste trochu obohatime
Zadejte serazenou posloupnost vsech malych pismen a-h, Spatne zadana posloupnost, zkuste znovu:
Spatne zadana posloupnost, zkuste znovu:
Spravne zadano!
xxxabcdefhg
END_EXPECTED
)" $'xxx\nabcdefhg\nabcdefgh'
run_test "$EXAMPLES_PATH/hello.zig" $'Hello from IFJ24'
run_test "$EXAMPLES_PATH/fun.zig" $'calling f with 10\ncalling g with 9\ncalling f with 9\nres: 8'
run_test "$EXAMPLES_PATH/multiline.zig" "$(cat <<'END_EXPECTED'
To\tto 
 je 

 nejaky\n 
  text  // ve viceradkovem retezcovem literalu nelze mit komentar
END_EXPECTED
)"
run_test "$EXAMPLES_PATH/wanna_die.zig" "\n"

# Výpis shrnutí na závěr testovací sady
echo -e "${BLUE}SUMMARY:${RESET}"
testCnt=$((testCnt - 1))
if [ $failed -eq 0 ]; then
    echo -e "${GREEN}ALL passed: $passed/$testCnt${NORMAL}"
    exit 0      # návratový kód 0 znamená úspěšné dokončení testů
else
    echo -e "${GREEN}Passed: $passed/$testCnt${NORMAL}"
    echo -e "\n${BLUE}FAILED TESTS:${RESET}"
    for test in "${failedTests[@]}"; do
        echo -e "${RED}$test${NORMAL}"
    done
    exit 1      # návratový kód 1 znamená neúspěšné dokončení testů
fi