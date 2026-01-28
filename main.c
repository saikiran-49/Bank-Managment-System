#include <stdio.h>
#include <stdlib.h>

#include "account.h"

void displayMenu(void) {
    printf("=====================================\n");
    printf("        BANK MANAGEMENT SYSTEM       \n");
    printf("=====================================\n");
    printf("1. Create New Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Check Balance\n");
    printf("5. Display Account Details\n");
    printf("6. List All Accounts\n");
    printf("7. Delete Account\n");
    printf("8. Exit\n");
    printf("=====================================\n");
}

int getMenuChoice(void) {
    int choice;
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number between 1 and 8.\n");
        flushInputBuffer();
        return -1;
    }
    flushInputBuffer();
    return choice;
}

int main(void) {
    int choice;

    while (1) {
        clearScreen();
        displayMenu();
        choice = getMenuChoice();

        if (choice == -1) {
            pauseScreen();
            continue;
        }

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                withdrawMoney();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                displayAccountDetails();
                break;
            case 6:
                listAllAccounts();
                break;
            case 7:
                deleteAccountMenu();
                break;
            case 8:
                printf("Exiting the program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a number between 1 and 8.\n");
                pauseScreen();
                break;
        }
    }

    return 0;
}

