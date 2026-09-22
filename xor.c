#include <stdio.h>
#include <string.h>
#include <stdlib.h> //needed to make fread stop crashing

#define MAX_KEY_LENGTH 100

void xor_encrypt_decrypt(char *data, const char *key) {
    int key_len = strlen(key);
    for (int i = 0; data[i] != '\0'; i++) {
        data[i] ^= key[i % key_len];
    }
}

void flush_stdin(void) { 
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
} //loop thorugh chars in stdin and do nothing to them


int main() {
    char choice[10];
    printf("Do you want to (e)ncrypt or (d)ecrypt? ");
    scanf("%s", choice);

    if (strcmp(choice, "e") == 0) {
        char message[MAX_KEY_LENGTH]; 
        char filename[MAX_KEY_LENGTH];
        char key[MAX_KEY_LENGTH];

        //first issue encountered, fgets reads the \n comming from the scanf input, returning early
        flush_stdin();
        // this can be remiated by "flushing the STDIN" which removes all chars

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
    }
    else if (strcmp(choice, "d") == 0) {
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

        fseek(file, 0, SEEK_END); //sets pointer to EOF
        long size = ftell(file); // sets size to current postion of file (end = size)
        fseek(file, 0, SEEK_SET); //returns pointer to start

        if (size < 0) {
            printf("negative sized file");
            return 1;
        }

        char *data = malloc(size + 1);

        if (data == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(file);
            return 1;
        }
        
        printf("this line runs \n");
        fread(data, sizeof(char), size, file);
        printf("this line does not run \n");

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