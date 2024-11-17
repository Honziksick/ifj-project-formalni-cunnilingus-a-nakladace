################################################################################
#                                                                              #
# Název projektu:   Implementace překladače imperativního jazyka IFJ24         #
#                                                                              #
# Soubor:           Makefile                                                   #
# Autor:            Jan Kalina   <xkalinj00>                                   #
#                                                                              #
# Datum:            29.09.2024                                                 #
# Poslední změna:   06.11.2024                                                 #
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
CXXFLAGS_TEST = $(STD_CXX) -O0 $(WARNING_FLAGS) $(DEBUG_FLAGS) $(SANITIZE_FLAGS)
CFLAGS_COV = $(STD_C) -O0 $(WARNING_FLAGS) $(DEBUG_FLAGS) $(COVERAGE_FLAGS)
CXXFLAGS_COV = $(STD_CXX) -O0 $(WARNING_FLAGS) $(DEBUG_FLAGS) $(COVERAGE_FLAGS) $(SANITIZE_FLAGS)


###                                                            ###
#  Proměnné obsahující seznamy zdrojových a objektových souborů  #
###                                                            ###

# Seznam všech zdrojových souborů překladače a testů
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

# Seznam všech objektových souborů přeložených pro testování
TEST_SRC_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(TEST_BUILD_DIR)/%.o,$(SRC_FILES))
TEST_TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.cpp,$(TEST_BUILD_DIR)/%.o,$(TEST_FILES))
TEST_C_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(TEST_BUILD_DIR)/%.o, $(SRC_FILES))

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
build: $(BUILD_DIR)/$(EXECUTABLE)
	mv $(BUILD_DIR)/$(EXECUTABLE) $(SRC_DIR)/$(EXECUTABLE)
	$(MAKE) clean-build

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

### MC # clean: # Příkaz 'clean' je namapován na příkaz 'clean-build' pro účely odevzdaní
clean: clean-build

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

### C # clean-all: # Odstraní všechny soubory vytvořené během kompilace (včetně dokumentace a archivu)
clean-all: clean-build clean-test clean-coverage clean-doc clean-pack

### C # clean-build: # Odstraní adresář 'src/build' s verzí překladače k nasazení
clean-build:
	rm -rf $(BUILD_DIR)

### C # clean-test: # Odstraní adresář '../test/build-test' s unit testy překladače
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
#                   KOMPILACE CELÉHO PŘEKLADAČE PRO NASAZENÍ                   #
#                                                                              #
################################################################################

# Pravidlo pro sestavení cílového programu
$(BUILD_DIR)/$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -o $@ $^

# Pravidlo pro sestavení objektových souborů
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@


################################################################################
#                                                                              #
#                 KOMPILACE CELÉHO PŘEKLADAČE PRO DEBUGGOVÁNÍ                  #
#                                                                              #
################################################################################

# Pravidlo pro sestavení cílového programu
$(TEST_BUILD_DIR)/$(EXECUTABLE): $(TEST_C_OBJ_FILES)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -o $@ $^

