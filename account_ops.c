#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "account.h"

// Local helper to get line input safely
static void getLineInput(const char *prompt, char *buffer, size_t size, int allowEmpty) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, (int)size, stdin) == NULL) {
            printf("Input error. Please try again.\n");
            flushInputBuffer();
            continue;
        }
        trimNewline(buffer);

        if (!allowEmpty && (buffer[0] == '\0')) {
            printf("This field cannot be empty. Please enter a value.\n");
            continue;
        }
        break;
    }
}

// -------------------------------
// Account Operations
// -------------------------------

void createAccount(void) {
    clearScreen();
    printf("=====================================\n");
    printf("          CREATE NEW ACCOUNT         \n");
    printf("=====================================\n");

    struct Account acc;
    memset(&acc, 0, sizeof(struct Account));

    acc.accountNumber = getNextAccountNumber();

    getLineInput("Enter customer name: ", acc.name, sizeof(acc.name), 0);
    getLineInput("Enter address: ", acc.address, sizeof(acc.address), 0);

    // Phone number validation
    while (1) {
        getLineInput("Enter phone number: ", acc.phoneNumber, sizeof(acc.phoneNumber), 0);
        if (!isValidPhoneNumber(acc.phoneNumber)) {
            printf("Invalid phone number. Please enter digits only.\n");
        } else {
            break;
        }
    }

    // Account type
    int typeChoice;
    while (1) {
        printf("Select account type:\n");
        printf("1. Savings\n");
        printf("2. Current\n");
        printf("Enter choice (1-2): ");
        if (scanf("%d", &typeChoice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            flushInputBuffer();
            continue;
        }
        flushInputBuffer();
        if (typeChoice == 1) {
            strncpy(acc.accountType, ACCOUNT_TYPE_SAVINGS, sizeof(acc.accountType) - 1);
            acc.accountType[sizeof(acc.accountType) - 1] = '\0';
            break;
        } else if (typeChoice == 2) {
            strncpy(acc.accountType, ACCOUNT_TYPE_CURRENT, sizeof(acc.accountType) - 1);
            acc.accountType[sizeof(acc.accountType) - 1] = '\0';
            break;
        } else {
            printf("Invalid choice. Please select 1 or 2.\n");
        }
    }

    // Initial balance
    float initialDeposit;
    while (1) {
        printf("Enter initial deposit amount: ");
        if (scanf("%f", &initialDeposit) != 1) {
            printf("Invalid amount. Please enter a numeric value.\n");
            flushInputBuffer();
            continue;
        }
        flushInputBuffer();

        if (!validateAmount(initialDeposit) && initialDeposit != 0.0f) {
            printf("Amount must be positive.\n");
            continue;
        }

        if (strcmp(acc.accountType, ACCOUNT_TYPE_SAVINGS) == 0) {
            if (initialDeposit < MIN_BALANCE_SAVINGS) {
                printf("Minimum initial balance for Savings account is %.2f\n", MIN_BALANCE_SAVINGS);
                continue;
            }
        } else if (strcmp(acc.accountType, ACCOUNT_TYPE_CURRENT) == 0) {
            if (initialDeposit < MIN_BALANCE_CURRENT) {
                printf("Minimum initial balance for Current account is %.2f\n", MIN_BALANCE_CURRENT);
                continue;
            }
        }

        break;
    }

    acc.balance = initialDeposit;

    if (saveAccountToFile(acc)) {
        printf("\nAccount created successfully!\n");
        printf("Your Account Number is: %d\n", acc.accountNumber);
    } else {
        printf("Error: Could not save account. Please try again.\n");
    }

    pauseScreen();
}

void depositMoney(void) {
    clearScreen();
    printf("=====================================\n");
    printf("            DEPOSIT MONEY            \n");
    printf("=====================================\n");

    int accNo;
    printf("Enter account number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("Invalid account number.\n");
        flushInputBuffer();
        pauseScreen();
        return;
    }
    flushInputBuffer();

    struct Account acc;
    if (!loadAccountFromFile(accNo, &acc)) {
        printf("Account not found.\n");
        pauseScreen();
        return;
    }

    float amount;
    while (1) {
        printf("Enter amount to deposit: ");
        if (scanf("%f", &amount) != 1) {
            printf("Invalid amount. Please enter a numeric value.\n");
            flushInputBuffer();
            continue;
        }
        flushInputBuffer();

        if (!validateAmount(amount)) {
            printf("Amount must be greater than 0.\n");
            continue;
        }
        break;
    }

    acc.balance += amount;

    if (updateAccountInFile(acc)) {
        printf("Deposit successful.\n");
        printf("Updated balance for %s (Acc No: %d): %.2f\n",
               acc.name, acc.accountNumber, acc.balance);
    } else {
        printf("Error: Could not update account.\n");
    }

    pauseScreen();
}

void withdrawMoney(void) {
    clearScreen();
    printf("=====================================\n");
    printf("           WITHDRAW MONEY            \n");
    printf("=====================================\n");

    int accNo;
    printf("Enter account number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("Invalid account number.\n");
        flushInputBuffer();
        pauseScreen();
        return;
    }
    flushInputBuffer();

    struct Account acc;
    if (!loadAccountFromFile(accNo, &acc)) {
        printf("Account not found.\n");
        pauseScreen();
        return;
    }

    float amount;
    while (1) {
        printf("Enter amount to withdraw: ");
        if (scanf("%f", &amount) != 1) {
            printf("Invalid amount. Please enter a numeric value.\n");
            flushInputBuffer();
            continue;
        }
        flushInputBuffer();

        if (!validateAmount(amount)) {
            printf("Amount must be greater than 0.\n");
            continue;
        }
        break;
    }

    float minBalance = 0.0f;
    if (strcmp(acc.accountType, ACCOUNT_TYPE_SAVINGS) == 0) {
        minBalance = MIN_BALANCE_SAVINGS;
    } else {
        minBalance = MIN_BALANCE_CURRENT;
    }

    if (acc.balance - amount < minBalance) {
        printf("Insufficient balance.\n");
        printf("You must maintain a minimum balance of %.2f for a %s account.\n",
               minBalance, acc.accountType);
        pauseScreen();
        return;
    }

    acc.balance -= amount;

    if (updateAccountInFile(acc)) {
        printf("Withdrawal successful.\n");
        printf("Updated balance for %s (Acc No: %d): %.2f\n",
               acc.name, acc.accountNumber, acc.balance);
    } else {
        printf("Error: Could not update account.\n");
    }

    pauseScreen();
}

void checkBalance(void) {
    clearScreen();
    printf("=====================================\n");
    printf("           BALANCE INQUIRY           \n");
    printf("=====================================\n");

    int accNo;
    printf("Enter account number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("Invalid account number.\n");
        flushInputBuffer();
        pauseScreen();
        return;
    }
    flushInputBuffer();

    struct Account acc;
    if (!loadAccountFromFile(accNo, &acc)) {
        printf("Account not found.\n");
        pauseScreen();
        return;
    }

    printf("Account Holder: %s\n", acc.name);
    printf("Account Number: %d\n", acc.accountNumber);
    printf("Account Type  : %s\n", acc.accountType);
    printf("Current Balance: %.2f\n", acc.balance);

    pauseScreen();
}

void displayAccountDetails(void) {
    clearScreen();
    printf("=====================================\n");
    printf("         ACCOUNT DETAILS VIEW        \n");
    printf("=====================================\n");

    int accNo;
    printf("Enter account number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("Invalid account number.\n");
        flushInputBuffer();
        pauseScreen();
        return;
    }
    flushInputBuffer();

    struct Account acc;
    if (!loadAccountFromFile(accNo, &acc)) {
        printf("Account not found.\n");
        pauseScreen();
        return;
    }

    printf("=============================================\n");
    printf("Account Number : %d\n", acc.accountNumber);
    printf("Name           : %s\n", acc.name);
    printf("Address        : %s\n", acc.address);
    printf("Phone Number   : %s\n", acc.phoneNumber);
    printf("Account Type   : %s\n", acc.accountType);
    printf("Balance        : %.2f\n", acc.balance);
    printf("=============================================\n");

    pauseScreen();
}

void listAllAccounts(void) {
    clearScreen();
    printf("=====================================\n");
    printf("          LIST ALL ACCOUNTS          \n");
    printf("=====================================\n");

    displayAllAccounts();

    pauseScreen();
}

void deleteAccountMenu(void) {
    clearScreen();
    printf("=====================================\n");
    printf("            DELETE ACCOUNT           \n");
    printf("=====================================\n");

    int accNo;
    printf("Enter account number to delete: ");
    if (scanf("%d", &accNo) != 1) {
        printf("Invalid account number.\n");
        flushInputBuffer();
        pauseScreen();
        return;
    }
    flushInputBuffer();

    struct Account acc;
    if (!loadAccountFromFile(accNo, &acc)) {
        printf("Account not found.\n");
        pauseScreen();
        return;
    }

    printf("Account found:\n");
    printf("Account Number: %d\n", acc.accountNumber);
    printf("Name          : %s\n", acc.name);
    printf("Account Type  : %s\n", acc.accountType);
    printf("Balance       : %.2f\n", acc.balance);

    char confirm;
    printf("\nAre you sure you want to delete this account? (y/n): ");
    if (scanf(" %c", &confirm) != 1) {
        printf("Invalid input.\n");
        flushInputBuffer();
        pauseScreen();
        return;
    }
    flushInputBuffer();

    if (confirm == 'y' || confirm == 'Y') {
        if (deleteAccountFromFile(accNo)) {
            printf("Account deleted successfully.\n");
        } else {
            printf("Error: Could not delete account.\n");
        }
    } else {
        printf("Deletion cancelled.\n");
    }

    pauseScreen();
}

