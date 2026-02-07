#!/bin/bash

MINISHELL=./minishell
OUT_FILE=token_test_out.txt

run_test() {
    input="$1"
    description="$2"
    echo "Running test: $description"
    echo "Input: $input"
    echo "$input" | $MINISHELL > "$OUT_FILE" 2>&1
    exit_code=$?
    
    if [ $exit_code -ne 0 ]; then
        echo "FAIL: Minishell exited with code $exit_code"
        cat "$OUT_FILE"
        return 1
    fi
    
    echo "Output:"
    cat "$OUT_FILE"
    echo "----------------------------------------"
}

echo "Starting Tokenization Tests..."

# Basic Commands
run_test "ls -la" "Basic Command"

# Pipes
run_test "ls|wc" "Pipe (no spaces)"
run_test "ls | wc" "Pipe (spaces)"

# Redirections
run_test "echo hello > out" "Redirect Output"
run_test "echo hello >> out" "Append Output"
run_test "cat < in" "Redirect Input"
run_test "cat << EOF" "Here-doc"

# Logical Operators
run_test "cmd1 && cmd2" "Logical AND"
run_test "cmd1 || cmd2" "Logical OR"

# Parentheses
run_test "(ls|wc)" "Parentheses group"
run_test "( ls | wc )" "Parentheses with spaces"

# Quotes
run_test "echo 'hello world'" "Single Quotes"
run_test 'echo "hello world"' "Double Quotes"
run_test "echo \"nested 'quotes'\"" "Nested Single inside Double"
run_test "echo 'nested \"quotes\"'" "Nested Double inside Single"

# Complex / Edge Cases
run_test "echo \"\"" "Empty String"
run_test "       " "Whitespace only"
run_test "||" "Operator only"
run_test "echo 'unclosed" "Unclosed Quote (might fail/hang depending on implementation)"

echo "Tests Completed."
rm -f "$OUT_FILE"
