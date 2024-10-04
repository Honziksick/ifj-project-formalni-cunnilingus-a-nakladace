################################################################################
#                                                                              #
# Název projektu:   Implementace překladače imperativního jazyka IFJ24         #
#                                                                              #
# Soubor:           Makefile                                                   #
# Autor:            Jan Kalina <xkalinj00>                                     #
#                                                                              #
# Datum:            29.09.2024                                                 #
# Poslední změna:   29.09.2024                                                 #
#                                                                              #
# Tým:      Tým xkalinj00                                                      #
# Členové:  Farkašovský Lukáš    <xfarkal00>                                   #
#           Hýža Pavel           <xhyzapa00>                                   #
#           Kalina Jan           <xkalinj00>                                   #
#           Krejčí David         <xkrejcd00>                                   #
#                                                                              #
# Popis:    Tento soubor obsahuje Makefile pro "Společný projekt do předmětů   #
#           IFJ a IAL: Implementace překladače imperativního jazyka IFJ24".    #
#           Makefile vychází ze souboru `Makefile` vytvořeného studentem       #
#           <xkalinj00> v rámci druhého projektu do předmětu IVS na VUT FIT.   #
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
PACK_NAME = xkalinj00.zip


###                                       ###
#  Přepínače pro spouštění příkazu $(MAKE)  #
###                                       ###

# Spuštění 'make' v tichém režímu (bez výpisu událostí)
$(VERBOSE)SILENTOPT = -s


###                                 ###
#  Definice cest pro některé výstupy  #
###                                 ###

# Adresář pro generování dokumentace
DOC_OUT_DIR = ../doc


###                                                          ###
#  Definice cest ke složkám se soubory pro testování projektu  #
###                                                          ###

# Cesta k adresáři s testy
TEST_DIR = ../test

# Cesta ke zdrojovým souborům Google Test a Google Mock knihoven
GTEST_DIR = $(TEST_DIR)/googletest-src/googletest
GMOCK_DIR = $(TEST_DIR)/googletest-src/googlemock


###                                                         ###
#  Deklarace cest k adresářům určeným pro kompilaci projektu  #
###                                                         ###

# Cesta k adresáři se zdrojovými soubory překladače
SRC_DIR = .

# Adresáře pro umístění postavených souborů
BUILD_DIR = $(SRC_DIR)/build
TEST_BUILD_DIR = $(TEST_DIR)/build_test
COVERAGE_BUILD_DIR = $(TEST_DIR)/build_coverage

# Adresář s připraveným projektem pro zabalení
PACK_DIR = ../$(PACK_NAME)


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


###                                                           ###
#  Proměnné obsahující seznamy zdojových a objektových souborů  #
###                                                           ###

# Seznam všech zdrojových souborů
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

### all: ## Provede sestavení celého překladače
all: build

### build: ## Sestaví překladač týmu "Tým xkalinj00"
build:
	@echo "\033[31mPřekladač tým xkalinj00 alias \"Formální cunnilingus a nakladače\" ještě není připraven.\033[0m"
# NEZAPOMENOUT PŘESUNOUT VÝSLEDNOU BINÁRKU

### help: ## Vypíše nápovědu k použití Makefile
help:
	$(MAKE) $(SILENTOPT) install-help-dep
	@{ \
	echo "Proměnné:"; \
	grep -E '### [A-Z\_]+:.*?## .*$$' $(MAKEFILE_LIST) | \
	grep -v "grep -E '###.*:.*?## .*$$'" | \
	sort -f | \
	awk 'BEGIN {FS = ":.*?## "}; \
	{gsub(/### /, "", $$1); \
	printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'; \
	echo ""; \
	echo "Příkazy:"; \
	grep -E '### [a-z\-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
	grep -v "grep -E '###.*:.*?## .*$$'" | \
	sort -f | \
	awk 'BEGIN {FS = ":.*?## "}; \
	{gsub(/### /, "", $$1); \
	printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'; \
	} | less -R

### clean: ## Odstraní všechny soubory vytvořené během kompilace (včetně dokumentace a archivu)
clean: clean-build clean-test clean-doc clean-coverage clean-pack

### test: ## Sestaví knihovny GoogleTest a GoogleMock a spustí test překladače
test: build-test
	$(TEST_BUILD_DIR)/$(EXECUTABLE)_test

### doc: ## Vygeneruje dokumentaci k projektu
doc:
	$(MAKE) $(SILENTOPT) install-doc-dep
	$(MAKE) $(SILENTOPT) clean-doc
	doxygen Doxyfile 
	cd $(DOC_OUT_DIR)/html && grep -v 'target="_self">resources\|target="_self">doc' files.html > temp.html && mv temp.html files.html

