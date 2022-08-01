#ifndef PROTOCOL_H__
#define PROTOCOL_H__

#define KEY_PATHNAME    "/etc/services"
#define KEY_PROJID      'a'
#define PATHMAX         1024
#define DATAMAX         1024

enum
{
    MSG_PATH = 1,
    MSG_DATA,
    MSG_EOT
};

typedef struct msg_path_st
{
    long mtype;
    char path[PATHMAX];
}msg_path_t;

typedef struct msg_data_st
{
    long mtype;
    int datalen;
    char data[DATAMAX];
}msg_data_t;

typedef struct msg_eot_st
{
    long mtype;

}msg_eot_st;

typedef union msg_s2c_st
{
    long mtype;
    msg_data_st msg_data;
    msg_eot_st msg_eot;
}msg_s2c_st;

#endif