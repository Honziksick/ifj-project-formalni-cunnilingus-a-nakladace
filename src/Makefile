################################################################################
#                                                                              #
# Název projektu:   Implementace překladače imperativního jazyka IFJ24         #
#                                                                              #
# Soubor:           Makefile                                                   #
# Autor:            Jan Kalina   <xkalinj00>                                   #
#                                                                              #
# Datum:            29.09.2024                                                 #
# Poslední změna:   04.12.2024                                                 #
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

###                                     ###
#  Základní konfigurace souboru Makefile  #
###                                     ###

# Název projektu
EXECUTABLE = ifj24_compiler

# Název ZIP archivu pro odevzdání projektu
PACK_NAME = xkalinj00

# ANSI sekvence pro barvy
COLOR_RESET = \033[0m
COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_BLUE = \033[0;36m
COLOR_YELLOW = \033[0;33m
COLOR_MAGENTA = \033[0;35m

###                                       ###
#  Přepínače pro spouštění příkazu $(MAKE)  #
###                                       ###

# Spuštění 'make' v tichém režímu (bez výpisu událostí)
$(VERBOSE)SILENTOPT = -s

# Definice konstanty pro zakázání vybraných cílů (kvůli odevzdání)
#DISABLE_TARGETS ?= true

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
EXCLUDED_TEST_SRC_FILES = $(SRC_DIR)/ifj24_compiler.c
TEST_SRC_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(TEST_BUILD_DIR)/%.o,$(TEST_INCLUDES)) \
$(TEST_BUILD_DIR)/ifj24_compiler_test_utils.o $(TEST_BUILD_DIR)/gtest_main.a $(TEST_BUILD_DIR)/gtest.a

# Vyčlenění souborů pro testování
TEST_INCLUDES = $(filter-out $(EXCLUDED_TEST_SRC_FILES), $(SRC_FILES))

# Seznam všech objektových souborů přeložených pro code coverage
COV_SRC_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(COVERAGE_BUILD_DIR)/%.o,$(SRC_FILES))

# Vyčlenění souborů z coverage
COV_SRC_OBJ_FILES := $(filter-out $(COVERAGE_BUILD_DIR)/ifj24_compiler.o, $(COV_SRC_OBJ_FILES))


################################################################################
#                                                                              #
#                          ZÁKLADNÍ PŘÍKAZY MAKEFILE                           #
#                                                                              #
################################################################################

# Příkaz '.PHONY' určuje, že následující příkazy nejsou nikdy brány jako soubory
.PHONY: all build help clean test doc coverage pack clean-all clean-build \
        clean-test clean-test-bin clean-doc clean-coverage clean-pack \
		build-error build-string build-scanner build-parser build-llparser \
		build-precparser build-lltable build-prectable build-precstack \
		build-frame-stack build-ast build-symtable build-semantic build-tac \
		build-built-in build-test build-test-libs build-error-test \
		run-error-test build-string-test run-string-test build-scanner-test \
		run-scanner-test build-parser-test run-parser-test build-lltable-test \
		run-lltable-test build-frame-stack-test run-frame-stack-test \
		build-ast-test run-ast-test build-symtable-test run-symtable-test \
		build-semantic-test run-semantic-test build-tac-test run-tac-test \
		run-script run-code gen-code gen-run-code build-coverage \
		run-test-coverage process-coverage pack-prepare install-dev-dep \
		install-help-dep install-cov-dep install-doc-dep install-pack-dep \
		update-dep

### MC # all: # Provede sestavení celého překladače určeného k nasazení
all: build

### MC # build: # Sestaví překladač týmu "Tým xkalinj00"
build: $(BUILD_DIR)/$(EXECUTABLE)
	mv $(BUILD_DIR)/$(EXECUTABLE) $(SRC_DIR)/$(EXECUTABLE)
	rm -rf $(BUILD_DIR)

# Definice zkratek pro kategorie příkazů
CATEGORIES := MC C B G T CC DEV O

### MC # help: # Vypíše nápovědu k použití Makefile
help:
ifndef DISABLE_TARGETS
	@$(MAKE) $(SILENTOPT) install-help-dep
endif
	@{ \
	for CATEGORY in $(CATEGORIES); do \
		case $$CATEGORY in \
		"MC") FULL_CAT="Main Commands";; \
		"C") FULL_CAT="Clean";; \
		"B") FULL_CAT="Build Modules and Submodules";; \
		"G") FULL_CAT="Generate and interpret IFJ24code";; \
		"T") FULL_CAT="Test";; \
		"CC") FULL_CAT="Code Coverage";; \
		"DEV") FULL_CAT="Install Dependencies";; \
		"O") FULL_CAT="Others";; \
		esac; \
		echo "$(COLOR_YELLOW)$$FULL_CAT:$(COLOR_RESET)"; \
		grep -E "^### $$CATEGORY # [a-zA-Z0-9_\-]+:.*?# .*$$" $(MAKEFILE_LIST) | \
		sort -f | \
		awk 'BEGIN {FS = ":.*?# "}; \
		{ \
			gsub(/^### [A-Z]+ # /, "", $$1); \
			split($$2, lines, "\\\\n"); \
			printf "$(COLOR_BLUE)%-30s$(COLOR_RESET) %s\n", $$1, lines[1]; \
			for (i = 2; i <= length(lines); i++) { \
				printf "$(COLOR_BLUE)%-30s$(COLOR_RESET) %s\n", "", lines[i]; \
			} \
		}'; \
		echo ""; \
	done; \
	} | less -R

