#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct option long_options[] = {
    {"encrypt", 0, NULL, 'e'},
    {"decrypt", 0, NULL, 'd'},
    {"help", 0, NULL, 'h'},
    {"input", 1, NULL, 'i'},
    {"key", 1, NULL, 'k'},
    {"output", 1, NULL, 'o'},
    {NULL, 0, NULL, 0}};

enum { ENCRYPT,
       DECRYPT,
       HELP,
       INPUT,
       KEY,
       OUTPUT };

void printHelp() {
    printf("\033[0;33mUsing Help Command...\033[0m\n");
    printf("=======================\n");
    printf("HW0301 - Stream Cipher\n");
    printf("=======================\n");
    printf("It is a Encrypt & Decrypt program using formula\n");
    printf("   ∀i ∈ { 0, . . . , n−1 }, c_i = f_i ⊕ k_(i mod(m))\n");

    printf("\n* Usages *\n");
    printf("  1. \"Encrypt: %s -e -i <input_file> -k <key_file> -o <output_file>\"\n", "hw0301");
    printf("  2. \"Decrypt: %s -d -i <input_file> -k <key_file> -o <output_file>\"\n", "hw0301");
    printf("\n* Options *\n");
    printf(" %s / %-10s\t | %-20s\n", "Short", "Long Opt.", "Function");
    printf("-------------------------+-----------------------\n");
    printf(" %3s    %-10s\t | %-20s\n", "-e", "--encrypt", "encrypt the input file");
    printf(" %3s    %-10s\t | %-20s\n", "-d", "--decrypt", "decrypt the input file");
    printf(" %3s    %-10s\t | %-20s\n", "-h", "--help", "print this help");
    printf(" %3s    %-10s\t | %-20s\n", "-i", "--input=FILE", "input file");
    printf(" %3s    %-10s\t | %-20s\n", "-k", "--key=FILE", "key file");
    printf(" %3s    %-10s\t | %-20s\n", "-o", "--output=FILE", "output file");
    return;
}

bool isInvalid(bool cmd[], char *input_file, char *output_file, char *key_file) {
    printf("\033[0;31m");
    if (!cmd[ENCRYPT] && !cmd[DECRYPT]) {
        printf("E: Please specify -e or -d\n");
        return true;
    }
    if (cmd[ENCRYPT] && cmd[DECRYPT]) {
        printf("E: Please specify -e or -d\n");
        return true;
    }
    if (!cmd[INPUT]) {
        printf("E: Please specify -i\n");
        return true;
    }
    if (!cmd[KEY]) {
        printf("E: Please specify -k\n");
        return true;
    }
    if (!cmd[OUTPUT]) {
        printf("E: Please specify -o\n");
        return true;
    }
    if (input_file == NULL) {
        printf("E: Please specify input file\n");
        return true;
    }
    if (output_file == NULL) {
        printf("E: Please specify output file\n");
        return true;
    }
    if (key_file == NULL) {
        printf("E: Please specify key file\n");
        return true;
    }
    printf("\033[0m");
    return false;
}

int enFile(char *input_file, char *output_file, char *key_file) {
    FILE *in = fopen(input_file, "rb");
    FILE *out = fopen(output_file, "wb");
    FILE *key = fopen(key_file, "rb");
    if (in == NULL || out == NULL || key == NULL) {
        printf("\033[0;31m");
        printf("E: File not found\n");
        printf("\033[0m");
        return -1;
    }

    uint8_t in_buf, key_buf;
    while (fread(&in_buf, sizeof(uint8_t), 1, in) == 1) {
        if (feof(key)) rewind(key);
        fread(&key_buf, sizeof(uint8_t), 1, key);
        in_buf ^= key_buf;
        fwrite(&in_buf, sizeof(uint8_t), 1, out);
    }

    fclose(in), fclose(out), fclose(key);
    return 0;
}

int deFile(char *input_file, char *output_file, char *key_file) {
    FILE *in = fopen(input_file, "rb");
    FILE *out = fopen(output_file, "wb");
    FILE *key = fopen(key_file, "rb");
    if (in == NULL || out == NULL || key == NULL) {
        printf("\033[0;31m");
        printf("E: File not found\n");
        printf("\033[0m");
        return -1;
    }

    uint8_t in_buf, key_buf;
    while (fread(&in_buf, sizeof(uint8_t), 1, in) == 1) {
        if (feof(key)) rewind(key);
        fread(&key_buf, sizeof(uint8_t), 1, key);
        in_buf ^= key_buf;
        fwrite(&in_buf, sizeof(uint8_t), 1, out);
    }

    fclose(in), fclose(out), fclose(key);
    return 0;
}

int main(const int argc, char *const argv[]) {
    bool cmd[6] = {false};
    int opt;
    char *input_file = NULL, *output_file = NULL, *key_file = NULL;

    while ((opt = getopt_long(argc, argv, "edhi:k:o:", long_options, NULL)) != -1) {
        if (opt == 'e') cmd[ENCRYPT] = true;
        if (opt == 'd') cmd[DECRYPT] = true;
        if (opt == 'h') cmd[HELP] = true, printHelp();
        if (opt == 'i') cmd[INPUT] = true, input_file = optarg;
        if (opt == 'k') cmd[KEY] = true, key_file = optarg;
        if (opt == 'o') cmd[OUTPUT] = true, output_file = optarg;
        if (opt == '?') {
            printf("Unknown option: %c\n", optopt);
            return -1;
        }
    }
    if (cmd[HELP]) return 0;
    if (isInvalid(cmd, input_file, output_file, key_file))
        return -1;

    if (cmd[ENCRYPT])
        return enFile(input_file, output_file, key_file);
    if (cmd[DECRYPT])
        return deFile(input_file, output_file, key_file);
    return 0;
}
