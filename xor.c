#include <stdio.h>
#include <string.h>

#define MAX_KEY_LENGTH 100

void xor_encrypt_decrypt(char *data, const char *key) {
    int key_len = strlen(key);
    for (int i = 0; data[i] != '\0'; i++) {
        data[i] ^= key[i % key_len];
    }
}

int main() {
    char choice[10];
    printf("Do you want to encrypt or decrypt? ");
    scanf("%s", choice);

    if (strcmp(choice, "encrypt") == 0) {
        char message[MAX_KEY_LENGTH];
        char filename[MAX_KEY_LENGTH];
        char key[MAX_KEY_LENGTH];

        printf("Enter the string to encrypt: ");
        fgets(message, MAX_KEY_LENGTH, stdin);
        message[strcspn(message, "\n")] = '\0';

        printf("Enter the filename to save the encrypted data: ");
        scanf("%s", filename);

        printf("Enter the key for encryption: ");
        scanf("%s", key);

        FILE *file = fopen(filename, "wb");
        if (file == NULL) {
            fprintf(stderr, "Error opening file for writing\n");
            return 1;
        }

        xor_encrypt_decrypt(message, key);
        fwrite(message, sizeof(char), strlen(message), file);
        fclose(file);
    } else if (strcmp(choice, "decrypt") == 0) {
        char filename[MAX_KEY_LENGTH];
        char key[MAX_KEY_LENGTH];

        printf("Enter the filename to read the encrypted data: ");
        scanf("%s", filename);

        printf("Enter the key for decryption: ");
        scanf("%s", key);

        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
            fprintf(stderr, "Error opening file for reading\n");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *data = malloc(size + 1);
        if (data == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(file);
            return 1;
        }

        fread(data, sizeof(char), size, file);
        data[size] = '\0';
        fclose(file);

        xor_encrypt_decrypt(data, key);
        printf("Decrypted message: %s\n", data);
        free(data);
    } else {
        printf("Invalid choice. Please enter 'encrypt' or 'decrypt'.\n");
    }

    return 0;
}