### MC # clean: # Příkaz 'clean' odstraní adresář './build' s objektovými soubory
ifndef DISABLE_TARGETS
clean: clean-all
else
clean: clean-build clean-doc
endif

### MC # test: # Sestaví knihovny GoogleTest a GoogleMock a spustí test překladače
ifndef DISABLE_TARGETS
test: build-test
	$(TEST_BUILD_DIR)/$(INTEGRATE)
else
test:
	@echo "$(COLOR_RED)Cíl 'test' je ve verzi projektu pro odevzdání zakázán.$(COLOR_RESET)"
endif

### MC # doc: # Vygeneruje dokumentaci k projektu do adresáře `../doc/`
ifndef DISABLE_TARGETS
doc:
	@$(MAKE) $(SILENTOPT) install-doc-dep
	$(MAKE) $(SILENTOPT) clean-doc
	doxygen Doxyfile
	cd $(DOC_DIR)/html && grep -v 'target="_self">resources\|target="_self">doc' files.html > temp.html && mv temp.html files.html
	@echo '<html><head><meta http-equiv="refresh" content="0; url=html/index.html"></head></html>' > $(DOC_DIR)/documentation.html
	@echo -e "$(COLOR_YELLOW)Chcete otevřít HTML dokumentaci v hlavním systémovém prohlížeči? (y/n): $(COLOR_RESET)"
	@bash -c 'read -t 5 -p "" choice; \
	if [ "$$choice" = "y" ]; then \
		if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null; then \
			cmd.exe /C start $(DOC_DIR)/documentation.html; \
		else \
			xdg-open $(DOC_DIR)/documentation.html; \
		fi \
	fi'
else
doc:
	$(MAKE) $(SILENTOPT) clean-doc
	doxygen Doxyfile
	@echo '<html><head><meta http-equiv="refresh" content="0; url=./html/index.html"></head></html>' > ./doc/documentation.html
endif

### MC # coverage: # Provede analýzu pokrytí kódu a vygeneruje HTML přehled
ifndef DISABLE_TARGETS
coverage:
	@$(MAKE) $(SILENTOPT) install-cov-dep
	$(MAKE) build-coverage
	$(MAKE) run-test-coverage
	$(MAKE) process-coverage
	@echo -e "$(COLOR_YELLOW)Chcete otevřít soubor HTML v hlavním systémovém prohlížeči? (y/n): $(COLOR_RESET)"
	@bash -c 'read -t 5 -p "" choice; \
	if [ "$$choice" = "y" ]; then \
		if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null; then \
			cmd.exe /C start $(COVERAGE_BUILD_DIR)/code_coverage_web_result.html; \
		else \
			xdg-open $(COVERAGE_BUILD_DIR)/code_coverage_web_result.html; \
		fi \
	fi'
else
coverage:
	@echo "$(COLOR_RED)Cíl 'coverage' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### MC # pack: # Vytvoří ZIP archiv se soubory určenými k odevzdání
ifndef DISABLE_TARGETS
pack:
	@$(MAKE) $(SILENTOPT) install-pack-dep
	$(MAKE) $(SILENTOPT) clean-pack
	mkdir -p $(PACK_DIR)
	$(MAKE) $(SILENTOPT) pack-prepare
	@echo ""
	cd $(PACK_DIR)/$(ARCHIVE_DIR) && zip -qr ../$(PACK_NAME) ./
	@{ \
		cd "$(PACK_DIR)" && \
			mkdir is_it_ok && \
			cp $(PACK_NAME).zip is_it_ok/ && \
			cp $(TEST_DIR)/is_it_ok.sh is_it_ok/ && \
			./is_it_ok/is_it_ok.sh $(PACK_NAME).zip testdir && \
			rm -rf is_it_ok; \
	}
else
pack:
	@echo "$(COLOR_RED)Cíl 'pack' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

################################################################################
#                                                                              #
#                        SPECIALIZOVANÉ PŘÍKAZY 'CLEAN'                        #
#                                                                              #
################################################################################

