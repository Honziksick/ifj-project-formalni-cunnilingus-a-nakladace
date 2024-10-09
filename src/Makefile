################################################################################
#                                                                              #
# Název projektu:   Implementace překladače imperativního jazyka IFJ24         #
#                                                                              #
# Soubor:           Makefile                                                   #
# Autor:            Jan Kalina <xkalinj00>                                     #
#                                                                              #
# Datum:            29.09.2024                                                 #
# Poslední změna:   8.10.2024                                                  #
#                                                                              #
# Tým:      Tým xkalinj00                                                      #
# Členové:  Farkašovský Lukáš    <xfarkal00>                                   #
#           Hýža Pavel           <xhyzapa00>                                   #
#           Kalina Jan           <xkalinj00>                                   #
#           Krejčí David         <xkrejcd00>                                   #
#                                                                              #
# Popis:    Tento soubor obsahuje Makefile pro "Společný projekt do předmětů   #
#           IFJ a IAL: Implementace překladače imperativního jazyka IFJ24".    #
#           Makefile vychází ze souboru `Makefile` vytvořeného mnou (studentem #
#           <xkalinj00>) v rámci druhého projektu do předmětu IVS na VUT FIT.  #
#                                                                              #
################################################################################

################################################################################
#                                                                              #
#                  ZÁKLADNÍ NASTAVENÍ A DEFINICE PRO MAKEFILE                  #
#                                                                              #
################################################################################

###                                             ###
#  Proměnné pro opakující se částí názvů souborů  #
###                                             ###

# Název projektu
EXECUTABLE = ifj24_compiler

# Název ZIP archivu pro odevzdání projektu
PACK_NAME = xkalinj00


###                                       ###
#  Přepínače pro spouštění příkazu $(MAKE)  #
###                                       ###

# Spuštění 'make' v tichém režímu (bez výpisu událostí)
$(VERBOSE)SILENTOPT = -s


###                                        ###
#  Definice cest pro některé vstupy/výstupy  #
###                                        ###

# Adresář pro generování dokumentace
DOC_DIR = ../doc

# Cesta ke zdrojovým souborům Google Test a Google Mock knihoven
GTEST_DIR = $(TEST_DIR)/googletest-src/googletest
GMOCK_DIR = $(TEST_DIR)/googletest-src/googlemock


###                                                         ###
#  Deklarace cest k adresářům určeným pro kompilaci projektu  #
###                                                         ###

# Cesta k adresáři se zdrojovými soubory překladače
SRC_DIR = .

# Cesta k adresáři s testy
TEST_DIR = ../test

# Adresáře pro umístění postavených souborů
BUILD_DIR = $(SRC_DIR)/build
TEST_BUILD_DIR = $(TEST_DIR)/build_test
COVERAGE_BUILD_DIR = $(TEST_DIR)/build_coverage

# Adresář s připraveným projektem pro zabalení
PACK_DIR = ../pack
ARCHIVE_DIR = $(PACK_DIR)/$(PACK_NAME)


###                                   ###
#  Předdefinované flagy pro kompilátor  #
###                                   ###

# Definice kompilátoru
CC = gcc
CXX = g++

# Flagy obsahující cesty k hlavičkovým souborům
CFLAGS += -I$(GTEST_DIR) -I$(GMOCK_DIR) -I"$(CURDIR)"

# Parametry pro archivaci objektových souborů
STD_C = -std=c17
STD_CXX =-std=c++17
WARNING_FLAGS = -Wall -Wextra -Werror -pedantic -Wshadow -Wconversion -pthread
DEBUG_FLAGS = -g
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
SANITIZE_FLAGS = -fsanitize=address -fsanitize=undefined

