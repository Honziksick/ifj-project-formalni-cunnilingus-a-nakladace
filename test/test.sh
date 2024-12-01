#!/bin/bash

# IFJ24 - Test script for running compiler and interpreter tests

TESTDIR=../test/test_examples/ifj24code_examples
COMPILER=./ifj24_compiler                   # Path to the compiler binary
INTERPRETER=../test/ic24int                 # Path to the interpreter binary
path_in="$TESTDIR"/in                       # Input directory containing .zig and .in* files
path_out="$TESTDIR"/out                     # Output directory for generated files
path_ref="$TESTDIR"/ref                     # Reference direcotry for .ref* and .exit* files

# Define color codes
RED='\033[31m'
GREEN='\033[32m'
YELLOW='\033[33m'
RESET='\033[0m'
CYAN="\033[36m"
BLUE="\033[34m"
BROWN="\033[0;33m"
PINK='\033[35m' 

# Check for required arguments
if [ $# -gt 0 ]; then
    echo -e "${RED}Usage: $0${RESET}"
    echo -e "${RED}Before running this script make sure you launched "make"!${RESET}"
    exit 1
fi

# Ensure output directory exists
mkdir -p "$path_out"

tests_total=0
tests_passed=0
tests_failed=0
tests_no_ref=0
results_table=""

# Iterate over .zig files in the input directory
for code in "$path_in"/*.zig; do
    # Extract base name without extension
    base_name=$(basename "$code" .zig)
    ref_file="${path_ref}/${base_name}.ref"
    expected_exit_file="${path_ref}/${base_name}.exit"

    # Generate .ifj24code file using the compiler
    ifjcode="${path_out}/${base_name}.ifj24code"
    echo -e "\nCompiling: $(echo "$code" | sed "s|$TESTDIR|.|g") -> $(echo "$ifjcode" | sed "s|$TESTDIR|.|g")${RESET}"

    "$COMPILER" < "$code" > "$ifjcode"

    compiler_exit_code=$?

    # Test exit code of Compiler
    if [ $compiler_exit_code -ne 0 ]; then
        tests_total=$((tests_total + 1))
        if [ -f "$expected_exit_file" ]; then
            expected_exit_code=$(cat "$expected_exit_file")
            
            if [ "$expected_exit_code" -eq "$compiler_exit_code" ]; then
                echo -e "${GREEN}Test passed: $(echo "$code" | sed "s|$TESTDIR|.|g") exit code: $compiler_exit_code${RESET}"
                tests_passed=$((tests_passed + 1))
                results_table+="${GREEN}$base_name | $expected_exit_file | PASSED\n${RESET}"        
                continue 
            else
                echo -e "${RED}Compiler failed with exit code $compiler_exit_code for $(echo "$code" | sed "s|$TESTDIR|.|g"), expected exit code: $expected_exit_code${RESET}"
                results_table+="${RED}$base_name | ${RED}$expected_exit_file${RESET} | ${RED}FAILED${RESET} | $compiler_exit_code | ${RESET}\n"
                tests_failed=$((tests_failed + 1))
                continue;
            fi
        fi
        echo -e "${RED}Compiler failed with exit code $compiler_exit_code for $(echo "$code" | sed "s|$TESTDIR|.|g")${RESET}"
        results_table+="$base_name | ${CYAN}N/A${RESET} | ${RED}COMPILER FAILED${RESET} | $compiler_exit_code | ${RESET}\n"
        tests_failed=$((tests_failed + 1))
        continue # Skip to the next file if the compilation fails
    fi


    # Iterate over matching .in* files
    for input_file in "$path_in/${base_name}.in"*; do
        # Skip if no matching input files
        if [ "$input_file" = "$path_in/${base_name}.in*" ]; then
            input_file="${path_in}/empty.in"
            #echo "No input file found, using default empty file: $input_file"
        fi

        
		after_name="${input_file##*.in}"

        # Define output and reference file paths
        output_file="${path_out}/${base_name}.out${after_name}"
        ref_file="${path_ref}/${base_name}.ref${after_name}"
        expected_exit_file="${path_ref}/${base_name}.exit${after_name}"

        # Run the interpreter
        echo -e "Interpreting: $(echo "$ifjcode" | sed "s|$TESTDIR|.|g") with input $(echo "$input_file" | sed "s|$TESTDIR|.|g") -> $(echo "$output_file" | sed "s|$TESTDIR|.|g")${RESET}"
        "$INTERPRETER" "$ifjcode" < "$input_file" > "$output_file"
        interpreter_exit_code=$?

        # Test exit code of Interpreter
        if [ $interpreter_exit_code -ne 0 ] || [ -f $expected_exit_file ]; then
            if [ -f $expected_exit_file ]; then
                tests_total=$((tests_total + 1))
                expected_exit_code=$(cat "$expected_exit_file")
                if [ "$expected_exit_code" -eq "$interpreter_exit_code" ]; then
                    echo -e "${GREEN}Test passed: $(echo "$code" | sed "s|$TESTDIR|.|g") exit code: $interpreter_exit_code${RESET}"
                    tests_passed=$((tests_passed + 1))
                    results_table+="${GREEN}$base_name | $expected_exit_file | PASSED\n${RESET}"        
                else
                    echo -e "${RED}Interpretation failed with exit code $interpreter_exit_code for $(echo "$code" | sed "s|$TESTDIR|.|g"), expected exit code: $expected_exit_code${RESET}"
                    results_table+="${RED}$base_name | ${RED}$expected_exit_file${RESET} | ${RED}FAILED${RESET} | $interpreter_exit_code | ${RESET}\n"
                    tests_failed=$((tests_failed + 1))
                    continue;
                fi
            else
                echo -e "${RED}Error: Interpretation failed for $(echo "$ifjcode" | sed "s|$TESTDIR|.|g") (exit code: $interpreter_exit_code)${RESET}"
                results_table+="$base_name | $input_file | ${RED}INTERPRETER FAILED${RESET} | $interpreter_exit_code\n"
                tests_failed=$((tests_failed + 1))
                continue
            fi
        fi

		tests_total=$((tests_total + 1))

        # Compare output to reference, if reference exists
        if [ -f "$ref_file" ]; then
            if diff -q "$output_file" "$ref_file" > /dev/null; then
                echo -e "${GREEN}Test passed: $(echo "$code" | sed "s|$TESTDIR|.|g") $(echo "$input_file" | sed "s|$TESTDIR|.|g")${RESET}"
                tests_passed=$((tests_passed + 1))
                results_table+="${GREEN}$base_name | $input_file | PASSED\n${RESET}"
            else
                echo -e "${RED}Output differs from reference file:${RESET}"
                echo -e "${YELLOW}Differences:${RESET}"
                diff "$output_file" "$ref_file"
                tests_failed=$((tests_failed + 1))
                results_table+="${RED}$base_name | $input_file | FAILED\n${RESET}"
            fi
        else
            echo -e "${YELLOW}No reference file for test: $input_file, $ref_file expected ${RESET}"
            tests_no_ref=$((tests_no_ref + 1))
            results_table+="${YELLOW}$base_name | $input_file | NO REFERENCE ${RESET}\n"
        fi
    done
done

# Print results summary
echo -e "\n${BLUE}--- Test Results Summary ---${RESET}"
echo -e "Total tests: $tests_total"
echo -e "Tests passed: $tests_passed"
echo -e "Tests failed: $tests_failed"
echo -e "Tests without reference: $tests_no_ref"

# Print detailed results table
echo -e "\n${CYAN}Detailed Results:${RESET}"
echo -e "File Base | Input File | Result ${RESET}"
echo -e "-----------------------------------"
echo -e "$results_table" | sed "s|$TESTDIR|.|g" | column -t -s "|"
echo -e "${RESET}"

exit 0