### C # clean-all: # Odstraní všechny soubory vytvořené během kompilace (včetně dokumentace a archivu)
ifndef DISABLE_TARGETS
clean-all: clean-build clean-test clean-coverage clean-doc clean-pack
else
clean-all:
	@echo "$(COLOR_RED)Cíl 'clean-all' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### C # clean-build: # Odstraní adresář 'src/build' s verzí překladače k nasazení
clean-build:
	rm -rf $(BUILD_DIR)
	rm -rf $(EXECUTABLE)

### C # clean-test: # Odstraní adresář '../test/build-test' s unit testy překladače
ifndef DISABLE_TARGETS
clean-test:
	rm -rf $(TEST_BUILD_DIR)
else
clean-test:
	@echo "$(COLOR_RED)Cíl 'clean-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### C # clean-test-bin: # Odstraní adresář obsah '../test/build-test' bez GoogleTest knihoven
ifndef DISABLE_TARGETS
clean-test-bin:
	find $(TEST_BUILD_DIR) -type f ! -name 'gtest.a' ! -name 'gtest_main.a' ! -name 'gtest_main.o' ! -name 'gtest-all.o' -delete
	find $(TEST_BUILD_DIR) -type d -empty -delete
else
clean-test-bin:
	@echo "$(COLOR_RED)Cíl 'clean-test-bin:' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### C # clean-doc: # Odstraní veškerý obsah adresáře '../doc' (kromě '../doc/resources/')
ifndef DISABLE_TARGETS
clean-doc:
	find $(DOC_DIR) -mindepth 1 ! -path '$(DOC_DIR)/resources*' -delete || true
else
clean-doc:
	rm -rf ./doc
endif

### C # clean-coverage: # Odstraní adresář '../test/build-coverage' s pokrytím kódu
ifndef DISABLE_TARGETS
clean-coverage:
	rm -rf $(COVERAGE_BUILD_DIR)
else
clean-coverage:
	@echo "$(COLOR_RED)Cíl 'clean-coverage' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### C # clean-pack: # Odstraní složku '../pack' pro zabalení projektu (včetně archivu)
ifndef DISABLE_TARGETS
clean-pack:
	rm -rf $(PACK_DIR)
else
clean-pack:
	@echo "$(COLOR_RED)Cíl 'clean-pack' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif


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

### B # build-parser-common: # Sestaví obecné funkce využívané ve všech submodulech parseru
build-parser-common: $(BUILD_DIR)/$(PARSER)_common.o

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
$(BUILD_DIR)/$(PARSER)_common.o: $(SRC_DIR)/$(PARSER)_common.c
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

$(BUILD_DIR)/$(SEMANTIC).o: $(SRC_DIR)/$(SEMANTIC).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@


###                                                                          ###
#                 SUBMODULY generátoru tříadresného kódu (3AK)                 #
###                                                                          ###

TAC = tac_generator
BUILT_IN = built_in_functions

### B # build-tac: # Sestaví generátor tříadresného kódu (3AK)
build-tac: $(BUILD_DIR)/$(TAC).o

### B # build-built-in: # Sestaví podpůrné funkce pro generování built-in ("ifj") funkcí
build-built-in: $(BUILD_DIR)/$(BUILT_IN).o

$(BUILD_DIR)/$(TAC).o: $(SRC_DIR)/$(TAC).c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_STD) -c $< -o $@

$(BUILD_DIR)/$(BUILT_IN).o: $(SRC_DIR)/$(BUILT_IN).c
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

INTEGRATE=ifj24_compiler_integration_test

### T # build-test: # Sestaví testovací program sdružující veškeré testy překladače
ifndef DISABLE_TARGETS
build-test: $(TEST_BUILD_DIR)/$(INTEGRATE)
else
build-test:
	@echo "$(COLOR_RED)Cíl 'build-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

# Stavba objektových souborů modulů a submodulů s přepínači pro pokrytí kódu
$(TEST_BUILD_DIR)/$(INTEGRATE): $(TEST_BUILD_DIR)/$(INTEGRATE).o $(TEST_SRC_OBJ_FILES)
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(INTEGRATE).o: $(TEST_DIR)/$(INTEGRATE).cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(EXECUTABLE).o: $(SRC_DIR)/$(EXECUTABLE).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@


###                                                                          ###
#                        Kompilace GOOGLE TEST KNIHOVNY                        #
###                                                                          ###

### T # build-test-libs: # Sestaví knihovny Google Test a Google Mock
ifndef DISABLE_TARGETS
build-test-libs: $(TEST_BUILD_DIR)/gtest_main.a $(TEST_BUILD_DIR)/gtest.a
else
build-test-libs:
	@echo "$(COLOR_RED)Cíl 'build-test-libs' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

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
ifndef DISABLE_TARGETS
build-error-test: $(TEST_BUILD_DIR)/$(ERROR_LIB)_test
else
build-error-test:
	@echo "$(COLOR_RED)Cíl 'build-error-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-error-test: # Spustí test knihovny pro řízení chybových stavů