### coverage: ## Sestaví celý projekt s pokrytím kódu
coverage: 
	$(MAKE) $(SILENTOPT) install-cov-dep
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
	
### pack: ## Vytvoří ZIP archiv s projektem pro odevzdání
pack:
	$(MAKE) $(SILENTOPT) install-pack-dep
	$(MAKE) $(SILENTOPT) clean
	$(MAKE) $(SILENTOPT) clean-pack
	mkdir -p $(PACK_DIR)
	$(MAKE) pack-repo
	$(MAKE) pack-doc
	$(MAKE) pack-install
	cd $(PACK_DIR) && zip -r ../$(PACK_NAME) ./
	rm -rf $(PACK_DIR)

################################################################################
#                                                                              #
#                        SPECIALIZOVANÉ PŘÍKAZY 'CLEAN'                        #
#                                                                              #
################################################################################

### clean-build: ## Odstraní adresář src/build s verzí překladače k nasazení
clean-build:
	rm -rf $(BUILD_DIR)

### clean-build: ## Odstraní adresář test/build s unit testy překladače
clean-test:
	rm -rf $(TEST_BUILD_DIR)

### clean-doc: ## Odstraní všechny soubory z adresáře 'doc' (kromě ./resources/)
clean-doc:
	find $(DOC_OUT_DIR) -mindepth 1 ! -path '$(DOC_OUT_DIR)/resources*' -delete || true

### clean-coverage: ## Odstraní všechny soubory vytvořené během pokrytí kódu
clean-coverage:
	rm -rf $(COVERAGE_BUILD_DIR)

### clean-pack: ## Odstraní ZIP archiv s projektem pro odevzdání
clean-pack:
	rm -f ../$(PACK_NAME)


################################################################################
#                                                                              #
#                         KOMPILACE DÍLČÍCH SUBMODULŮ                          #
#                                                                              #
################################################################################

###                                                                          ###
#                         Podpůrné KNIHOVNY a SUBMODULY                        #
###                                                                          ###

ERROR_LIB_XKALINJ00 = error
STRING_LIB_XFARKAL00 = string
SYMTABLE_XKREJCD00 = symtable

### build-error: ## Sestaví knihovnu k řízení chybových stavů
build-$(ERROR_LIB_XKALINJ00): $(BUILD_DIR)/$(ERROR_LIB_XKALINJ00).o

### build-string: ## Sestaví knihovnu operací nad dynamickým stringem
build-$(STRING_LIB_XFARKAL00): $(BUILD_DIR)/$(STRING_LIB_XFARKAL00).o

### build-symtable: ## Sestaví knihovnu operací nad tabulkou symbolů typu TRP
build-$(SYMTABLE_XKREJCD00): $(BUILD_DIR)/$(SYMTABLE_XKREJCD00).o

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

### build-scanner: ## Sestaví lexikální analyzátor alias scanner
build-$(SCANNER_XHYZAPA00): $(BUILD_DIR)/$(SCANNER_XHYZAPA00).o

### build-parser: ## Sestaví lexikální analyzátor alias scanner
build-$(PARSER_TEAM): $(BUILD_DIR)/$(PARSER_TEAM).o

# Stavba objekotvých souborů modulů překladače
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

### build-test: ## Sestaví veškeré pro překladač napsané testy
build-test: $(TEST_SRC_OBJ_FILES) $(TEST_TEST_OBJ_FILES) $(TEST_BUILD_DIR)/gtest_main.a $(TEST_BUILD_DIR)/gtest.a
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS_TEST) $(CXXFLAGS_TEST) $(TEST_SRC_OBJ_FILES) $(TEST_TEST_OBJ_FILES) \
		$(TEST_BUILD_DIR)/gtest_main.a -o $(TEST_BUILD_DIR)/$(EXECUTABLE)_test

