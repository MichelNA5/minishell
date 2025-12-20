#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== STRESS TEST: Memory Leak Detection ===${NC}\n"

# Check if minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: minishell executable not found. Run 'make' first.${NC}"
    exit 1
fi

# Test function - uses newlines for proper command separation
test_leaks() {
    local test_name="$1"
    shift
    local test_commands="$@"
    
    echo -e "${YELLOW}Testing: ${test_name}${NC}"
    echo "Commands: $(echo "$test_commands" | tr '\n' '; ' | head -c 80)..."
    
    # Build command input with newlines
    local input=""
    for cmd in $test_commands; do
        input="${input}${cmd}\n"
    done
    input="${input}exit\n"
    
    # Run valgrind with leak detection
    echo -e "$input" | valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --error-limit=no \
             --log-file=valgrind_stress_${test_name// /_}.log \
             ./minishell >/dev/null 2>&1
    
    # Check for leaks in the log
    if grep -q "All heap blocks were freed -- no leaks are possible" valgrind_stress_${test_name// /_}.log || \
       (grep -q "ERROR SUMMARY: 0 errors" valgrind_stress_${test_name// /_}.log && \
        grep -q "in use at exit: 0 bytes in 0 blocks" valgrind_stress_${test_name// /_}.log); then
        local allocs=$(grep "total heap usage:" valgrind_stress_${test_name// /_}.log | grep -o "[0-9]* allocs" | grep -o "[0-9]*")
        echo -e "${GREEN}✓ No leaks detected (${allocs} allocations, all freed)${NC}\n"
        return 0
    else
        echo -e "${RED}✗ LEAKS DETECTED!${NC}"
        echo "Check valgrind_stress_${test_name// /_}.log for details"
        grep -A 10 "LEAK SUMMARY\|definitely lost\|indirectly lost\|ERROR SUMMARY\|in use at exit" valgrind_stress_${test_name// /_}.log | head -30
        echo ""
        return 1
    fi
}

# Run stress tests
FAILED=0
TOTAL_TESTS=0

echo -e "${BLUE}--- Sequential Command Stress (20 commands) ---${NC}\n"
test_leaks "sequential_20" "echo test1" "echo test2" "echo test3" "echo test4" "echo test5" "echo test6" "echo test7" "echo test8" "echo test9" "echo test10" "echo test11" "echo test12" "echo test13" "echo test14" "echo test15" "echo test16" "echo test17" "echo test18" "echo test19" "echo test20" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo -e "${BLUE}--- Pipe Stress Tests ---${NC}\n"
test_leaks "simple_pipe" "echo hello | cat" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "double_pipe" "echo hello | cat | cat" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "triple_pipe" "echo hello | cat | cat | cat" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "quad_pipe" "echo hello | cat | cat | cat | cat" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo -e "${BLUE}--- Redirection Stress Tests ---${NC}\n"
test_leaks "simple_redir" "echo test > /tmp/stress1.txt" "cat /tmp/stress1.txt" "rm /tmp/stress1.txt" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "multiple_redirs" "echo test1 > /tmp/stress1.txt" "echo test2 > /tmp/stress2.txt" "echo test3 > /tmp/stress3.txt" "cat /tmp/stress1.txt" "cat /tmp/stress2.txt" "cat /tmp/stress3.txt" "rm /tmp/stress1.txt /tmp/stress2.txt /tmp/stress3.txt" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "pipe_with_redir" "echo hello | cat > /tmp/pipe_redir.txt" "cat /tmp/pipe_redir.txt" "rm /tmp/pipe_redir.txt" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo -e "${BLUE}--- Builtin Commands Stress ---${NC}\n"
test_leaks "builtin_pwd" "pwd" "pwd" "pwd" "pwd" "pwd" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "builtin_echo" "echo test" "echo -n test" "echo hello world" "echo test1 test2 test3" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "builtin_cd" "pwd" "cd /tmp" "pwd" "cd /" "pwd" "cd /tmp" "pwd" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "builtin_env" "env" "env" "env" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo -e "${BLUE}--- Environment Variable Operations Stress ---${NC}\n"
test_leaks "export_single" "export TEST_VAR=test_value" "echo \$TEST_VAR" "unset TEST_VAR" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "export_multiple" "export VAR1=val1" "export VAR2=val2" "export VAR3=val3" "export VAR4=val4" "export VAR5=val5" "unset VAR1" "unset VAR2" "unset VAR3" "unset VAR4" "unset VAR5" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "export_repeated" "export T=1" "unset T" "export T=2" "unset T" "export T=3" "unset T" "export T=4" "unset T" "export T=5" "unset T" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "env_expansion" "echo \$HOME" "echo \$PWD" "echo \$USER" "echo test\$HOME" "echo \$HOME/test" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo -e "${BLUE}--- Quote Handling Stress ---${NC}\n"
test_leaks "single_quotes" "echo 'single quoted'" "echo 'test with spaces'" "echo 'test\"inside\"test'" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "double_quotes" "echo \"double quoted\"" "echo \"test with spaces\"" "echo \"test'inside'test\"" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "mixed_quotes" "echo 'literal'\$HOME'literal'" "echo \"expanded \$HOME\"" "echo 'single'\$PWD\"double\"" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo -e "${BLUE}--- Error Condition Stress ---${NC}\n"
test_leaks "invalid_command" "nonexistent_command_xyz" "another_fake_cmd" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "failed_redir" "echo test > /nonexistent/dir/file.txt" "cat /nonexistent/file.txt" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

echo -e "${BLUE}--- Complex Combinations ---${NC}\n"
test_leaks "complex_sequence" "echo test1" "echo test2 | cat" "pwd" "echo \$HOME" "export TEST=val" "echo \$TEST" "unset TEST" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

test_leaks "mixed_operations" "pwd" "echo hello | cat" "export T=1" "echo \$T" "cd /tmp" "pwd" "unset T" "cd /" || FAILED=1
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# Summary
echo -e "${BLUE}=== Stress Test Summary ===${NC}"
echo -e "Total tests run: ${TOTAL_TESTS}"
if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All stress tests passed - No leaks detected!${NC}"
    echo -e "${GREEN}Your minishell handles complex scenarios without memory leaks.${NC}"
else
    echo -e "${RED}✗ ${FAILED} test(s) failed - Check valgrind_stress_*.log files for details${NC}"
fi

echo -e "\n${YELLOW}Valgrind logs saved in: valgrind_stress_*.log${NC}"