# Flagy pro překlad různých částí projektu
CFLAGS_STD = $(STD_C) -O2 $(WARNING_FLAGS) 
CFLAGS_TEST = $(STD_C) -O0 $(WARNING_FLAGS) $(DEBUG_FLAGS) $(SANITIZE_FLAGS)
CXXFLAGS_TEST = $(STD_CXX) -O0 $(DEBUG_FLAGS) $(WARNING_FLAGS) $(SANITIZE_FLAGS)
CFLAGS_COV = $(STD_C) -O0 $(WARNING_FLAGS) $(DEBUG_FLAGS) $(COVERAGE_FLAGS) $(SANITIZE_FLAGS)
CXXFLAGS_COV = $(STD_CXX) -O0 $(WARNING_FLAGS) $(DEBUG_FLAGS) $(COVERAGE_FLAGS) $(SANITIZE_FLAGS)


###                                                            ###
#  Proměnné obsahující seznamy zdrojových a objektových souborů  #
###                                                            ###

# Seznam všech zdrojových souborů překladače a testů
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

# Seznam všech objektových souborů přeložených pro testování
TEST_SRC_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(TEST_BUILD_DIR)/%.o,$(SRC_FILES))
TEST_TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.cpp,$(TEST_BUILD_DIR)/%.o,$(TEST_FILES))

# Seznam všech objektových souborů přeložených pro code coverage
COV_SRC_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(COVERAGE_BUILD_DIR)/%.o,$(SRC_FILES))
COV_TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.cpp,$(COVERAGE_BUILD_DIR)/%.o,$(TEST_FILES))


################################################################################
#                                                                              #
#                          ZÁKLADNÍ PŘÍKAZY MAKEFILE                           #
#                                                                              #
################################################################################

# Příkaz '.PHONY' určuje, že následující příkazy nejsou nikdy brány jako soubory
.PHONY: all build help clean test doc coverage pack

### MC # all: # Provede sestavení celého překladače určeného k nasazení
all: build

### MC # build: # Sestaví překladač týmu "Tým xkalinj00"
build:
	@echo "\033[31mPřekladač tým xkalinj00 alias \"Formální cunnilingus a nakladače\" ještě není připraven.\033[0m"
# NEZAPOMENOUT PŘESUNOUT VÝSLEDNOU BINÁRKU

# Definice zkratek pro kategorie příkazů
CATEGORIES := MC C B T CC DEV O

### MC # help: # Vypíše nápovědu k použití Makefile
help:
	@$(MAKE) $(SILENTOPT) install-help-dep
	@{ \
	for CATEGORY in $(CATEGORIES); do \
		case $$CATEGORY in \
		"MC") FULL_CAT="Main Commands";; \
		"C") FULL_CAT="Clean";; \
		"B") FULL_CAT="Build Modules and Submodules";; \
		"T") FULL_CAT="Test";; \
		"CC") FULL_CAT="Code Coverage";; \
		"DEV") FULL_CAT="Install Dependencies";; \
		"O") FULL_CAT="Others";; \
		esac; \
		echo "\033[0;33m$$FULL_CAT:\033[0m"; \
		grep -E "^### $$CATEGORY # [a-zA-Z0-9_\-]+:.*?# .*$$" $(MAKEFILE_LIST) | \
		sort -f | \
		awk 'BEGIN {FS = ":.*?# "}; \
		{gsub(/^### [A-Z]+ # /, "", $$1); \
		printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'; \
		echo ""; \
	done; \
	} | less -R

### MC # clean: # Odstraní všechny soubory vytvořené během kompilace (včetně dokumentace a archivu)
clean: clean-build clean-test clean-coverage clean-doc clean-pack

### MC # test: # Sestaví knihovny GoogleTest a GoogleMock a spustí test překladače
test: build-test
	$(TEST_BUILD_DIR)/$(EXECUTABLE)_test

