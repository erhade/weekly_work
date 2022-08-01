#ifndef PROTOCOL_H__
#define PROTOCOL_H__

#include <stdint.h>

#define NAME_MAX        (512-8-8)
#define DOMAIN          AF_INET
#define TYPE            SOCK_DGRAM
#define ADDR            "0.0.0.0"
#define PORT            "1989"          

typedef struct msg_st
{
    uint32_t math;
    uint32_t chinese;
    uint8_t name[1];
}msg_t;



#endif