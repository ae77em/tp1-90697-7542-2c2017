#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

#include <stdint.h>
#include <stdlib.h>

#define OPCODE_INSERT       1
#define OPCODE_DELETE       2
#define OPCODE_SPACE        3
#define OPCODE_NEWLINE      4
#define OPCODE_PRINT        5

struct insert_command_t {
    int opcode;
    int pos;
    short size;
    char *text;
} __attribute__((packed));

struct delete_command_t {
    int opcode;
    int from;
    int to;
} __attribute__((packed));

struct space_command_t {
    int opcode;
    int pos;    
} __attribute__((packed));

struct newline_command_t {
    int opcode;
    int pos;    
} __attribute__((packed));

struct print_command_t {
    int opcode;    
} __attribute__((packed));

struct response_command_t {
    int opcode;
    char *text;
} __attribute__((packed));

#endif /* COMMON_STRUCTS_H */

