## Bank Management System (C, File-Based)

This is a simple console-based Bank Management System written in C, using binary file handling (`accounts.dat`) for persistent storage.

### Files
- `main.c` – Main menu loop and entry point
- `account_ops.c` – Core banking operations (create, deposit, withdraw, etc.)
- `file_ops.c` – File handling (save, load, update, delete, list)
- `utils.c` – Utility functions (screen control, validation, input helpers)
- `account.h` – Struct definition and function declarations
- `accounts.dat` – Binary data file created at runtime

### Build Instructions (GCC)

From the project directory, run:

```bash
gcc -std=c99 -Wall -Wextra -o bank main.c account_ops.c file_ops.c utils.c
```

On Windows with MinGW or similar, you can run the same command in your terminal/PowerShell (without `bash`):

```bash
gcc -std=c99 -Wall -Wextra -o bank.exe main.c account_ops.c file_ops.c utils.c
```

Then run:

```bash
./bank      # or: bank.exe on Windows
```

### Core Features
- Create new accounts with auto-generated account numbers
- Deposit and withdraw money with validation and minimum balance rules
- Check balance and display full account details
- List all accounts in a table
- Delete accounts with confirmation
- All operations are persisted immediately to `accounts.dat`

