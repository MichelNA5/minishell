#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== Memory Leak Detection Test ===${NC}\n"

# Check if minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell executable not found. Run 'make' first.${NC}"
    exit 1
fi

# Test function
test_leaks() {
    local test_name="$1"
    local test_input="$2"
    
    echo -e "${YELLOW}Testing: ${test_name}${NC}"
    echo "Command: $test_input"
    
    # Run valgrind with leak detection (using readline suppressions)
    valgrind --leak-check=full \
             --show-leak-kinds=definite,indirect \
             --track-origins=yes \
             --suppressions=readline.supp \
             --error-limit=no \
             --log-file=valgrind_${test_name// /_}.log \
             ./minishell <<EOF
$test_input
exit
EOF
    
    # Check for actual leaks (not readline's reachable memory)
    local def_lost=$(grep "definitely lost:" valgrind_${test_name// /_}.log | grep -o "[0-9,]* bytes" | head -1 | grep -o "[0-9,]*" | tr -d ',')
    local ind_lost=$(grep "indirectly lost:" valgrind_${test_name// /_}.log | grep -o "[0-9,]* bytes" | head -1 | grep -o "[0-9,]*" | tr -d ',')
    
    if [ "${def_lost:-0}" -eq 0 ] && [ "${ind_lost:-0}" -eq 0 ]; then
        echo -e "${GREEN}✓ No leaks detected${NC}\n"
        return 0
    else
        echo -e "${RED}✗ LEAKS DETECTED!${NC}"
        echo "Check valgrind_${test_name// /_}.log for details"
        grep -A 10 "LEAK SUMMARY\|definitely lost\|indirectly lost\|ERROR SUMMARY" valgrind_${test_name// /_}.log | head -30
        echo ""
        return 1
    fi
}

# Run tests
FAILED=0

# Test 1: Simple echo command
test_leaks "echo" "echo hello world" || FAILED=1

# Test 2: Exit command
test_leaks "exit" "exit" || FAILED=1

# Test 3: Environment variable
test_leaks "env_var" "echo \$HOME" || FAILED=1

# Test 4: Multiple commands
test_leaks "multiple" "echo test1; echo test2" || FAILED=1

# Test 5: Pipes
test_leaks "pipe" "echo hello | cat" || FAILED=1

# Test 6: Redirections
test_leaks "redirection" "echo test > /tmp/test_leak.txt && cat /tmp/test_leak.txt && rm /tmp/test_leak.txt" || FAILED=1

# Test 7: Empty input
test_leaks "empty" "" || FAILED=1

# Summary
echo -e "${YELLOW}=== Test Summary ===${NC}"
if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed - No leaks detected!${NC}"
else
    echo -e "${RED}Some tests failed - Check valgrind logs for details${NC}"
fi

echo -e "\n${YELLOW}Valgrind logs saved in: valgrind_*.log${NC}"