### MC # doc: # Vygeneruje dokumentaci k projektu do adresáře `../doc/`
doc:
	@$(MAKE) $(SILENTOPT) install-doc-dep
	$(MAKE) $(SILENTOPT) clean-doc
	doxygen Doxyfile 
	cd $(DOC_DIR)/html && grep -v 'target="_self">resources\|target="_self">doc' files.html > temp.html && mv temp.html files.html
	@echo '<html><head><meta http-equiv="refresh" content="0; url=html/index.html"></head></html>' > $(DOC_DIR)/documentation.html
	@echo -e "\033[33mChcete otevřít HTML dokumentaci v hlavním systémovém prohlížeči? (y/n): \033[0m"
	@bash -c 'read -t 5 -p "" choice; \
	if [ "$$choice" = "y" ]; then \
		if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null; then \
			cmd.exe /C start $(DOC_DIR)/documentation.html; \
		else \
			xdg-open $(DOC_DIR)/documentation.html; \
		fi \
	fi'

### MC # coverage: # Provede analýzu pokrytí kódu a vygeneruje HTML přehled
coverage: 
	@$(MAKE) $(SILENTOPT) install-cov-dep
	$(MAKE) build-coverage
	$(MAKE) run-test-coverage
	$(MAKE) process-coverage
	@echo -e "\033[33mChcete otevřít soubor HTML v hlavním systémovém prohlížeči? (y/n): \033[0m"
	@bash -c 'read -t 5 -p "" choice; \
	if [ "$$choice" = "y" ]; then \
		if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null; then \
			cmd.exe /C start $(COVERAGE_BUILD_DIR)/code_coverage_web_result.html; \
		else \
			xdg-open $(COVERAGE_BUILD_DIR)/code_coverage_web_result.html; \
		fi \
	fi'

### MC # pack: # Vytvoří ZIP archiv se soubory určenými k odevzdání
pack:
	@$(MAKE) $(SILENTOPT) install-pack-dep
	$(MAKE) $(SILENTOPT) clean-pack
	mkdir -p $(PACK_DIR)
	$(MAKE) $(SILENTOPT) pack-prepare
	@echo ""
	cd $(PACK_DIR)/$(ARCHIVE_DIR) && zip -r ../$(PACK_NAME) ./


################################################################################
#                                                                              #
#                        SPECIALIZOVANÉ PŘÍKAZY 'CLEAN'                        #
#                                                                              #
################################################################################

### C # clean-build: # Odstraní adresář 'src/build' s verzí překladače k nasazení
clean-build:
	rm -rf $(BUILD_DIR)

### C # clean-build: # Odstraní adresář '../test/build-test' s unit testy překladače
clean-test:
	rm -rf $(TEST_BUILD_DIR)

### C # clean-doc: # Odstraní veškerý obsah adresáře '../doc' (kromě '../doc/resources/')
clean-doc:
	find $(DOC_DIR) -mindepth 1 ! -path '$(DOC_DIR)/resources*' -delete || true

### C # clean-coverage: # Odstraní adresář '../test/build-coverage' s pokrytím kódu
clean-coverage:
	rm -rf $(COVERAGE_BUILD_DIR)

### C # clean-pack: # Odstraní složku '../pack' pro zabalení projektu (včetně archivu)
clean-pack:
	rm -rf $(PACK_DIR)


################################################################################
#                                                                              #
#                         KOMPILACE DÍLČÍCH SUBMODULŮ                          #
#                                                                              #
################################################################################

###                                                                          ###
#                         Podpůrné KNIHOVNY a SUBMODULY                        #
###                                                                          ###

ERROR_LIB_XKALINJ00 = error
STRING_LIB_XFARKAL00 = dynamic_string
SYMTABLE_XKREJCD00 = symtable

### B # build-error: # Sestaví knihovnu k řízení chybových stavů
build-error: $(BUILD_DIR)/$(ERROR_LIB_XKALINJ00).o

### B # build-string: # Sestaví knihovnu operací nad dynamickým stringem
build-string: $(BUILD_DIR)/$(STRING_LIB_XFARKAL00).o

### B # build-symtable: # Sestaví knihovnu operací nad tabulkou symbolů typu TRP-izp
build-symtable: $(BUILD_DIR)/$(SYMTABLE_XKREJCD00).o

