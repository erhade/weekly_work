#ifndef PROTOCOL_H__
#define PROTOCOL_H__

#define KEY_PATHNAME    "/etc/services"
#define KEY_PROJID      'g'
#define NAME_SIZE       20

typedef struct msg_st
{
    long mtype;
    char name[NAME_SIZE];
    int math;
    int chinese;
}msg_st;

#endif