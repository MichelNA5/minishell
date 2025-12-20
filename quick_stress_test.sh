#!/bin/bash

# Quick stress test for memory leaks
# Run specific scenarios to verify no leaks

echo "=== Quick Stress Test for Memory Leaks ==="
echo ""

# Test 1: Multiple sequential commands
echo "Test 1: Sequential commands (10x)"
echo -e "echo test1\necho test2\necho test3\necho test4\necho test5\necho test6\necho test7\necho test8\necho test9\necho test10\nexit" | \
  valgrind --leak-check=full --show-leak-kinds=all --log-file=/tmp/stress_seq.log ./minishell >/dev/null 2>&1
if grep -q "All heap blocks were freed" /tmp/stress_seq.log; then
    echo "✓ PASSED - No leaks"
else
    echo "✗ FAILED - Check /tmp/stress_seq.log"
fi

# Test 2: Complex pipes
echo ""
echo "Test 2: Complex pipe chain (4 pipes)"
echo -e "echo hello | cat | cat | cat | cat\nexit" | \
  valgrind --leak-check=full --show-leak-kinds=all --log-file=/tmp/stress_pipe.log ./minishell >/dev/null 2>&1
if grep -q "All heap blocks were freed" /tmp/stress_pipe.log; then
    echo "✓ PASSED - No leaks"
else
    echo "✗ FAILED - Check /tmp/stress_pipe.log"
fi

# Test 3: Environment operations
echo ""
echo "Test 3: Environment variable stress (export/unset cycle)"
echo -e "export T1=1\nexport T2=2\nexport T3=3\nexport T4=4\nexport T5=5\nunset T1\nunset T2\nunset T3\nunset T4\nunset T5\nexit" | \
  valgrind --leak-check=full --show-leak-kinds=all --log-file=/tmp/stress_env.log ./minishell >/dev/null 2>&1
if grep -q "All heap blocks were freed" /tmp/stress_env.log; then
    echo "✓ PASSED - No leaks"
else
    echo "✗ FAILED - Check /tmp/stress_env.log"
fi

# Test 4: Builtin commands
echo ""
echo "Test 4: Builtin commands (pwd, echo, cd)"
echo -e "pwd\npwd\npwd\necho test\necho test\ncd /tmp\npwd\ncd /\npwd\nexit" | \
  valgrind --leak-check=full --show-leak-kinds=all --log-file=/tmp/stress_builtin.log ./minishell >/dev/null 2>&1
if grep -q "All heap blocks were freed" /tmp/stress_builtin.log; then
    echo "✓ PASSED - No leaks"
else
    echo "✗ FAILED - Check /tmp/stress_builtin.log"
fi

# Test 5: Redirections
echo ""
echo "Test 5: Multiple redirections"
echo -e "echo test1 > /tmp/r1.txt\necho test2 > /tmp/r2.txt\necho test3 > /tmp/r3.txt\ncat /tmp/r1.txt\ncat /tmp/r2.txt\ncat /tmp/r3.txt\nrm /tmp/r1.txt /tmp/r2.txt /tmp/r3.txt\nexit" | \
  valgrind --leak-check=full --show-leak-kinds=all --log-file=/tmp/stress_redir.log ./minishell >/dev/null 2>&1
if grep -q "All heap blocks were freed" /tmp/stress_redir.log; then
    echo "✓ PASSED - No leaks"
else
    echo "✗ FAILED - Check /tmp/stress_redir.log"
fi

echo ""
echo "=== Summary ==="
echo "Check /tmp/stress_*.log files for detailed valgrind reports"

