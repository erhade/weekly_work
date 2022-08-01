#ifndef PROTOCOL_H__
#define PROTOCOL_H__

#include <stdint.h>

#define NAME_SIZE       11
#define DOMAIN          AF_INET
#define TYPE            SOCK_DGRAM
#define ADDR            "0.0.0.0"
#define PORT            "1989"          

typedef struct msg_st
{
    uint8_t name[NAME_SIZE];
    uint32_t math;
    uint32_t chinese;
}msg_t;



#endif