# Stavba objekotvých souborů jednotlivých podpůrných submodulů a knihoven
$(BUILD_DIR)/$(ERROR_LIB_XKALINJ00).o: $(SRC_DIR)/$(ERROR_LIB_XKALINJ00).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(STRING_LIB_XFARKAL00).o: $(SRC_DIR)/$(STRING_LIB_XFARKAL00).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(SYMTABLE_XKREJCD00).o: $(SRC_DIR)/$(SYMTABLE_XKREJCD00).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@


###                                                                          ###
#             SUBMODULY syntaktického analyzátoru alias "parseru"              #
###                                                                          ###

SUB_PARSER_XKALINJ00 = xkalinj00
SUB_PARSER_XFARKAL00 = xfarkal00
SUB_PARSER_XHYZAPA00 = xhyzapa00
SUB_PARSER_XKREJCD00 = xkrejcd00


###                                                                          ###
#                      SUBMODULY sémantického analyzátoru                      #
###                                                                          ###

SUB_SEMANT_XKALINJ00 = xkalinj00
SUB_SEMANT_XFARKAL00 = xfarkal00
SUB_SEMANT_XHYZAPA00 = xhyzapa00
SUB_SEMANT_XKREJCD00 = xkrejcd00


################################################################################
#                                                                              #
#                  KOMPILACE JEDNOTLIVÝCH MODULŮ ZE SUBMODULŮ                  #
#                                                                              #
################################################################################

SCANNER_XHYZAPA00 = scanner
PARSER_TEAM = parser

### B # build-scanner: # Sestaví lexikální analyzátor alias 'scanner'
build-scanner: $(BUILD_DIR)/$(SCANNER_XHYZAPA00).o

### B # build-parser: # Sestaví syntaktický analyzátor alias 'parser'
build-$(PARSER_TEAM): $(BUILD_DIR)/$(PARSER_TEAM).o

# Stavba objekotvých souborů jednotlivých modulů překladače
$(BUILD_DIR)/$(SCANNER_XHYZAPA00).o: $(SRC_DIR)/$(SCANNER_XHYZAPA00).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(PARSER_TEAM).o: $(SRC_DIR)/$(PARSER_TEAM).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@


################################################################################
#                                                                              #
#             KOMPILACE VŠECH VYTVOŘENÝCH TESTŮ DO JEDNOHO SOUBORU             #
#                                                                              #
################################################################################

###                                                                          ###
#                    Kompilace unit TESTU CELÉHO PŘEKLADAČE                    #
###                                                                          ###

### T # build-test: # Sestaví testovací program sdružující veškeré testy překladače
build-test: $(TEST_SRC_OBJ_FILES) $(TEST_TEST_OBJ_FILES) $(TEST_BUILD_DIR)/gtest_main.a $(TEST_BUILD_DIR)/gtest.a
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS_TEST) $(CXXFLAGS_TEST) $(TEST_SRC_OBJ_FILES) $(TEST_TEST_OBJ_FILES) \
		$(TEST_BUILD_DIR)/gtest_main.a -o $(TEST_BUILD_DIR)/$(EXECUTABLE)_test

