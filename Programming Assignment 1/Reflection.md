#### Name: Dylan Phoutthavong
#### Date: March 8th, 2025
#### Course: CSCI 3751

# PA1 Reflection

### 1. Creating and Editing `myShell2.c` Using `vi`

For this assignment, I used the `vi` editor to manually type and edit `myShell2.c`, implementing a basic shell program in C. This involved:

- Learning and using `vi` commands (`i` for insert mode, `Esc` to exit insert mode, `:wq` to save and quit).

- Configuring `vi` settings in `.vimrc`, including enabling `set tabstop=4` for indentation.

- Comparing my typed code against `shell2.c` using the `diff` tool to ensure correctness.

### 2. Modifying `myShell2.c` to Support Arguments and `execvp()`

- Replaced `execlp()` with `execvp()` to handle command-line arguments.

- Implemented dynamic memory allocation using `malloc()` for storing parsed arguments.

- Used `strtok()` to parse user input into an array of strings.

- Ensured memory was freed properly to prevent leaks.

### 3. Implementing Output Redirection (`>`) and Wildcard Expansion (`*`)

- Handled redirection by detecting `>`, opening a file, and using `dup2()` to redirect output.

- Used `wordexp()` to expand wildcard arguments before executing commands.

- Modified `execvp()` to ensure proper execution of parsed commands.

### 4. Reflection on `vi` Editor Experience

Using `vi` in the beginiing was challenging, but over time, I feel more comfortable using it. Learning navigation commands (`h, j, k, l`), search (`/pattern`), and efficient text manipulation (`dd`, `yy`, `p`) significantly improving efficiency. Learning `vi` was essential for efficiently editing `myShell2.c` within a Unix environment.

# Extra Credit Reflection

### 1. Explanation of Why Wildcards (`*`), Pipes (`|`), and Redirections (`>`) Fail in `execvp()`

- `execvp()` does not process special characters like `>`, `<`, `|`, or `*` because it directly executes the provided command **without shell preprocessing**.

- In a standard shell (`bash`), these characters are **interpreted before execution**, but `execvp()` treats them as normal arguments, causing failures.

- `find . -name "*.c"` works because `find` itself supports wildcard handling, unlike `ls *`, which depends on the shell for expansion.

### 2. Enhancing myShell2.c to myShell2++ Using bash -c

- Instead of executing commands directly via execvp(), the enhanced shell uses:

```
execlp("bash", "bash", "-c", buf, NULL);
```

- This forces Bash to handle command processing **before execution**, resolving issues with `*`, `>`, `<`, and `|`.

### 3. Compilation and Testing of myShell2++

- Compiled using:
```
make clean && make myShell2++
```
- Tested:

  - **Output Redirection**: `ls -lrt > output.txt` (stored output correctly)

  - **Pipes**: `ls -l | grep ".c"` (filtered files correctly)

  - **Wildcard Expansion**: `ls she*` (listed matching files)

  - **Quoted Text Handling**: `echo "Hello World"` (treated as a single argument)

### 4. Summary of Enhancements

- **Handled output redirection (`>`) properly**.
- **Allowed pipes (`|`) and wildcard expansion (`*`)**.
- **Executed complex Bash commands accurately using `bash -c`**.
- **Ensured memory safety and correct command execution**.

Overall, this extra credit portion has improved my understanding of how Bash processes commands and how `execvp()` differs from a full shell environment. Implementing `myShell2++` provided a deeper insight into Unix system calls and shell scripting behavior.

