#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

#include <stdint.h>
#include <stdlib.h>

#define OPCODE_INSERT       1
#define OPCODE_DELETE       2
#define OPCODE_SPACE        3
#define OPCODE_NEWLINE      4
#define OPCODE_PRINT        5

#define MAX_DATA_BUFFER     2048
#define _SIZE_OF_UINT32_    4
#define _SIZE_OF_SHORT_     2

struct insert_command_t {
    uint32_t msg_lenght;
    uint32_t opcode;
    uint32_t pos;
    short size;
    char *text;
} __attribute__((packed));

struct delete_command_t {
    uint32_t msg_lenght;
    uint32_t opcode;
    uint32_t from;
    uint32_t to;
} __attribute__((packed));

struct space_command_t {
    uint32_t msg_lenght;
    uint32_t opcode;
    uint32_t pos;    
} __attribute__((packed));

struct newline_command_t {
    uint32_t msg_lenght;
    uint32_t opcode;
    uint32_t pos;    
} __attribute__((packed));

struct print_command_t {
    uint32_t msg_lenght;
    uint32_t opcode;    
} __attribute__((packed));

struct response_command_t {
    uint32_t msg_lenght;
    short size;
    char *text;
} __attribute__((packed));

#endif /* COMMON_STRUCTS_H */

