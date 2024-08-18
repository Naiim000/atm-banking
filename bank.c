#include<stdio.h>
#include<string.h>

// Define structure for bank account
struct Account {
    char first_name[50];
    char last_name[50];
    char phone[15];
    float balance;
    char pin[20];
    int acc_no;
};

// Function prototypes
void writeAccount(struct Account acc);
void readAccountByPhone(const char *phone);
void createAccount();
int verifyPin(const char *phone, const char *pin);
void depositMoney(const char *phone, const char *pin);
void withdrawMoney(const char *phone, const char *pin);
int getNextAccountNumber();

void writeAccount(struct Account acc) {
    FILE *file = fopen("accounts.txt", "ab");
    if (file != NULL) {
        fwrite(&acc, sizeof(struct Account), 1, file);
        fclose(file);
        printf("Account created successfully.\n");
    } else {
        printf("Error opening file for writing.\n");
    }
}

void readAccountByPhone(const char *phone) {
    FILE *file = fopen("accounts.txt", "rb");
    if (file != NULL) {
        struct Account acc;
        int found = 0;
        while (fread(&acc, sizeof(struct Account), 1, file) == 1) {
            if (strcmp(acc.phone, phone) == 0) {
                printf("Name: %s %s\nPhone: %s\nBalance: %.2f\nAccount Number: %d\n", 
                       acc.first_name, acc.last_name, acc.phone, acc.balance, acc.acc_no);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Account not found for phone number %s\n", phone);
        }
        fclose(file);
    } else {
        printf("Error opening file for reading.\n");
    }
}

void createAccount() {
    struct Account newAcc;
    printf("Enter name: ");
    scanf("%s %s", newAcc.first_name, newAcc.last_name);
    printf("Enter phone number: ");
    scanf("%s", newAcc.phone);
    printf("Enter PIN: ");
    scanf("%s", newAcc.pin);
    newAcc.balance = 500.0;
    printf("Enter Account Number: ");
    scanf("%d", &newAcc.acc_no);

    writeAccount(newAcc);
}

int verifyPin(const char *phone, const char *pin) {
    FILE *file = fopen("accounts.txt", "rb");
    if (file != NULL) {
        struct Account acc;
        while (fread(&acc, sizeof(struct Account), 1, file) == 1) {
            if (strcmp(acc.phone, phone) == 0) {
                if (strcmp(acc.pin, pin) == 0) {
                    fclose(file);
                    return 1; // Pin is correct
                } else {
                    fclose(file);
                    return 0; // Pin is incorrect
                }
            }
        }
        fclose(file);
    }
    return -1; // Account not found
}

void depositMoney(const char *phone, const char *pin) {
    if (verifyPin(phone, pin) == 1) {
        FILE *file = fopen("accounts.txt", "rb+");
        if (file != NULL) {
            struct Account acc;
            while (fread(&acc, sizeof(struct Account), 1, file) == 1) {
                if (strcmp(acc.phone, phone) == 0) {
                    float amount;
                    printf("Enter amount to deposit: ");
                    scanf("%f", &amount);
                    acc.balance += amount;
                    fseek(file, -sizeof(struct Account), SEEK_CUR);
                    fwrite(&acc, sizeof(struct Account), 1, file);
                    printf("Deposit successful. New balance: %.2f\n", acc.balance);
                    break;
                }
            }
            fclose(file);
        } else {
            printf("Error opening file.\n");
        }
    } else {
        printf("Invalid phone number or PIN.\n");
    }
}

void withdrawMoney(const char *phone, const char *pin) {
    if (verifyPin(phone, pin) == 1) {
        FILE *file = fopen("accounts.txt", "rb+");
        if (file != NULL) {
            struct Account acc;
            while (fread(&acc, sizeof(struct Account), 1, file) == 1) {
                if (strcmp(acc.phone, phone) == 0) {
                    float amount;
                    printf("Enter amount to withdraw: ");
                    scanf("%f", &amount);
                    if (acc.balance >= amount) {
                        acc.balance -= amount;
                        fseek(file, -sizeof(struct Account), SEEK_CUR);
                        fwrite(&acc, sizeof(struct Account), 1, file);
                        printf("Withdrawal successful. New balance: %.2f\n", acc.balance);
                    } else {
                        printf("Insufficient balance.\n");
                    }
                    break;
                }
            }
            fclose(file);
        } else {
            printf("Error opening file.\n");
        }
    } else {
        printf("Invalid phone number or PIN.\n");
    }
}

// int getNextAccountNumber() {
//     FILE *file = fopen("account_number.txt", "r+");
//     int accNo;
//     if (file == NULL) {
//         file = fopen("account_number.txt", "w+");
//                 accNo = 1000001;
//         fprintf(file, "%d", accNo);
//     } else {
//         fscanf(file, "%d", &accNo);
//         accNo++;
//         rewind(file);
//         fprintf(file, "%d", accNo);
//     }
//     fclose(file);
//     return accNo;
// }

int main() {
    int choice;
    char phone[15];
    char pin[20];
    
    do {
        printf("\n1. Create New Account\n2. Check Account Details\n3. Deposit Money\n4. Withdraw Money\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                printf("Enter phone number: ");
                scanf("%s", phone);
                readAccountByPhone(phone);
                break;
            case 3:
                printf("Enter phone number: ");
                scanf("%s", phone);
                printf("Enter PIN: ");
                scanf("%s", pin);
                depositMoney(phone, pin);
                break;
            case 4:
                printf("Enter phone number: ");
                scanf("%s", phone);
                printf("Enter PIN: ");
                scanf("%s", pin);
                withdrawMoney(phone, pin);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

