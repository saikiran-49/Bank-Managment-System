#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "account.h"

// Append new account to the file
int saveAccountToFile(struct Account acc) {
    FILE *fp = fopen(DATA_FILE, "ab");
    if (fp == NULL) {
        perror("Error opening data file for saving");
        return 0;
    }

    size_t written = fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);

    return written == 1;
}

// Load specific account by account number
int loadAccountFromFile(int accountNumber, struct Account *acc) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        return 0;
    }

    struct Account temp;
    while (fread(&temp, sizeof(struct Account), 1, fp) == 1) {
        if (temp.accountNumber == accountNumber) {
            if (acc != NULL) {
                *acc = temp;
            }
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// Update an existing account (rewrite the record in place)
int updateAccountInFile(struct Account acc) {
    FILE *fp = fopen(DATA_FILE, "r+b");
    if (fp == NULL) {
        perror("Error opening data file for updating");
        return 0;
    }

    struct Account temp;
    while (fread(&temp, sizeof(struct Account), 1, fp) == 1) {
        if (temp.accountNumber == acc.accountNumber) {
            // Move file position back one record
            if (fseek(fp, -(long)sizeof(struct Account), SEEK_CUR) != 0) {
                fclose(fp);
                return 0;
            }
            size_t written = fwrite(&acc, sizeof(struct Account), 1, fp);
            fclose(fp);
            return written == 1;
        }
    }

    fclose(fp);
    return 0;
}

// Delete account by rewriting all other records to a temp file
int deleteAccountFromFile(int accountNumber) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        return 0;
    }

    FILE *tempFp = fopen("temp.dat", "wb");
    if (tempFp == NULL) {
        fclose(fp);
        perror("Error opening temporary file for deletion");
        return 0;
    }

    struct Account temp;
    int found = 0;

    while (fread(&temp, sizeof(struct Account), 1, fp) == 1) {
        if (temp.accountNumber == accountNumber) {
            found = 1;
            continue;
        }
        fwrite(&temp, sizeof(struct Account), 1, tempFp);
    }

    fclose(fp);
    fclose(tempFp);

    if (!found) {
        // No changes; remove temp file
        remove("temp.dat");
        return 0;
    }

    // Replace original file with temp
    if (remove(DATA_FILE) != 0) {
        perror("Error removing original data file");
        return 0;
    }
    if (rename("temp.dat", DATA_FILE) != 0) {
        perror("Error renaming temporary file");
        return 0;
    }

    return 1;
}

// Get next unique account number by scanning file
int getNextAccountNumber(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    int maxAccNo = 1000; // starting base

    if (fp == NULL) {
        // File doesn't exist yet, first account
        return maxAccNo + 1;
    }

    struct Account temp;
    while (fread(&temp, sizeof(struct Account), 1, fp) == 1) {
        if (temp.accountNumber > maxAccNo) {
            maxAccNo = temp.accountNumber;
        }
    }

    fclose(fp);
    return maxAccNo + 1;
}

// Display all accounts in a tabular format
void displayAllAccounts(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No accounts found. Data file does not exist.\n");
        return;
    }

    struct Account temp;
    int count = 0;

    printf("=======================================================================\n");
    printf("%-10s %-25s %-10s %-12s\n", "Acc No", "Name", "Type", "Balance");
    printf("=======================================================================\n");

    while (fread(&temp, sizeof(struct Account), 1, fp) == 1) {
        printf("%-10d %-25s %-10s %10.2f\n",
               temp.accountNumber,
               temp.name,
               temp.accountType,
               temp.balance);
        count++;
    }

    if (count == 0) {
        printf("No accounts to display.\n");
    }

    printf("=======================================================================\n");

    fclose(fp);
}