ifndef DISABLE_TARGETS
run-error-test: $(TEST_BUILD_DIR)/$(ERROR_LIB)_test
	$(TEST_BUILD_DIR)/$(ERROR_LIB)_test
else
run-error-test:
	@echo "$(COLOR_RED)Cíl 'run-error-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # build-string-test: # Sestaví test knihovny operací nad dynamickým stringem
ifndef DISABLE_TARGETS
build-string-test: $(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test
else
build-string-test:
	@echo "$(COLOR_RED)Cíl 'build-string-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-string-test: # Spustí test knihovny operací nad dynamickým stringem
ifndef DISABLE_TARGETS
run-string-test: $(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test
	$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test
else
run-string-test:
	@echo "$(COLOR_RED)Cíl 'run-string-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

# Stavba testů podpůrných knihoven a submodulů
$(TEST_BUILD_DIR)/$(ERROR_LIB)_test: $(TEST_BUILD_DIR)/$(ERROR_LIB)_test.o \
									 $(TEST_SRC_OBJ_FILES)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(ERROR_LIB)_test.o: $(TEST_DIR)/$(ERROR_LIB)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(ERROR_LIB).o: $(SRC_DIR)/$(ERROR_LIB).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test: $(TEST_BUILD_DIR)/$(DYNAMIC_STRING_LIB)_test.o \
											  $(TEST_SRC_OBJ_FILES)
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
ifndef DISABLE_TARGETS
build-scanner-test: $(TEST_BUILD_DIR)/$(SCANNER)_test
else
build-scanner-test:
	@echo "$(COLOR_RED)Cíl 'build-scanner-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-scanner-test: # Spustí testy lexikálního analyzátoru alias 'scanneru'
ifndef DISABLE_TARGETS
run-scanner-test: $(TEST_BUILD_DIR)/$(SCANNER)_test
	$(TEST_BUILD_DIR)/$(SCANNER)_test
else
run-scanner-test:
	@echo "$(COLOR_RED)Cíl 'run-scanner-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

# Stavba testů pro lexikální analyzátor
$(TEST_BUILD_DIR)/$(SCANNER)_test: $(TEST_BUILD_DIR)/$(SCANNER)_test.o \
								   $(TEST_SRC_OBJ_FILES)
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
ifndef DISABLE_TARGETS
build-parser-test: $(TEST_BUILD_DIR)/$(PARSER)_test
else
build-parser-test:
	@echo "$(COLOR_RED)Cíl 'build-parser-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-parser-test: # Spustí testy syntaktického analyzátoru alias 'parseru'
ifndef DISABLE_TARGETS
run-parser-test: $(TEST_BUILD_DIR)/$(PARSER)_test
	$(TEST_BUILD_DIR)/$(PARSER)_test
else
run-parser-test:
	@echo "$(COLOR_RED)Cíl 'run-parser-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # build-lltable-test: # Sestaví testy aplikace pravidel LL-tabulky pro LL parser
ifndef DISABLE_TARGETS
build-lltable-test: $(TEST_BUILD_DIR)/$(LLTABLE)_test
else
build-lltable-test:
	@echo "$(COLOR_RED)Cíl 'build-lltable-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-lltable-test: # Spustí testy aplikace pravidel LL-tabulky pro LL parser
ifndef DISABLE_TARGETS
run-lltable-test: $(TEST_BUILD_DIR)/$(LLTABLE)_test
	$(TEST_BUILD_DIR)/$(LLTABLE)_test
else
run-lltable-test:
	@echo "$(COLOR_RED)Cíl 'run-lltable-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # build-frame-stack-test: # Sestaví testy zásobíku
ifndef DISABLE_TARGETS
build-frame-stack-test: $(TEST_BUILD_DIR)/$(FRAME_STACK)_test
else
build-frame-stack-test:
	@echo "$(COLOR_RED)Cíl 'build-frame-stack-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-frame-stack-test: # Spustí testy zásobníku rámců
ifndef DISABLE_TARGETS
run-frame-stack-test: $(TEST_BUILD_DIR)/$(FRAME_STACK)_test
	$(TEST_BUILD_DIR)/$(FRAME_STACK)_test
else
run-frame-stack-test:
	@echo "$(COLOR_RED)Cíl 'run-frame-stack-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # build-ast-test: # Sestaví testy rozhraní pro uzly AST
ifndef DISABLE_TARGETS
build-ast-test: $(TEST_BUILD_DIR)/$(AST_LIB)_test
else
build-ast-test:
	@echo "$(COLOR_RED)Cíl 'build-ast-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-ast-test: # Spustí testy rozhraní pro uzly AST
ifndef DISABLE_TARGETS
run-ast-test: $(TEST_BUILD_DIR)/$(AST_LIB)_test
	$(TEST_BUILD_DIR)/$(AST_LIB)_test
else
run-ast-test:
	@echo "$(COLOR_RED)Cíl 'run-ast-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # build-symtable-test: # Sestaví testy knihovny operací nad tabulkou symbolů
ifndef DISABLE_TARGETS
build-symtable-test: $(TEST_BUILD_DIR)/$(SYMTABLE)_test
else
build-symtable-test:
	@echo "$(COLOR_RED)Cíl 'build-symtable-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-symtable-test: # Spustí testy knihovny operací nad tabulkou symbolů
ifndef DISABLE_TARGETS
run-symtable-test: $(TEST_BUILD_DIR)/$(SYMTABLE)_test
	$(TEST_BUILD_DIR)/$(SYMTABLE)_test
else
run-symtable-test:
	@echo "$(COLOR_RED)Cíl 'run-symtable-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

# Stavba testů pro syntaktický analyzátor
$(TEST_BUILD_DIR)/$(PARSER)_test: $(TEST_BUILD_DIR)/$(PARSER)_test.o \
								  $(TEST_SRC_OBJ_FILES)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(PARSER)_test.o: $(TEST_DIR)/$(PARSER)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(PARSER)_common.o: $(SRC_DIR)/$(PARSER)_common.c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/ll$(PARSER).o: $(SRC_DIR)/ll$(PARSER).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/precedence_$(PARSER).o: $(SRC_DIR)/precedence_$(PARSER).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(LLTABLE)_test: $(TEST_BUILD_DIR)/$(LLTABLE)_test.o \
								   $(TEST_SRC_OBJ_FILES)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(LLTABLE)_test.o: $(TEST_DIR)/$(LLTABLE)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(LLTABLE).o: $(SRC_DIR)/$(LLTABLE).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(FRAME_STACK)_test: $(TEST_BUILD_DIR)/$(FRAME_STACK)_test.o \
								       $(TEST_SRC_OBJ_FILES)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(FRAME_STACK)_test.o: $(TEST_DIR)/$(FRAME_STACK)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(FRAME_STACK).o: $(SRC_DIR)/$(FRAME_STACK).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(AST_LIB)_test: $(TEST_BUILD_DIR)/$(AST_LIB)_test.o \
								   $(TEST_SRC_OBJ_FILES)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(AST_LIB)_test.o: $(TEST_DIR)/$(AST_LIB)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(AST_LIB).o: $(SRC_DIR)/$(AST_LIB).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/ifj24_compiler_test_utils.o: $(TEST_DIR)/ifj24_compiler_test_utils.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(SYMTABLE)_test: $(TEST_BUILD_DIR)/$(SYMTABLE)_test.o \
									$(TEST_SRC_OBJ_FILES)
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
ifndef DISABLE_TARGETS
build-semantic-test: $(TEST_BUILD_DIR)/$(SEMANTIC)_test
else
build-semantic-test:
	@echo "$(COLOR_RED)Cíl 'build-semantic-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-semantic-test: # Spustí testy sémantického analyzátoru
ifndef DISABLE_TARGETS
run-semantic-test: $(TEST_BUILD_DIR)/$(SEMANTIC)_test
	$(TEST_BUILD_DIR)/$(SEMANTIC)_test
else
run-semantic-test:
	@echo "$(COLOR_RED)Cíl 'run-semantic-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

# Stavba testů pro sémantický analyzátor
$(TEST_BUILD_DIR)/$(SEMANTIC)_test: $(TEST_BUILD_DIR)/$(SEMANTIC)_test.o \
									$(TEST_SRC_OBJ_FILES)
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
ifndef DISABLE_TARGETS
build-tac-test: $(TEST_BUILD_DIR)/$(TAC)_test
else
build-tac-test:
	@echo "$(COLOR_RED)Cíl 'build-tac-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### T # run-tac-test: # Spustí testy generátoru tříadresného kódu (3AK)
ifndef DISABLE_TARGETS
run-tac-test: $(TEST_BUILD_DIR)/$(TAC)_test
	$(TEST_BUILD_DIR)/$(TAC)_test
else
run-tac-test:
	@echo "$(COLOR_RED)Cíl 'run-tac-test' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

# Stavba testů pro generátor tříadresného kódu
$(TEST_BUILD_DIR)/$(TAC)_test: $(TEST_BUILD_DIR)/$(TAC)_test.o \
							   $(TEST_SRC_OBJ_FILES)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) $^ -o $@

$(TEST_BUILD_DIR)/$(TAC)_test.o: $(TEST_DIR)/$(TAC)_test.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(TAC).o: $(SRC_DIR)/$(TAC).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

$(TEST_BUILD_DIR)/$(BUILT_IN).o: $(SRC_DIR)/$(BUILT_IN).c
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) -c $< -o $@

