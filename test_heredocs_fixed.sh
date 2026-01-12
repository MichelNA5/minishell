#!/bin/bash

# Test script for heredoc functionality after fixes

echo "=== HEREDOC TESTS ==="
echo ""

echo "Test 1: Basic heredoc"
echo -e "cat <<HEREDOC\noi\nHEREDOC\nexit" | ./minishell 2>&1 | grep -A1 "cat <<HEREDOC"
echo ""

echo "Test 2: Multiple heredocs (should use last one)"
echo -e "cat <<HERE <<DOC\noi\nHERE\ntime\nDOC\nexit" | ./minishell 2>&1 | grep -A4 "cat <<HERE"
echo ""

echo "Test 3: Heredoc with pipe (CRITICAL FIX)"
echo -e "cat <<HERE | ls\noi\nHERE\nexit" | ./minishell 2>&1 | grep -A3 "cat <<HERE"
echo ""

echo "Test 4: Heredoc with grep"
echo -e "cat <<HERE | grep oi\noi\ntest\noi again\nHERE\nexit" | ./minishell 2>&1 | grep -A6 "cat <<HERE"
echo ""

echo "Test 5: Heredoc with wc"
echo -e "cat <<EOF | wc -l\nLine 1\nLine 2\nLine 3\nEOF\nexit" | ./minishell 2>&1 | grep -A5 "cat <<EOF"
echo ""

echo "Test 6: Variable expansion in heredoc"
echo -e "cat <<HERE\n\$USER\noi\nHERE\nexit" | ./minishell 2>&1 | grep -A4 "cat <<HERE"
echo ""

echo "Test 7: Dollar sign as delimiter"
echo -e "cat << \$\noi\n\$\nexit" | ./minishell 2>&1 | grep -A3 "cat <<"
echo ""

echo "=== ALL TESTS COMPLETED ==="
