#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "account.h"

void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen(void) {
    printf("\nPress Enter to continue...");
    flushInputBuffer();
    getchar();
}

int validateAmount(float amount) {
    return amount > 0.0f;
}

int isValidPhoneNumber(const char *phone) {
    if (phone == NULL || phone[0] == '\0') {
        return 0;
    }

    for (size_t i = 0; phone[i] != '\0'; ++i) {
        if (phone[i] < '0' || phone[i] > '9') {
            return 0;
        }
    }

    return 1;
}

void trimNewline(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
    }
}

void flushInputBuffer(void) {
    int ch;
    // Read until newline or EOF
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard */
    }
}