################################################################################
#                                                                              #
#                         TESTOVÁNÍ MEZIKÓDU IFJ24CODE                         #
#                                                                              #
################################################################################

# Základní cesta k souborům
BASE_PATH = ../test/test_examples/ifj24code_examples

### G # run-script: Spustí All-In-One testovací skript na kompilátor.
ifndef DISABLE_TARGETS
run-script: build
	$(TEST_DIR)/test.sh
else
run-script:
	@echo "$(COLOR_RED)Cíl 'run-script' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### G # run-code: # Spustí interpret jazyk IFJ24 s IFJ24code assembler souborem: \nUSAGE: "make run-code F=<FILE>" \n(očekává soubor bez přípony '.zig' a ve složce: \n'../test/test_examples/ifj24code_examples/pregenerated_ifj24code/<FILE>.zig')
ifndef DISABLE_TARGETS
run-code:
	@echo "$(COLOR_MAGENTA)Launching 'ic24int' interpreter with:$(COLOR_RESET) $(COLOR_YELLOW)$(F).ifj24code$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)RUNNING:$(COLOR_RESET) $(COLOR_YELLOW)$(BASE_PATH)/out/$(F).ifj24code$(COLOR_RESET)"
	@$(TEST_DIR)/ic24int $(BASE_PATH)/out/$(F).ifj24code
	@echo "\n$(COLOR_MAGENTA)INTERPRETATION DONE$(COLOR_RESET)"
