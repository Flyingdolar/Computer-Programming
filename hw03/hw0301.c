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
    printf("HW0301 - Stream Cipher\n");
    printf("Usage & option\n");
    printf("\n");
    printf("  -e, --encrypt\t\tencrypt the input file\n");
    printf("  -d, --decrypt\t\tdecrypt the input file\n");
    printf("  -h, --help\t\tprint this help\n");
    printf("  -i, --input=FILE\tinput file\n");
    printf("  -k, --key=FILE\tkey file\n");
    printf("  -o, --output=FILE\toutput file\n");
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

void enFile(char *input_file, char *output_file, char *key_file) {
    FILE *in = fopen(input_file, "rb");
    FILE *out = fopen(output_file, "wb");
    FILE *key = fopen(key_file, "rb");

    uint8_t in_buf, key_buf;
    while (fread(&in_buf, sizeof(uint8_t), 1, in) == 1) {
        if (feof(key)) rewind(key);
        fread(&key_buf, sizeof(uint8_t), 1, key);
        in_buf ^= key_buf;
        fwrite(&in_buf, sizeof(uint8_t), 1, out);
    }

    fclose(in), fclose(out), fclose(key);
    return;
}

void deFile(char *input_file, char *output_file, char *key_file) {
    FILE *in = fopen(input_file, "rb");
    FILE *out = fopen(output_file, "wb");
    FILE *key = fopen(key_file, "rb");

    uint8_t in_buf, key_buf;
    while (fread(&in_buf, sizeof(uint8_t), 1, in) == 1) {
        if (feof(key)) rewind(key);
        fread(&key_buf, sizeof(uint8_t), 1, key);
        in_buf ^= key_buf;
        fwrite(&in_buf, sizeof(uint8_t), 1, out);
    }

    fclose(in), fclose(out), fclose(key);
    return;
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
    printf("cmd = e:%d d:%d h:%d i:%d k:%d o:%d\n", cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], cmd[5]);
    printf("input_file = %s\n", input_file);
    printf("output_file = %s\n", output_file);
    printf("key_file = %s\n", key_file);
    if (cmd[HELP]) return 0;
    if (isInvalid(cmd, input_file, output_file, key_file))
        return -1;

    if (cmd[ENCRYPT]) enFile(input_file, output_file, key_file);
    if (cmd[DECRYPT]) deFile(input_file, output_file, key_file);
    return 0;
}
