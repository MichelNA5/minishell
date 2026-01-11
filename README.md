*This project has been created as part of the 42 curriculum by mmakhlou, mnaouss.*

## Description

Minishell is a simplified Unix shell implementation written in C. This project aims to recreate a functional shell interpreter similar to bash, allowing users to execute commands, manage environment variables, and perform various shell operations.

**Goals:**
- Parse and execute shell commands from user input
- Support command pipelines using pipes (`|`)
- Handle input/output redirections (`>`, `>>`, `<`, `<<`)
- Implement built-in commands (echo, cd, pwd, export, unset, env, exit)
- Manage environment variables and their expansion
- Handle signals (SIGINT, SIGQUIT) appropriately
- Support single and double quotes
- Provide a user-friendly interactive prompt using readline library

**Overview:**
The shell operates in a REPL (Read-Eval-Print Loop) fashion: it reads user input, parses the command structure, validates syntax, and executes commands either as built-in functions or external programs. The implementation follows a modular architecture with separate components for tokenization, parsing, and execution, ensuring clean separation of concerns and maintainability.

**Project Structure:**
```
minishell/
├── Makefile
├── include/
│   └── minishell.h
├── libft/
│   └── (custom C library source files)
└── src/
    ├── builtins/
    ├── env/
    ├── execution/
    ├── input/
    ├── main/
    ├── parsing/
    ├── redirections/
    ├── signals/
    └── tokenize/
```

## Instructions

### Compilation

To compile the project, ensure you have the following dependencies installed:
- GCC compiler
- Make utility
- Readline library (development headers)

On Ubuntu/Debian:
```bash
sudo apt-get install build-essential libreadline-dev
```

On macOS:
```bash
brew install readline
```

Then compile the project using Make:
```bash
make
```

This will:
1. Compile the libft library (custom C library)
2. Compile all source files in the `src/` directory
3. Link everything together with the readline library to create the `minishell` executable

### Installation

No installation is required. The compiled `minishell` executable is ready to run directly from the project directory.

To clean object files:
```bash
make clean
```

To clean everything including the executable:
```bash
make fclean
```

To recompile from scratch:
```bash
make re
```

### Execution

Run the minishell executable:
```bash
./minishell
```

The shell will start and display a prompt. You can then enter commands:
```bash
$ echo "Hello, World!"
Hello, World!
$ ls -la
$ cd /tmp
$ pwd
/tmp
$ export MY_VAR="test"
$ env | grep MY_VAR
MY_VAR=test
$ exit
```

To exit the shell, you can:
- Type `exit` command
- Press `Ctrl+D` (EOF)
- Press `Ctrl+C` to interrupt current command (returns to prompt)

## Algorithm

This minishell implementation uses a **three-stage processing pipeline** following traditional compiler/interpreter design principles:

### 1. Tokenization (Lexical Analysis)

The tokenization stage converts the raw input string into a linked list of tokens. Each token represents a distinct lexical element:
- **WORD**: Regular text tokens (commands, arguments, file paths)
- **PIPE** (`|`): Command separator for pipelines
- **REDIR_IN** (`<`): Input redirection
- **REDIR_OUT** (`>`): Output redirection (truncate)
- **REDIR_APPEND** (`>>`): Output redirection (append)
- **REDIR_HEREDOC** (`<<`): Here-document input
- **QUOTE_SINGLE** / **QUOTE_DOUBLE**: Quoted strings
- **DOLLAR** (`$`): Environment variable expansion marker

**Algorithm rationale:** A finite-state machine approach scans the input character by character, identifying token boundaries based on special characters and whitespace. This approach is chosen for its simplicity, efficiency, and direct mapping to shell syntax rules. The tokenizer handles quotes specially to prevent premature tokenization of spaces and special characters within quoted strings.

**Example:** Input `echo "hello world" > file.txt` becomes:
- Token(ECHO) → Token(QUOTE_DOUBLE, "hello world") → Token(REDIR_OUT) → Token(WORD, "file.txt")

### 2. Parsing (Syntactic Analysis)

The parsing stage transforms the token list into a structured command representation (`t_parser`). The parser:
- Validates syntax (ensures proper token sequences, no invalid redirections)
- Counts commands (number of pipe-separated commands)
- Builds command structures (`t_cmd`) containing:
  - Argument arrays (`args[]`)
  - Redirection arrays (`redirs[]`)
  - Pipe file descriptors