else
run-code:
	@echo "$(COLOR_RED)Cíl 'run-code' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### G # gen-code: # Spustí týmový překladač jazyka IFJ24 a vygeneruje pomocí něj IFJ24code \npro zvolený program: \nUSAGE: "make gen-code F=<FILE>" \n(očekává soubor bez přípony '.zig' a ve složce: \n'../test/test_examples/ifj24code_examples/programs/<FILE>.zig')
ifndef DISABLE_TARGETS
gen-code: $(BUILD_DIR)/$(EXECUTABLE)
	@mkdir -p $(BASE_PATH)/out
	@echo "$(COLOR_MAGENTA)Launching 'ifj24compiler' with:$(COLOR_RESET) $(COLOR_YELLOW)$(F).zig$(COLOR_RESET)"
	@$(BUILD_DIR)/$(EXECUTABLE) < $(BASE_PATH)/in/$(F).zig > $(BASE_PATH)/out/$(F).ifj24code
	@echo "$(COLOR_MAGENTA)COMPILATION DONE:$(COLOR_RESET) $(COLOR_YELLOW)$(BASE_PATH)/out/$(F).ifj24code$(COLOR_RESET)"
else
gen-code:
	@echo "$(COLOR_RED)Cíl 'gen-code' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### G # gen-run-code: # Spustí týmový překladač jazyka IFJ24 a vygeneruje pomocí něj IFJ24code \npro zvolený program. Následně spustí vygenerovaný IFJ24code soubor: \nUSAGE: "make gen-run-code F=<FILE>" \n(očekává soubor bez přípony '.zig' a ve složce: \n'../test/test_examples/ifj24code_examples/programs/<FILE>.zig')
ifndef DISABLE_TARGETS
gen-run-code: $(BUILD_DIR)/$(EXECUTABLE)
	@mkdir -p $(BASE_PATH)/out
	@echo "$(COLOR_MAGENTA)Launching 'ifj24compiler' with:$(COLOR_RESET) $(COLOR_YELLOW)$(F).zig$(COLOR_RESET)"
	@$(BUILD_DIR)/$(EXECUTABLE) < $(BASE_PATH)/in/$(F).zig > $(BASE_PATH)/out/$(F).ifj24code
	@echo "$(COLOR_MAGENTA)COMPILATION DONE:$(COLOR_RESET) $(COLOR_YELLOW)$(BASE_PATH)/out/$(F).ifj24code$(COLOR_RESET)\n"
	@echo "$(COLOR_MAGENTA)Launching 'ic24int' interpreter with:$(COLOR_RESET) $(COLOR_YELLOW)$(F).ifj24code$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)RUNNING:$(COLOR_RESET) $(COLOR_YELLOW)$(BASE_PATH)/out/$(F).ifj24code$(COLOR_RESET)"
	@$(TEST_DIR)/ic24int $(BASE_PATH)/out/$(F).ifj24code
	@echo "\n$(COLOR_MAGENTA)INTERPRETATION DONE$(COLOR_RESET)"
else
gen-run-code:
	@echo "$(COLOR_RED)Cíl 'gen-run-code' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif


