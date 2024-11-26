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
##
run_test() {
    local inputFile=$1
    local expectedOut=$2
    local base_name=$(basename "$inputFile" .zig)
    local output_file="$OUTPUT_DIR/$base_name.ifj24code"

    # Výpis označení testu
    echo -e "${PINK}TESTING: ${NORMAL}$inputFile"

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
    actualOut=$("$INTERPRET" "$output_file" 2>&1)

    # Porovnání výstupu interpretu s očekávaným výstupem
    if [[ "$actualOut" == "$expectedOut" ]]; then
        echo -e "${GREEN}[PASSED]${NORMAL}\n"
        passed=$((passed + 1))
    else
        echo -e "${YELLOW}Expected:${NORMAL} $expectedOut"
        echo -e "${YELLOW}Got:${NORMAL} $actualOut"
        failed=$((failed + 1))
        failedTests+=("$inputFile")
        echo -e "${RED}[FAILED]${NORMAL}\n"
    fi

    testCnt=$((testCnt + 1))  # inkrementace počítadla testů
}

# Seznam jednotlivých testů
run_test "$EXAMPLES_PATH/example1.zig" "xxx"
run_test "$EXAMPLES_PATH/example2.zig" "xxx"
run_test "$EXAMPLES_PATH/example3.zig" "xxx"
run_test "$EXAMPLES_PATH/hello.zig" "xxx"
run_test "$EXAMPLES_PATH/fun.zig" "xxx"
run_test "$EXAMPLES_PATH/multiline.zig" "xxx"

# Výpis shrnutí na závěr testovací sady
echo -e "${BLUE}SUMMARY:${RESET}"
testCnt=$((testCnt - 1))
if [ $failed -eq 0 ]; then
    echo -e "${GREEN}Passed: $passed/$testCnt${NORMAL}"
    exit 0      # návratový kód 0 znamená úspěšné dokončení testů
else
    echo -e "Passed: $passed/$testCnt"
    echo -e "\n${BLUE}FAILED TESTS:${RESET}"
    for test in "${failedTests[@]}"; do
        echo -e "${RED}$test${NORMAL}"
    done
    exit 1      # návratový kód 1 znamená neúspěšné dokončení testů
fi