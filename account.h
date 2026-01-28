// account.h - Structure definitions and function declarations

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdio.h>

// -------------------------------
// Constants
// -------------------------------

#define DATA_FILE "accounts.dat"
#define MIN_BALANCE_SAVINGS 100.0f
#define MIN_BALANCE_CURRENT 0.0f

#define ACCOUNT_TYPE_SAVINGS "Savings"
#define ACCOUNT_TYPE_CURRENT "Current"

// -------------------------------
// Structures
// -------------------------------

struct Account {
    int accountNumber;
    char name[50];
    char address[100];
    char phoneNumber[15];
    float balance;
    char accountType[20]; // Savings / Current
};

// -------------------------------
// Menu and UI
// -------------------------------

void displayMenu(void);
int getMenuChoice(void);

// -------------------------------
// Account Operations
// -------------------------------

void createAccount(void);
void depositMoney(void);
void withdrawMoney(void);
void checkBalance(void);
void displayAccountDetails(void);
void listAllAccounts(void);
void deleteAccountMenu(void);

// -------------------------------
// File Operations
// -------------------------------

int saveAccountToFile(struct Account acc);
int loadAccountFromFile(int accountNumber, struct Account *acc);
int updateAccountInFile(struct Account acc);
int deleteAccountFromFile(int accountNumber);
int getNextAccountNumber(void);
void displayAllAccounts(void);

// -------------------------------
// Utility Functions
// -------------------------------

void clearScreen(void);
void pauseScreen(void);
int validateAmount(float amount);
int isValidPhoneNumber(const char *phone);
void trimNewline(char *str);
void flushInputBuffer(void);

#endif // ACCOUNT_H