################################################################################
#                                                                              #
#                           GENEROVÁNÍ CODE COVERAGE                           #
#                                                                              #
################################################################################

###                                                                          ###
#                   ZPRACOVÁNÍ vygenerovaného CODE COVERAGE                    #
###                                                                          ###

### CC # build-coverage: # Sestaví všechny zdrojové soubory a testy s přepínači pro pokrytí kódu
ifndef DISABLE_TARGETS
build-coverage: $(COV_SRC_OBJ_FILES) $(COVERAGE_BUILD_DIR)/$(INTEGRATE).o $(TEST_BUILD_DIR)/ifj24_compiler_test_utils.o $(COVERAGE_BUILD_DIR)/gtest_main.a $(COVERAGE_BUILD_DIR)/gtest.a
	@mkdir -p $(COVERAGE_BUILD_DIR)
	$(CXX) $(CFLAGS_COV) $(CXXFLAGS_COV) $(COV_SRC_OBJ_FILES) $(COVERAGE_BUILD_DIR)/$(INTEGRATE).o $(TEST_BUILD_DIR)/ifj24_compiler_test_utils.o \
		$(COVERAGE_BUILD_DIR)/gtest_main.a -o $(COVERAGE_BUILD_DIR)/$(EXECUTABLE)_coverage
else
build-coverage:
	@echo "$(COLOR_RED)Cíl 'build-coverage' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### CC # run-test-coverage: # Spustí všechny testy generující pokrytí kódu
ifndef DISABLE_TARGETS
run-test-coverage:
	$(COVERAGE_BUILD_DIR)/$(EXECUTABLE)_coverage
else
run-test-coverage:
	@echo "$(COLOR_RED)Cíl 'run-test-coverage' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### CC # process-coverage: # Zpracuje data o pokrytí kódu a vygeneruje HTML výstup
ifndef DISABLE_TARGETS
process-coverage:
	lcov --quiet --directory $(COVERAGE_BUILD_DIR) --capture \
	--output-file $(COVERAGE_BUILD_DIR)/coverage.info
	@lcov --quiet --remove $(COVERAGE_BUILD_DIR)/coverage.info '/usr/*' \
	--output-file $(COVERAGE_BUILD_DIR)/filtered_coverage.info
	@lcov --quiet --remove $(COVERAGE_BUILD_DIR)/filtered_coverage.info \
	'*/googletest-src/*' --output-file $(COVERAGE_BUILD_DIR)/filtered_coverage.info
	@lcov --quiet --remove $(COVERAGE_BUILD_DIR)/filtered_coverage.info \
	'*/test/*' --output-file $(COVERAGE_BUILD_DIR)/filtered_coverage.info
	@lcov --quiet --remove $(COVERAGE_BUILD_DIR)/filtered_coverage.info \
	'*/tac_generator.c' --output-file $(COVERAGE_BUILD_DIR)/filtered_coverage.info
	genhtml $(COVERAGE_BUILD_DIR)/filtered_coverage.info \
	--output-directory $(COVERAGE_BUILD_DIR)
	@mkdir -p $(COVERAGE_BUILD_DIR)/coverage_data
	@find $(COVERAGE_BUILD_DIR) -maxdepth 1 -type f -exec \
	mv -f {} $(COVERAGE_BUILD_DIR)/coverage_data/ \;
	@find $(COVERAGE_BUILD_DIR) -maxdepth 1 -type d ! -name \
	'coverage_data' ! -path '$(COVERAGE_BUILD_DIR)' -exec mv {} \
	$(COVERAGE_BUILD_DIR)/coverage_data/ \;
	@echo '<html><head><meta http-equiv="refresh" content="0; url=coverage_data/index.html"></head></html>' > $(COVERAGE_BUILD_DIR)/code_coverage_web_result.html
else
process-coverage:
	@echo "$(COLOR_RED)Cíl 'process-coverage' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif


###                                                                          ###
#                KOMPILACE souborů potřebných pro CODE COVERAGE                #
###                                                                          ###

