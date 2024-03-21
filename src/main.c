#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct Flags {
    char encrypt[2];
    char decrypt[2];
};

void process_text(char* input, char* key, char* output, struct Flags* flags, char* selected_flag);

int main(int argc, char* argv[]) {
    short input_l, key_l;
    char* input = NULL; 
    char* key = NULL;
    char* output = NULL;
    char* flag = NULL;
    
    struct Flags flags = { .encrypt = "-e", .decrypt = "-d" };

   if (argc != 4) {
    printf("Example usage: %s -e \"this is a message\" \"thisisakey\"\n", argv[0]);
    printf("Example usage: %s -d \"moqk qk a wiqlhow\" \"thisisakey\"\n", argv[0]);
    return 1;
   }

    input_l = strlen(argv[2]);
    key_l = strlen(argv[3]);

    if (input_l == 0 || key_l == 0) {
        printf("Invalid size for message and key.\n");
        return 1;
    }

    flag = (char*) calloc(2, sizeof(char));
    input = (char*) calloc(input_l + 1, sizeof(char));
    key = (char*) calloc(key_l + 1, sizeof(char));
    output = (char*) calloc(input_l + 1, sizeof(char));

    if (input == NULL || key == NULL || output == NULL) {
        printf("[-] Fatal error: Could not allocate memory.\n");
        return 1;
    }
    
    if (strncmp(argv[1], flags.encrypt, 2) && strncmp(argv[1], flags.decrypt, 2)) {
        printf("[-] Invalid flags: Flag should either be -e or -d.\n");
        return 1;
    }

    strncpy(flag, argv[1], 2);
    strncpy(input, argv[2], input_l + 1);
    strncpy(key, argv[3], key_l + 1);


    process_text(input, key, output, &flags, flag);

    printf("Key: %s\nInput: %s\nOutput: %s\n", key, input, output);
    
    free(input);
    free(key);
    free(output);
    free(flag);

    return 0;
}

void process_text(char* input, char* key, char* output, struct Flags* flags, char* selected_flag) {
    char* key_start = key;
    char current_char;
    char key_base;
    char input_base;

    while(*input) {
        if (isalpha(*input)) {
            key_base = isupper(*key) ? 'A' : 'a';
            input_base = isupper(*input) ? 'A' : 'a';

            if (strncmp(flags->encrypt, selected_flag, 2) == 0) {
                current_char = (*input - input_base + *key - key_base) % 26 + input_base;
            } else if (strncmp(flags->decrypt, selected_flag, 2) == 0) {
                int difference = (*input - input_base) - (*key - key_base);
                
                if (difference < 0)
                    difference += 26;
                current_char = difference % 26 + input_base;
            }
                
            input++;
            key++;

            if (!*key)
                key = key_start;

            *output++ = current_char;
        } else if (isspace(*input)) {
            *output++ = '\x20';
            input++;
        } else {
            *output++ = *input;
            input++;
        }
    }

    *output = '\0';
}