# Stavba objektových souborů modulů a submodulů s přepínači pro pokrytí kódu
$(TEST_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@


###                                                                          ###
#                        Kompilace GOOGLE TEST KNIHOVNY                        #
###                                                                          ###

### T # build-test-libs: # Sestaví knihovny Google Test a Google Mock
build-test-libs: $(TEST_BUILD_DIR)/gtest_main.a $(TEST_BUILD_DIR)/gtest.a

# Stavba objektových souborů Google Test knihoven
$(TEST_BUILD_DIR)/gtest-all.o: $(GTEST_DIR)/src/gtest-all.cc
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/gtest_main.o: $(GTEST_DIR)/src/gtest_main.cc
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

# Stavba statických knihoven pro Google Test
$(TEST_BUILD_DIR)/gtest.a: $(TEST_BUILD_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(TEST_BUILD_DIR)/gtest_main.a: $(TEST_BUILD_DIR)/gtest-all.o \
								$(TEST_BUILD_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


################################################################################
#                                                                              #
#                      KOMPILACE TESTŮ DÍLČÍCH SUBMODULŮ                       #
#                                                                              #
################################################################################

###                                                                          ###
#                    TESTY podpůrných KNIHOVEN a SUBMODULŮ                     #
###                                                                          ###

### T # build-error-test: # Sestaví test knihovny pro řízení chybových stavů
build-error-test: $(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00)_test

### T # run-error-test: # Spustí test knihovny pro řízení chybových stavů
run-error-test: $(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00)_test
	$(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00)_test

### T # build-string-test: # Sestaví test knihovny operací nad dynamickým stringem
build-string-test: $(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00)_test

### T # run-string-test: # Spustí test knihovny operací nad dynamickým stringem
run-string-test: $(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00)_test
	$(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00)_test

### T # build-symtable-test: # Sestaví test knihovny operací nad tabulkou symbolů
build-symtable-test: $(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00)_test

### T # run-symtable-test: # Spustí test knihovny operací nad tabulkou symbolů
run-symtable-test: $(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00)_test
	$(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00)_test

# Stavba testů podpůrných knihoven a submodulů
$(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00)_test: $(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00)_test.o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00)_test.o: $(TEST_DIR)/$(ERROR_LIB_XKALINJ00)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00).o: $(SRC_DIR)/$(ERROR_LIB_XKALINJ00).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00)_test: $(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00)_test.o \
											$(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00)_test.o: $(TEST_DIR)/$(STRING_LIB_XFARKAL00)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00).o: $(SRC_DIR)/$(STRING_LIB_XFARKAL00).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00)_test: $(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00)_test.o \
											$(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00)_test.o: $(TEST_DIR)/$(SYMTABLE_XKREJCD00)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00).o: $(SRC_DIR)/$(SYMTABLE_XKREJCD00).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@


###                                                                          ###
#                  TESTY SUBMODULŮ syntaktického analyzátoru                   #
###                                                                          ###


###                                                                          ###
#                   TESTY SUBMODULŮ sémantického analyzátoru                   #
###                                                                          ###


################################################################################
#                                                                              #
#                  KOMPILACE INTEGRAČNÍCH TESTŮ CELÝCH MODULŮ                  #
#                                                                              #
################################################################################

### T # build-scanner-test: # Sestaví testy lexikálního analyzátoru alias 'scanneru'
build-scanner-test: $(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00)_test

### T # run-scanner-test: # Spustí testy lexikálního analyzátoru alias 'scanneru'
run-scanner-test: $(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00)_test
	$(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00)_test

# Stavba integračních testů celých modulů 
$(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00)_test: $(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00)_test.o \
											$(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00)_test.o: $(TEST_DIR)/$(SCANNER_XHYZAPA00)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00).o: $(SRC_DIR)/$(SCANNER_XHYZAPA00).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@


################################################################################
#                                                                              #
#                           GENEROVÁNÍ CODE COVERAGE                           #
#                                                                              #
################################################################################

###                                                                          ###
#                   ZPRACOVÁNÍ vygenerovaného CODE COVERAGE                    #
###                                                                          ###

### CC # build-coverage: # Sestaví všechny zdrojové soubory a testy s přepínači pro pokrytí kódu
build-coverage: $(COV_SRC_OBJ_FILES) $(COV_TEST_OBJ_FILES) $(COVERAGE_BUILD_DIR)/gtest_main.a $(COVERAGE_BUILD_DIR)/gtest.a
	@mkdir -p $(COVERAGE_BUILD_DIR)
	$(CXX) $(CFLAGS_COV) $(CXXFLAGS_COV) $(COV_SRC_OBJ_FILES) $(COV_TEST_OBJ_FILES) \
		$(COVERAGE_BUILD_DIR)/gtest_main.a -o $(COVERAGE_BUILD_DIR)/$(EXECUTABLE)_coverage