# Stavba objektových souborů s přepínači pro pokrytí kódu
$(TEST_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@


###                                                                          ###
#                        Kompilace GOOGLE TEST KNIHOVNY                        #
###                                                                          ###

### build-test-libs: ## Sestaví knihovny Google Test a Google Mock
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

### build-error-test: ## Sestaví testy knihovny pro řízení chybových stavů
build-$(ERROR_LIB_XKALINJ00)-test: $(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00)_test

### run-error-test: ## Spustí testy knihovny pro řízení chybových stavů
run-$(ERROR_LIB_XKALINJ00)-test: $(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00)_test
	$(TEST_BUILD_DIR)/$(ERROR_LIB_XKALINJ00)_test

### build-string-test: ## Sestaví testy knihovny operací nad dynamickým stringem
build-$(STRING_LIB_XFARKAL00)-test: $(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00)_test

### run-string-test: ## Spustí testy knihovny operací nad dynamickým stringem
run-$(STRING_LIB_XFARKAL00)-test: $(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00)_test
	$(TEST_BUILD_DIR)/$(STRING_LIB_XFARKAL00)_test

### build-symtable-test: ## Sestaví testy knihovny operací na tabulkou symbolů
build-$(SYMTABLE_XKREJCD00)-test: $(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00)_test

### run-symtable-test: ## Spustí testy knihovny operací na tabulkou symbolů
run-$(SYMTABLE_XKREJCD00)-test: $(TEST_BUILD_DIR)/$(SYMTABLE_XKREJCD00)_test
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

###                                                                          ###
#                TESTY lexikálního analyzátoru alias "scanneru"                #
###                                                                          ###

### build-scanner-test: ## Sestaví testy lexikálního analyzátoru alias scanneru
build-$(SCANNER_XHYZAPA00)-test: $(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00)_test

### run-scanner-test: ## Spustí testy lexikálního analyzátoru alias scanneru
run-$(SCANNER_XHYZAPA00)-test: $(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00)_test
	$(TEST_BUILD_DIR)/$(SCANNER_XHYZAPA00)_test

# Stavba testů podpůrných knihoven a submodulů
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


###                                                                          ###
#               TESTY syntaktického analyzátoru alias "parseru"                #
###                                                                          ###


###                                                                          ###
#                        TESTY sémantického analyzátoru                        #
###                                                                          ###


################################################################################
#                                                                              #
#                           GENEROVÁNÍ CODE COVERAGE                           #
#                                                                              #
################################################################################

###                                                                          ###
#                   ZPRACOVÁNÍ vygenerovaného CODE COVERAGE                    #
###                                                                          ###

### build-coverage: ## Sestaví všechny zdrojové soubory a testy s přepínači pro pokrytí kódu
build-coverage: $(COV_SRC_OBJ_FILES) $(COV_TEST_OBJ_FILES) $(COVERAGE_BUILD_DIR)/gtest_main.a $(COVERAGE_BUILD_DIR)/gtest.a
	@mkdir -p $(COVERAGE_BUILD_DIR)
	$(CXX) $(CFLAGS_COV) $(CXXFLAGS_COV) $(COV_SRC_OBJ_FILES) $(COV_TEST_OBJ_FILES) \
		$(COVERAGE_BUILD_DIR)/gtest_main.a -o $(COVERAGE_BUILD_DIR)/$(EXECUTABLE)_coverage

### run-test-coverage: ## Spustí všechny testy s pokrytím kódu
run-test-coverage:
	$(COVERAGE_BUILD_DIR)/$(EXECUTABLE)_coverage

### process-coverage: ## Zpracuje data o pokrytí kódu a vygeneruje HTML výstup
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

# Stavba objektových souborů Google Test knihoven s přepínačí pro code coverage
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

### pack-repo: ## Zkopíruje požadované složky repozitáře do složky pro odevzdání
pack-repo:
	mkdir -p $(PACK_DIR)/$(PACK_REPO_DIR)
	if [ -d "../.git" ]; then \
		rsync -av --progress ../.git $(PACK_REPO_DIR)/; \
	else \
		echo "../.git does not exist"; \
	fi
	if [ -d "../.github" ]; then \
		rsync -av --progress ../.github $(PACK_REPO_DIR)/; \
	else \
		echo "../.github does not exist"; \
	fi
	if [ -d "../.vscode" ]; then \
		rsync -av --progress ../.vscode $(PACK_REPO_DIR)/; \
	else \
		echo "../.vscode does not exist"; \
	fi
	if [ -d "../doc" ]; then \
		rsync -av --progress ../doc $(PACK_REPO_DIR)/; \
	else \
		echo "../doc does not exist"; \
	fi
	if [ -d "../mockup" ]; then \
		rsync -av --progress ../mockup $(PACK_REPO_DIR)/; \
	else \
		echo "../mockup does not exist"; \
	fi
	if [ -d "../plan" ]; then \
		rsync -av --progress ../plan $(PACK_REPO_DIR)/; \
	else \
		echo "../plan does not exist"; \
	fi
	if [ -d "../profiling" ]; then \
		rsync -av --progress ../profiling $(PACK_REPO_DIR)/; \
	else \
		echo "../profiling does not exist"; \
	fi
	if [ -d "../src" ]; then \
		rsync -av --progress ../src $(PACK_REPO_DIR)/ --exclude build; \
	else \
		echo "../src does not exist"; \
	fi
	if [ -f "../.editorconfig" ]; then \
		rsync -av --progress ../.editorconfig $(PACK_REPO_DIR)/; \
	else \
		echo "../.editorconfig does not exist"; \
	fi
	if [ -f "../.gitignore" ]; then \
		rsync -av --progress ../.gitignore $(PACK_REPO_DIR)/; \
	else \
		echo "../.gitignore does not exist"; \
	fi
	if [ -f "../LICENSE" ]; then \
		rsync -av --progress ../LICENSE $(PACK_REPO_DIR)/; \
	else \
		echo "../LICENSE does not exist"; \
	fi
	if [ -f "../README.md" ]; then \
		rsync -av --progress ../README.md $(PACK_REPO_DIR)/; \
	else \
		echo "../README.md does not exist"; \
	fi
	if [ -f "../screenshot.png" ]; then \
		rsync -av --progress ../screenshot.png $(PACK_REPO_DIR)/; \
	else \
		echo "../screenshot.png does not exist"; \
	fi
	if [ -f "../dokumentace.pdf" ]; then \
		rsync -av --progress ../dokumentace.pdf $(PACK_REPO_DIR)/; \
	else \
		echo "../dokumentace.pdf does not exist"; \
	fi
	if [ -d "../mockup" ]; then \
		rsync -av --progress ../mockup $(PACK_REPO_DIR)/; \
	else \
		echo "../mockup does not exist"; \
	fi
	if [ -d "../install" ]; then \
		rsync -av --progress ../install $(PACK_REPO_DIR)/; \
	else \
		echo "../install does not exist"; \
	fi
	if [ -f "../skutecnost.txt" ]; then \
		rsync -av --progress ../skutecnost.txt $(PACK_REPO_DIR)/; \
	else \
		echo "../skutecnost.txt does not exist"; \
	fi
	if [ -f "../hodnoceni.txt" ]; then \
		rsync -av --progress ../hodnoceni.txt $(PACK_REPO_DIR)/; \
	else \
		echo "../hodnoceni.txt does not exist"; \
	fi
	if [ -f "../debugging.png" ]; then \
		rsync -av --progress ../debugging.png $(PACK_REPO_DIR)/; \
	else \
		echo "../debugging.png does not exist"; \
	fi

### pack-doc: ## Vygeneruje dokumentaci pro účely odevzdání do složky pro odevzdání
pack-doc:
	$(MAKE) $(SILENTOPT) doc
	cp -r $(DOC_OUT_DIR)/ $(PACK_DIR)/
	@$(MAKE) $(SILENTOPT) clean-doc


################################################################################
#                                                                              #
#                    CÍLE PRO INSTALACI POTŘEBNÝCH NÁSTROJŮ                    #
#                                                                              #
################################################################################

### install-dev-dep: ## Nainstaluje závislosti potřebné pro kompilaci projektu
install-dev-dep: update-dep install-doc-dep install-pack-dep install-help-dep install-cov-dep

### install-help-dep: ## Nainstaluje závislosti potřebné pro výpis nápovědy k Makefile
install-help-dep:
	@dpkg -s less >/dev/null 2>&1 || (echo "Instaluji less" && sudo apt-get install less)

### install-cov-dep: ## Nainstaluje závislosti potřebné pro generování code coverage
install-cov-dep:
	@dpkg -s lcov >/dev/null 2>&1 || (echo "Instaluji lcov" && sudo apt-get install lcov)

### install-doc-dep: ## Nainstaluje závislosti potřebné pro generování dokumentace
install-doc-dep:
	@dpkg -s doxygen >/dev/null 2>&1 || (echo "Instaluji doxygen" && sudo apt-get install doxygen)

### install-pack-dep: ## Nainstaluje závislosti potřebné pro zabalení projektu
install-pack-dep:
	@dpkg -s rsync >/dev/null 2>&1 || (echo "Instaluji rsync" && sudo apt-get install rsync)
	@dpkg -s zip >/dev/null 2>&1 || (echo "Instaluji zip" && sudo apt-get install zip)

### dev-update-dep: ## Aktualizuje seznam dostupných balíčků
update-dep:
	sudo apt-get update -y