**Algorithm rationale:** A recursive-descent style parser processes tokens sequentially, building an Abstract Syntax Tree (AST) representation. For each command in a pipeline:
1. Initialize command structure
2. Process tokens until PIPE or end-of-input
3. Handle redirections (extract type and operand)
4. Handle arguments (words, quoted strings, environment variable expansions)
5. Validate syntax at each step

This approach allows for natural handling of command sequences and proper error detection. The parser uses a context structure (`t_parse_ctx`) to track current command index and argument position, enabling clean state management during parsing.

**Example:** Tokens from above are parsed into:
```
t_parser {
  cmd_count: 1
  cmds[0]: {
    args: ["echo", "hello world"]
    redirs: [{type: REDIR_OUT, file: "file.txt"}]
  }
}
```

### 3. Execution

The execution stage runs the parsed commands:
- **Built-in commands** (echo, cd, pwd, export, unset, env, exit): Executed directly in the parent process
- **External commands**: Executed using `fork()` and `execve()`

**Pipeline execution algorithm:**
For pipelines with multiple commands:
1. Create pipes between consecutive commands (`setup_pipes()`)
2. For each command in the pipeline:
   - Fork a child process
   - Redirect stdin/stdout to appropriate pipe ends
   - Setup file redirections (`<`, `>`, `>>`, `<<`)
   - Execute command (builtin or external)
3. Parent process waits for all children
4. Set exit status from the last command in pipeline

**Algorithm rationale:** Using `fork()` for each command allows proper isolation and signal handling. Pipes are created upfront and assigned to commands, enabling efficient data flow. Built-ins run in the parent process when there's a single command (for efficiency), but are forked in pipelines to maintain consistent file descriptor handling.

**Redirection handling:** Before executing any command, redirections are processed:
- Input redirections (`<`, `<<`) redirect `stdin`
- Output redirections (`>`, `>>`) redirect `stdout`
- File descriptors are saved before redirection and restored after execution
- Here-documents (`<<`) read input until delimiter is found

This three-stage approach provides clear separation of concerns, making the codebase maintainable and testable. Each stage can be developed and debugged independently, and the modular design allows for easy extension with new features.

## Resources

### Classic References

- **GNU Bash Manual**: [https://www.gnu.org/software/bash/manual/](https://www.gnu.org/software/bash/manual/) - Comprehensive reference for shell behavior and POSIX standards
- **POSIX Shell Command Language**: IEEE Std 1003.1 - Standard specification for shell behavior
- **Advanced Programming in the UNIX Environment** (W. Richard Stevens) - Essential reference for system programming, fork/exec, signals, and pipes
- **The Linux Programming Interface** (Michael Kerrisk) - Detailed coverage of Linux system calls and concepts used in shell implementation
- **GNU Readline Library Documentation**: [https://tiswww.cwru.edu/php/chet/readline/rltop.html](https://tiswww.cwru.edu/php/chet/readline/rltop.html) - API reference for readline integration
- **42 Network Resources**: Intra documentation on minishell project requirements and specifications

### AI Usage Description

AI tools were used in the following aspects of this project:

- **Code Structure and Architecture**: AI was consulted for understanding best practices in shell implementation architecture, particularly for organizing the tokenization-parsing-execution pipeline and structuring data types (tokens, commands, parser structures).

- **Algorithm Design**: AI assistance was used to discuss and refine the three-stage processing algorithm, especially for understanding trade-offs between different parsing approaches (recursive descent vs. table-driven parsers) and pipeline execution strategies.

- **Debugging Assistance**: When encountering memory leaks or segmentation faults, AI tools were used to help identify potential issues in memory management, particularly in complex scenarios involving token lists, command structures, and environment variable arrays.

- **Documentation**: AI was used to help structure and write comprehensive documentation, including this README file, ensuring clarity and completeness in explaining the project's architecture and implementation details.

- **Code Review and Optimization**: AI assistance was utilized to review code for potential improvements, particularly in error handling patterns, memory management practices, and code organization.

**Parts of the project NOT assisted by AI**: The core implementation logic, specific function implementations, debugging process, testing strategies, and the actual writing and debugging of the code were done manually by the project contributors.