# Stavba objektových souborů s přepínači pro pokrytí kódu
$(COVERAGE_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(COVERAGE_BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_COV) -c $< -o $@

$(COVERAGE_BUILD_DIR)/$(INTEGRATE).o: $(TEST_DIR)/$(INTEGRATE).cpp
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
ifndef DISABLE_TARGETS
pack-prepare:
	@{ \
		missing_files=0; \
		if [ -d "$(SRC_DIR)" ]; then \
			rsync -a --include '*/' --include '*.c' --exclude '*' --exclude '*/' \
			--prune-empty-dirs $(SRC_DIR)/ $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			echo "$(COLOR_RED)\nError: Adresář "$(SRC_DIR)" neexistuje.$(COLOR_RESET)"; \
		fi; \
		if [ -d "$(SRC_DIR)" ]; then \
			rsync -a --include '*/' --include '*.h' --exclude '*' --exclude '*/' \
			--prune-empty-dirs $(SRC_DIR)/ $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			echo "$(COLOR_RED)\nError: Adresář "$(SRC_DIR)" neexistuje.$(COLOR_RESET)"; \
		fi; \
		if [ -f "$(SRC_DIR)/Makefile" ]; then \
			rsync -a $(SRC_DIR)/Makefile $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -f "$(SRC_DIR)/Doxyfile" ]; then \
			rsync -a $(SRC_DIR)/Doxyfile $(PACK_DIR)/$(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
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
		echo "$(COLOR_GREEN)\nSeznam zkopírovaných souborů:$(COLOR_RESET)"; \
		find $(PACK_DIR) -type f -printf "$(COLOR_GREEN)%p$(COLOR_RESET)\n"; \
		if [ "$$missing_files" -eq 1 ]; then \
			echo "$(COLOR_RED)\nSeznam chybějících souborů:$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(PACK_DIR)/$(ARCHIVE_DIR)/Makefile" ]; then \
			echo "$(COLOR_RED)Error: Soubor "Makefile" nebyl zkopírován.$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(PACK_DIR)/$(ARCHIVE_DIR)/Doxyfile" ]; then \
			echo "$(COLOR_RED)Error: Soubor "Doxyfile" nebyl zkopírován.$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(PACK_DIR)/$(ARCHIVE_DIR)/dokumentace.pdf" ]; then \
			echo "$(COLOR_RED)Error: Soubor "dokumentace.pdf" nebyl zkopírován.$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(PACK_DIR)/$(ARCHIVE_DIR)/rozdeleni" ]; then \
			echo "$(COLOR_RED)Error: Soubor "rozdeleni" nebyl zkopírován.$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(PACK_DIR)/$(ARCHIVE_DIR)/rozsireni" ]; then \
			echo "$(COLOR_RED)Error: Soubor "rozsireni" nebyl zkopírován.$(COLOR_RESET)"; \
		fi; \
	}
else
pack-prepare:
	@echo "$(COLOR_RED)Cíl 'pack-prepare' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

################################################################################
#                                                                              #
#                    CÍLE PRO INSTALACI POTŘEBNÝCH NÁSTROJŮ                    #
#                                                                              #
################################################################################

### DEV # install-dev-dep: # Nainstaluje závislosti potřebné pro používání všech funkcí 'Makefile'
ifndef DISABLE_TARGETS
install-dev-dep: update-dep install-doc-dep install-pack-dep install-help-dep install-cov-dep
else
install-dev-dep:
	@echo "$(COLOR_RED)Cíl 'install-dev-dep' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### DEV # install-help-dep: # Nainstaluje závislosti potřebné pro výpis nápovědy k 'Makefile'
ifndef DISABLE_TARGETS
install-help-dep:
	@dpkg -s less >/dev/null 2>&1 || (echo "Instaluji less" && sudo apt-get install less)
else
install-help-dep:
	@echo "$(COLOR_RED)Cíl 'install-help-dep' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### DEV # install-cov-dep: # Nainstaluje závislosti potřebné pro generování pokrytí kódu
ifndef DISABLE_TARGETS
install-cov-dep:
	@dpkg -s lcov >/dev/null 2>&1 || (echo "Instaluji lcov" && sudo apt-get install lcov)
else
install-cov-dep:
	@echo "$(COLOR_RED)Cíl 'install-cov-dep' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### DEV # install-doc-dep: # Nainstaluje závislosti potřebné pro generování dokumentace
ifndef DISABLE_TARGETS
install-doc-dep:
	@dpkg -s doxygen >/dev/null 2>&1 || (echo "Instaluji doxygen" && sudo apt-get install doxygen)
else
install-doc-dep:
	@echo "$(COLOR_RED)Cíl 'install-doc-dep' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### DEV # install-pack-dep: # Nainstaluje závislosti potřebné pro zabalení projektu
ifndef DISABLE_TARGETS
install-pack-dep:
	@dpkg -s rsync >/dev/null 2>&1 || (echo "Instaluji rsync" && sudo apt-get install rsync)
	@dpkg -s zip >/dev/null 2>&1 || (echo "Instaluji zip" && sudo apt-get install zip)
else
install-pack-dep:
	@echo "$(COLOR_RED)Cíl 'install-pack-dep' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif

### DEV # update-dep: # Aktualizuje seznam dostupných balíčků
ifndef DISABLE_TARGETS
update-dep:
	sudo apt-get update -y
else
update-dep:
	@echo "$(COLOR_RED)Cíl 'dev-update-dep' je zakázán pro odevzdání projektu.$(COLOR_RESET)"
endif