### CC # run-test-coverage: # Spustí všechny testy generující pokrytí kódu
run-test-coverage:
	$(COVERAGE_BUILD_DIR)/$(EXECUTABLE)_coverage

### CC # process-coverage: # Zpracuje data o pokrytí kódu a vygeneruje HTML výstup
process-coverage:
	lcov --quiet --directory $(COVERAGE_BUILD_DIR) --capture \
	--output-file $(COVERAGE_BUILD_DIR)/coverage.info
	@lcov --quiet --remove $(COVERAGE_BUILD_DIR)/coverage.info '/usr/*' \
	--output-file $(COVERAGE_BUILD_DIR)/filtered_coverage.info
	@lcov --quiet --remove $(COVERAGE_BUILD_DIR)/filtered_coverage.info \
	'*/googletest-src/*' --output-file $(COVERAGE_BUILD_DIR)/filtered_coverage.info
	@lcov --quiet --remove $(COVERAGE_BUILD_DIR)/filtered_coverage.info \
	'*/profiler/*' --output-file $(COVERAGE_BUILD_DIR)/filtered_coverage.info
	genhtml $(COVERAGE_BUILD_DIR)/filtered_coverage.info \
	--output-directory $(COVERAGE_BUILD_DIR)
	@mkdir -p $(COVERAGE_BUILD_DIR)/coverage_data
	@find $(COVERAGE_BUILD_DIR) -maxdepth 1 -type f -exec mv -f {} $(COVERAGE_BUILD_DIR)/coverage_data/ \;
	@find $(COVERAGE_BUILD_DIR) -maxdepth 1 -type d ! -name 'coverage_data' ! -path '$(COVERAGE_BUILD_DIR)' -exec mv {} $(COVERAGE_BUILD_DIR)/coverage_data/ \;
	@echo '<html><head><meta http-equiv="refresh" content="0; url=coverage_data/index.html"></head></html>' > $(COVERAGE_BUILD_DIR)/code_coverage_web_result.html


###                                                                          ###
#                KOMPILACE souborů potřebných pro CODE COVERAGE                #
###                                                                          ###