# Pravidlo pro sestavení objektových souborů
$(TEST_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@


################################################################################
#                                                                              #
#                         KOMPILACE DÍLČÍCH SUBMODULŮ                          #
#                                                                              #
################################################################################

###                                                                          ###
#                         Podpůrné KNIHOVNY a SUBMODULY                        #
###                                                                          ###

ERROR_LIB = error
DYNAMIC_STRING_LIB = dynamic_string
SYMTABLE = symtable

### B # build-error: # Sestaví knihovnu k řízení chybových stavů
build-error: $(BUILD_DIR)/$(ERROR_LIB).o

### B # build-string: # Sestaví knihovnu operací nad dynamickým stringem
build-string: $(BUILD_DIR)/$(DYNAMIC_STRING_LIB).o

# Stavba objekotvých souborů jednotlivých podpůrných submodulů a knihoven
$(BUILD_DIR)/$(ERROR_LIB).o: $(SRC_DIR)/$(ERROR_LIB).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(DYNAMIC_STRING_LIB).o: $(SRC_DIR)/$(DYNAMIC_STRING_LIB).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@


###                                                                          ###
#              SUBMODULY lexikálního analyzátoru alias "scanneru"              #
###                                                                          ###

SCANNER = scanner

### B # build-scanner: # Sestaví lexikální analyzátor alias 'scanner'
build-scanner: $(BUILD_DIR)/$(SCANNER).o

# Stavba objektových souborů jednotlivých modulů překladače
$(BUILD_DIR)/$(SCANNER).o: $(SRC_DIR)/$(SCANNER).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@


###                                                                          ###
#             SUBMODULY syntaktického analyzátoru alias "parseru"              #
###                                                                          ###

LLTABLE = lltable
PRECTABLE = precedence_table
FRAME_STACK = frame_stack
AST_LIB = ast_interface
PRECSTACK = precedence_stack
PARSER = parser

### B # build-parser: # Sestaví obecné funkce využívané ve všech submodulech parseru
build-parser: $(BUILD_DIR)/$(PARSER).o

### B # build-llparser: # Sestaví funkce pro LL syntaktický analyzátor
build-llparser: $(BUILD_DIR)/ll$(PARSER).o

### B # build-precparser: # Sestaví funkce pro precedenční syntaktický analyzátor
build-precparser: $(BUILD_DIR)/precedence_$(PARSER).o

### B # build-lltable: # Sestaví LL-tabulku využívanou LL parserem
build-lltable: $(BUILD_DIR)/$(LLTABLE).o

### B # build-prectable: # Sestaví precedenční tabulku využívanou precedenčním parserem
build-prectable: $(BUILD_DIR)/$(PRECTABLE).o

### B # build-precstack: # Sestaví precedenční zásobník využívaný precedenčním parserem
build-precstack: $(BUILD_DIR)/$(PRECSTACK).o

### B # build-frame-stack: # Sestaví knihovnu pro zásobník rámců využívný parserem
build-frame-stack: $(BUILD_DIR)/$(FRAME_STACK).o

### B # build-ast: # Sestaví knihovnu pro abstraktní syntaktický strom (AST)
build-ast: $(BUILD_DIR)/$(AST_LIB).o

### B # build-symtable: # Sestaví knihovnu operací nad tabulkou symbolů typu TRP-izp
build-symtable: $(BUILD_DIR)/$(SYMTABLE).o

# Stavba objektových souborů jednotlivých modulů parseru
$(BUILD_DIR)/$(PARSER).o: $(SRC_DIR)/$(PARSER).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/ll$(PARSER).o: $(SRC_DIR)/ll$(PARSER).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/precedence_$(PARSER).o: $(SRC_DIR)/precedence_$(PARSER).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(LLTABLE).o: $(SRC_DIR)/$(LLTABLE).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(PRECTABLE).o: $(SRC_DIR)/$(PRECTABLE).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(PRECSTACK).o: $(SRC_DIR)/$(PRECSTACK).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(FRAME_STACK).o: $(SRC_DIR)/$(FRAME_STACK).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(AST_LIB).o: $(SRC_DIR)/$(AST_LIB).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(SYMTABLE).o: $(SRC_DIR)/$(SYMTABLE).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@


###                                                                          ###
#                      SUBMODULY sémantického analyzátoru                      #
###                                                                          ###

SEMANTIC = semantic_analyser

### B # build-semantic: # Sestaví sémantický analyzátor
build-semantic: $(BUILD_DIR)/$(SEMANTIC).o


###                                                                          ###
#                 SUBMODULY generátoru tříadresného kódu (3AK)                 #
###                                                                          ###

TAC = tac_generator

### B # build-tac: # Sestaví generátor tříadresného kódu (3AK)
build-tac: $(BUILD_DIR)/$(TAC).o


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
build-error-test: $(TEST_BUILD_DIR)/$(ERROR_LIB)_test

### T # run-error-test: # Spustí test knihovny pro řízení chybových stavů
run-error-test: $(TEST_BUILD_DIR)/$(ERROR_LIB)_test
	$(TEST_BUILD_DIR)/$(ERROR_LIB)_test

### T # build-string-test: # Sestaví test knihovny operací nad dynamickým stringem
build-string-test: $(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test

### T # run-string-test: # Spustí test knihovny operací nad dynamickým stringem
run-string-test: $(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test
	$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test

# Stavba testů podpůrných knihoven a submodulů
$(TEST_BUILD_DIR)/$(ERROR_LIB)_test: $(TEST_BUILD_DIR)/$(ERROR_LIB)_test.o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(ERROR_LIB)_test.o: $(TEST_DIR)/$(ERROR_LIB)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(ERROR_LIB).o: $(SRC_DIR)/$(ERROR_LIB).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test: $(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test.o \
											$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test.o: $(TEST_DIR)/$(DYNAMIC_STRING_LIB)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o: $(SRC_DIR)/$(DYNAMIC_STRING_LIB).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@


###                                                                          ###
#           TESTY submodulů lexikálního analyzátoru alias "scanneru"            #
###                                                                          ###

### T # build-scanner-test: # Sestaví testy lexikálního analyzátoru alias 'scanneru'
build-scanner-test: $(TEST_BUILD_DIR)/$(SCANNER)_test

### T # run-scanner-test: # Spustí testy lexikálního analyzátoru alias 'scanneru'
run-scanner-test: $(TEST_BUILD_DIR)/$(SCANNER)_test
	$(TEST_BUILD_DIR)/$(SCANNER)_test

# Stavba testů pro lexikální analyzátor
$(TEST_BUILD_DIR)/$(SCANNER)_test: $(TEST_BUILD_DIR)/$(SCANNER)_test.o \
											$(TEST_BUILD_DIR)/$(SCANNER).o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/$(SYMTABLE).o \
											$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(SCANNER)_test.o: $(TEST_DIR)/$(SCANNER)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(SCANNER).o: $(SRC_DIR)/$(SCANNER).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@


###                                                                          ###
#          TESTY submodulů syntaktického analyzátoru alias "parseru"           #
###                                                                          ###

### T # build-parser-test: # Sestaví testy syntaktického analyzátoru alias 'parseru'
build-parser-test: $(TEST_BUILD_DIR)/$(PARSER)_test

### T # run-parser-test: # Spustí testy syntaktického analyzátoru alias 'parseru'
run-parser-test: $(TEST_BUILD_DIR)/$(PARSER)_test
	$(TEST_BUILD_DIR)/$(PARSER)_test

### T # build-lltable-test: # Sestaví testy aplikace pravidel LL-tabulky pro LL parser
build-lltable-test: $(TEST_BUILD_DIR)/$(LLTABLE)_test

### T # run-lltable-test: # Spustí testy aplikace pravidel LL-tabulky pro LL parser
run-lltable-test: $(TEST_BUILD_DIR)/$(LLTABLE)_test
	$(TEST_BUILD_DIR)/$(LLTABLE)_test

### T # build-prectable-test: # Sestaví testy aplikace pravidel precedenční tabulky pro precedenční parser
build-prectable-test: $(TEST_BUILD_DIR)/$(PRECTABLE)_test

### T # run-prectable-test: # Spustí testy aplikace pravidel precedenční tabulky pro precedenční parser
run-prectable-test: $(TEST_BUILD_DIR)/$(PRECTABLE)_test
	$(TEST_BUILD_DIR)/$(PRECTABLE)_test

### T # build-precstack-test: # Sestaví testy precedenční zásobníku pro precedenční parser
build-precstack-test: $(TEST_BUILD_DIR)/$(PRECSTACK)_test

### T # run-precstack-test: # Spustí testy precedenční zásobníku pro precedenční parser
run-precstack-test: $(TEST_BUILD_DIR)/$(PRECSTACK)_test
	$(TEST_BUILD_DIR)/$(PRECSTACK)_test

### T # build-frame-stack-test: # Sestaví testy zásobíku rámců
build-frame-stack-test: $(TEST_BUILD_DIR)/$(FRAME_STACK)_test

### T # run-frame-stack-test: # Spustí testy zásobníku rámců
run-frame-stack-test: $(TEST_BUILD_DIR)/$(FRAME_STACK)_test
	$(TEST_BUILD_DIR)/$(FRAME_STACK)_test

### T # build-ast-test: # Sestaví testy rozhraní pro uzly AST
build-ast-test: $(TEST_BUILD_DIR)/$(AST_LIB)_test

### T # run-ast-test: # Spustí testy rozhraní pro uzly AST
run-ast-test: $(TEST_BUILD_DIR)/$(AST_LIB)_test
	$(TEST_BUILD_DIR)/$(AST_LIB)_test

### T # build-symtable-test: # Sestaví testy knihovny operací nad tabulkou symbolů
build-symtable-test: $(TEST_BUILD_DIR)/$(SYMTABLE)_test

### T # run-symtable-test: # Spustí testy knihovny operací nad tabulkou symbolů
run-symtable-test: $(TEST_BUILD_DIR)/$(SYMTABLE)_test
	$(TEST_BUILD_DIR)/$(SYMTABLE)_test

# Stavba testů pro syntaktický analyzátor
#                                            $(TEST_BUILD_DIR)/ll$(PARSER).o
#											 $(TEST_BUILD_DIR)/precedence_$(PARSER).o
$(TEST_BUILD_DIR)/$(PARSER)_test: $(TEST_BUILD_DIR)/$(PARSER)_test.o \
											$(TEST_BUILD_DIR)/$(PARSER).o \
											$(TEST_BUILD_DIR)/$(LLTABLE).o \
											$(TEST_BUILD_DIR)/$(PRECTABLE).o \
											$(TEST_BUILD_DIR)/$(PRECSTACK).o \
											$(TEST_BUILD_DIR)/$(FRAME_STACK).o \
											$(TEST_BUILD_DIR)/$(AST_LIB).o \
											$(TEST_BUILD_DIR)/$(SYMTABLE).o \
											$(TEST_BUILD_DIR)/$(SCANNER).o \
											$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/ast_test_utils.o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(PARSER)_test.o: $(TEST_DIR)/$(PARSER)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(PARSER).o: $(SRC_DIR)/$(PARSER).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/ll$(PARSER).o: $(SRC_DIR)/ll$(PARSER).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/precedence_$(PARSER).o: $(SRC_DIR)/precedence_$(PARSER).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(LLTABLE)_test: $(TEST_BUILD_DIR)/$(LLTABLE)_test.o \
											$(TEST_BUILD_DIR)/$(LLTABLE).o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(LLTABLE)_test.o: $(TEST_DIR)/$(LLTABLE)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(LLTABLE).o: $(SRC_DIR)/$(LLTABLE).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(PRECTABLE)_test: $(TEST_BUILD_DIR)/$(PRECTABLE)_test.o \
											$(TEST_BUILD_DIR)/$(PRECTABLE).o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(PRECTABLE)_test.o: $(TEST_DIR)/$(PRECTABLE)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(PRECTABLE).o: $(SRC_DIR)/$(PRECTABLE).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(PRECSTACK)_test: $(TEST_BUILD_DIR)/$(PRECSTACK)_test.o \
											$(TEST_BUILD_DIR)/$(PRECSTACK).o \
											$(TEST_BUILD_DIR)/$(AST_LIB).o \
											$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/ast_test_utils.o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(PRECSTACK)_test.o: $(TEST_DIR)/$(PRECSTACK)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(PRECSTACK).o: $(SRC_DIR)/$(PRECSTACK).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(FRAME_STACK)_test: $(TEST_BUILD_DIR)/$(FRAME_STACK)_test.o \
											$(TEST_BUILD_DIR)/$(FRAME_STACK).o \
											$(TEST_BUILD_DIR)/$(SYMTABLE).o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(FRAME_STACK)_test.o: $(TEST_DIR)/$(FRAME_STACK)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(FRAME_STACK).o: $(SRC_DIR)/$(FRAME_STACK).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(AST_LIB)_test: $(TEST_BUILD_DIR)/$(AST_LIB)_test.o \
											$(TEST_BUILD_DIR)/$(AST_LIB).o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o \
											$(TEST_BUILD_DIR)/ast_test_utils.o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(AST_LIB)_test.o: $(TEST_DIR)/$(AST_LIB)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(AST_LIB).o: $(SRC_DIR)/$(AST_LIB).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/ast_test_utils.o: $(TEST_DIR)/ast_test_utils.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(SYMTABLE)_test: $(TEST_BUILD_DIR)/$(SYMTABLE)_test.o \
											$(TEST_BUILD_DIR)/$(SYMTABLE).o \
											$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(SYMTABLE)_test.o: $(TEST_DIR)/$(SYMTABLE)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(SYMTABLE).o: $(SRC_DIR)/$(SYMTABLE).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@


###                                                                          ###
#                   TESTY submodulů sémantického analyzátoru                   #
###                                                                          ###

### T # build-semantic-test: # Sestaví testy sémantického analyzátoru
build-semantic-test: $(TEST_BUILD_DIR)/$(SEMANTIC)_test

### T # run-semantic-test: # Spustí testy sémantického analyzátoru
run-semantic-test: $(TEST_BUILD_DIR)/$(SEMANTIC)_test
	$(TEST_BUILD_DIR)/$(SEMANTIC)_test

# Stavba testů pro sémantický analyzátor
$(TEST_BUILD_DIR)/$(SEMANTIC)_test: $(TEST_BUILD_DIR)/$(SEMANTIC)_test.o \
											$(TEST_BUILD_DIR)/$(SEMANTIC).o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o \
											$(TEST_BUILD_DIR)/$(FRAME_STACK).o \
											$(TEST_BUILD_DIR)/$(AST_LIB).o \
											$(TEST_BUILD_DIR)/$(SYMTABLE).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(SEMANTIC)_test.o: $(TEST_DIR)/$(SEMANTIC)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(SEMANTIC).o: $(SRC_DIR)/$(SEMANTIC).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@


###                                                                          ###
#                   TESTY generátoru tříadresného kódu (3AK)                   #
###                                                                          ###

### T # build-tac-test: # Sestaví testy generátoru tříadresného kódu (3AK)
build-tac-test: $(TEST_BUILD_DIR)/$(TAC)_test

### T # run-tac-test: # Spustí testy generátoru tříadresného kódu (3AK)
run-tac-test: $(TEST_BUILD_DIR)/$(TAC)_test
	$(TEST_BUILD_DIR)/$(TAC)_test

# Stavba testů pro generátor tříadresného kódu
$(TEST_BUILD_DIR)/$(TAC)_test: $(TEST_BUILD_DIR)/$(TAC)_test.o \
											$(TEST_BUILD_DIR)/$(TAC).o \
											$(TEST_BUILD_DIR)/$(ERROR_LIB).o \
											$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB).o \
											$(TEST_BUILD_DIR)/$(AST_LIB).o \
											$(TEST_BUILD_DIR)/gtest_main.a \
											$(TEST_BUILD_DIR)/gtest.a
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(TAC)_test.o: $(TEST_DIR)/$(TAC)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(TAC).o: $(SRC_DIR)/$(TAC).c
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
	@find $(COVERAGE_BUILD_DIR) -maxdepth 1 -type f -exec \
	mv -f {} $(COVERAGE_BUILD_DIR)/coverage_data/ \;
	@find $(COVERAGE_BUILD_DIR) -maxdepth 1 -type d ! -name \
	'coverage_data' ! -path '$(COVERAGE_BUILD_DIR)' -exec mv {} \
	$(COVERAGE_BUILD_DIR)/coverage_data/ \;
	@echo '<html><head><meta http-equiv="refresh" content="0; \
	url=coverage_data/index.html"></head></html>' > $(COVERAGE_BUILD_DIR)/code_coverage_web_result.html


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
			rsync -a --include '*/' --include '*.c' --exclude '*' --exclude '*/' \
			--prune-empty-dirs $(SRC_DIR)/ $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			echo "\033[0;31m\nError: Adresář "$(SRC_DIR)" neexistuje.\033[0m"; \
		fi; \
		if [ -d "$(SRC_DIR)" ]; then \
			rsync -a --include '*/' --include '*.h' --exclude '*' --exclude '*/' \
			--prune-empty-dirs $(SRC_DIR)/ $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			echo "\033[0;31m\nError: Adresář "$(SRC_DIR)" neexistuje.\033[0m"; \
		fi; \
		if [ -f "$(SRC_DIR)/Makefile" ]; then \
			rsync -a $(SRC_DIR)/Makefile $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -d "$(TEST_DIR)" ]; then \
			rsync -a --include '*/' --include '*.cpp' --exclude '*' --exclude '*/' \
			--prune-empty-dirs $(TEST_DIR)/ $(PACK_DIR)/$(ARCHIVE_DIR)/; \
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