# Stavba objektových souborů s přepínači pro pokrytí kódu
$(COVERAGE_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(COVERAGE_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_COV) -c $< -o $@

$(COVERAGE_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(COVERAGE_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_COV) -c $< -o $@

# Stavba objektových souborů Google Test knihoven s přepínačí pro pokrytí kódu
$(COVERAGE_BUILD_DIR)/gtest-all.o: $(GTEST_DIR)/src/gtest-all.cc
	@mkdir -p $(COVERAGE_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_COV) -c $< -o $@

$(COVERAGE_BUILD_DIR)/gtest_main.o: $(GTEST_DIR)/src/gtest_main.cc
	@mkdir -p $(COVERAGE_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_COV) -c $< -o $@

$(COVERAGE_BUILD_DIR)/gtest.a: $(COVERAGE_BUILD_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(COVERAGE_BUILD_DIR)/gtest_main.a: $(COVERAGE_BUILD_DIR)/gtest-all.o \
								$(COVERAGE_BUILD_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


################################################################################
#                                                                              #
#                  ZABALENÍ PROJEKTU PRO ODEVZDÁNÍ do '.ZIP'                   #
#                                                                              #
################################################################################

### O # pack-prepare: # Zkopíruje všechny potřebné soubory do adresáře '../pack/xkalinj00'
pack-prepare:
	@{ \
		missing_files=0; \
		if [ -d "$(SRC_DIR)" ]; then \
			rsync -a --include '*/' --include '*.c' --exclude '*' --exclude '*/' --prune-empty-dirs $(SRC_DIR)/ $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			echo "\033[0;31m\nError: Adresář "$(SRC_DIR)" neexistuje.\033[0m"; \
		fi; \
		if [ -d "$(SRC_DIR)" ]; then \
			rsync -a --include '*/' --include '*.h' --exclude '*' --exclude '*/' --prune-empty-dirs $(SRC_DIR)/ $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			echo "\033[0;31m\nError: Adresář "$(SRC_DIR)" neexistuje.\033[0m"; \
		fi; \
		if [ -f "$(SRC_DIR)/Makefile" ]; then \
			rsync -a $(SRC_DIR)/Makefile $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -d "$(TEST_DIR)" ]; then \
			rsync -a --include '*/' --include '*.cpp' --exclude '*' --exclude '*/' --prune-empty-dirs $(TEST_DIR)/ $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			echo "\033[0;31m\nError: Adresář "$(TEST_DIR)" neexistuje.\033[0m"; \
		fi; \
		if [ -f "../rozsireni" ]; then \
			rsync -a ../rozsireni $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -f "../dokumentace.pdf" ]; then \
			rsync -a ../dokumentace.pdf $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -f "../rozdeleni" ]; then \
			rsync -a ../rozdeleni $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		echo "\033[0;32m\nSeznam zkopírovaných souborů:\033[0m"; \
		find $(PACK_DIR) -type f -printf "\033[0;32m%p\033[0m\n"; \
		if [ "$$missing_files" -eq 1 ]; then \
			echo "\033[0;31m\nSeznam chybějících souborů:\033[0m"; \
		fi; \
		if [ ! -f "$(PACK_DIR)/$(ARCHIVE_DIR)/Makefile" ]; then \
			echo "\033[0;31mError: Soubor "Makefile" nebyl zkopírován.\033[0m"; \
		fi; \
		if [ ! -f "$(PACK_DIR)/$(ARCHIVE_DIR)/dokumentace.pdf" ]; then \
			echo "\033[0;31mError: Soubor "dokumentace.pdf" nebyl zkopírován.\033[0m"; \
		fi; \
		if [ ! -f "$(PACK_DIR)/$(ARCHIVE_DIR)/rozdeleni" ]; then \
			echo "\033[0;31mError: Soubor "rozdeleni" nebyl zkopírován.\033[0m"; \
		fi; \
		if [ ! -f "$(PACK_DIR)/$(ARCHIVE_DIR)/rozsireni" ]; then \
			echo "\033[0;31mError: Soubor "rozsireni" nebyl zkopírován.\033[0m"; \
		fi; \
	}


################################################################################
#                                                                              #
#                    CÍLE PRO INSTALACI POTŘEBNÝCH NÁSTROJŮ                    #
#                                                                              #
################################################################################

### DEV # install-dev-dep: # Nainstaluje závislosti potřebné pro používání všech funkcí 'Makefile'
install-dev-dep: update-dep install-doc-dep install-pack-dep install-help-dep install-cov-dep

### DEV # install-help-dep: # Nainstaluje závislosti potřebné pro výpis nápovědy k 'Makefile'
install-help-dep:
	@dpkg -s less >/dev/null 2>&1 || (echo "Instaluji less" && sudo apt-get install less)

### DEV # install-cov-dep: # Nainstaluje závislosti potřebné pro generování pokrytí kódu
install-cov-dep:
	@dpkg -s lcov >/dev/null 2>&1 || (echo "Instaluji lcov" && sudo apt-get install lcov)

### DEV # install-doc-dep: # Nainstaluje závislosti potřebné pro generování dokumentace
install-doc-dep:
	@dpkg -s doxygen >/dev/null 2>&1 || (echo "Instaluji doxygen" && sudo apt-get install doxygen)

### DEV # install-pack-dep: # Nainstaluje závislosti potřebné pro zabalení projektu
install-pack-dep:
	@dpkg -s rsync >/dev/null 2>&1 || (echo "Instaluji rsync" && sudo apt-get install rsync)
	@dpkg -s zip >/dev/null 2>&1 || (echo "Instaluji zip" && sudo apt-get install zip)

### DEV # dev-update-dep: # Aktualizuje seznam dostupných balíčků
update-dep:
	sudo apt-get update -y