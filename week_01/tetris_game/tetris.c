#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#define BIG_BOX_ROW_LEN    20
#define BIG_BOX_COLUMN_LEN    10
#define SMALL_BOX_ROW_LEN  4
#define SMALL_BOX_COLUMN_LEN  4

int arr[BIG_BOX_ROW_LEN][BIG_BOX_COLUMN_LEN];
int graph_num;
int next_graph_num;
int row_pos = 0;
int column_pos = 4;
int level = 0;
int score = 0;

enum 
{
    SQUARE = 1,
    OBLONG,
    OBLONG_90,
    POSITIVE_Z,
    POSITIVE_Z_90,
    NEGATIVE_Z,
    NEGATIVE_Z_90,
    POSITIVE_7,
    POSITIVE_7_90,
    POSITIVE_7_180,
    POSITIVE_7_270,
    NEGATIVE_7,
    NEGATIVE_7_90,
    NEGATIVE_7_180,
    NEGATIVE_7_270,
    CONVEX,
    CONVEX_90,
    CONVEX_180,
    CONVEX_270,
    NONE,
};

enum
{
    INSERT,
    CLEAR,
};

static void box_insert(void *_arr, int graph_num, int row_offset, int column_offset, int BOX_LEN, int mode)
{
    int (*arr)[BOX_LEN] = _arr;
    switch (graph_num)
    {
    case SQUARE:
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                arr[row_offset+i][column_offset+j] = mode == CLEAR ? 0 : SQUARE;       
            }
        }
        break;
    case OBLONG:
        for (int j = 0; j < 4; j++)
        {
            arr[row_offset][column_offset+j] = mode == CLEAR ? 0 : OBLONG;
        }
        break;
    case OBLONG_90:
        for (int i = 0; i < 4; i++)
        {
            arr[row_offset+i][column_offset] = mode == CLEAR ? 0 : OBLONG_90;
        }
        break;
    case POSITIVE_Z:
        arr[row_offset][column_offset] = mode == CLEAR ? 0 : POSITIVE_Z;
        arr[row_offset][column_offset+1] = mode == CLEAR ? 0 : POSITIVE_Z;
        arr[row_offset+1][column_offset+1] = mode == CLEAR ? 0 : POSITIVE_Z;
        arr[row_offset+1][column_offset+2] = mode == CLEAR ? 0 : POSITIVE_Z;
        break;
    case POSITIVE_Z_90:
        arr[row_offset][column_offset+1] = mode == CLEAR ? 0 : POSITIVE_Z_90;
        arr[row_offset+1][column_offset] = mode == CLEAR ? 0 : POSITIVE_Z_90;
        arr[row_offset+1][column_offset+1] = mode == CLEAR ? 0 : POSITIVE_Z_90;
        arr[row_offset+2][column_offset] = mode == CLEAR ? 0 : POSITIVE_Z_90;
        break;
    case NEGATIVE_Z:
        arr[row_offset][column_offset+1] = mode == CLEAR ? 0 : NEGATIVE_Z;
        arr[row_offset][column_offset+2] = mode == CLEAR ? 0 : NEGATIVE_Z;
        arr[row_offset+1][column_offset] = mode == CLEAR ? 0 : NEGATIVE_Z;
        arr[row_offset+1][column_offset+1] = mode == CLEAR ? 0 : NEGATIVE_Z;
        break;
    case NEGATIVE_Z_90:
        arr[row_offset][column_offset] = mode == CLEAR ? 0 : NEGATIVE_Z_90;
        arr[row_offset+1][column_offset] = mode == CLEAR ? 0 : NEGATIVE_Z_90;
        arr[row_offset+1][column_offset+1] = mode == CLEAR ? 0 : NEGATIVE_Z_90;
        arr[row_offset+2][column_offset+1] = mode == CLEAR ? 0 : NEGATIVE_Z_90;
        break;
    case POSITIVE_7:
        arr[row_offset][column_offset] = mode == CLEAR ? 0 : POSITIVE_7;
        for (int i = 0; i < 3; i++)
        {
            arr[row_offset+i][column_offset+1] = mode == CLEAR ? 0 : POSITIVE_7;
        }
        
        break;
    case POSITIVE_7_90:
        for (int j = 0; j < 3; j++)
        {
            arr[row_offset][column_offset+j] = mode == CLEAR ? 0 : POSITIVE_7_90;
        }
        arr[row_offset+1][column_offset] = mode == CLEAR ? 0 : POSITIVE_7_90;
        break;
    case POSITIVE_7_180:
        for (int i = 0; i < 3; i++)
        {
            arr[row_offset+i][column_offset] = mode == CLEAR ? 0 : POSITIVE_7_180;
        }
        arr[row_offset+2][column_offset+1] = mode == CLEAR ? 0 : POSITIVE_7_180;
        break;
    case POSITIVE_7_270:
        arr[row_offset][column_offset+2] = mode == CLEAR ? 0 : POSITIVE_7_270;
        for (int j = 0; j < 3; j++)
        {
            arr[row_offset+1][column_offset+j] = mode == CLEAR ? 0 : POSITIVE_7_270;
        }
        break;
    case NEGATIVE_7:
        for (int i = 0; i < 3; i++)
        {
            arr[row_offset+i][column_offset] = mode == CLEAR ? 0 : NEGATIVE_7;
        }
        arr[row_offset][column_offset+1] = mode == CLEAR ? 0 : NEGATIVE_7;
        break;
    case NEGATIVE_7_90:
        arr[row_offset][column_offset] = mode == CLEAR ? 0 : NEGATIVE_7_90;
        for (int j = 0; j < 3; j++)
        {
            arr[row_offset+1][column_offset+j] = mode == CLEAR ? 0 : NEGATIVE_7_90;
        }
        break;
    case NEGATIVE_7_180:
        for (int i = 0; i < 3; i++)
        {
            arr[row_offset+i][column_offset+1] = mode == CLEAR ? 0 : NEGATIVE_7_180;
        }
        arr[row_offset+2][column_offset] = mode == CLEAR ? 0 : NEGATIVE_7_180;
        break;
    case NEGATIVE_7_270:
        for (int j = 0; j < 3; j++)
        {
            arr[row_offset][column_offset+j] = mode == CLEAR ? 0 : NEGATIVE_7_270;
        }
        arr[row_offset+1][column_offset+2] = mode == CLEAR ? 0 : NEGATIVE_7_270;
        break;
    case CONVEX:
        arr[row_offset][column_offset+1] = mode == CLEAR ? 0 : CONVEX;
        for (int j = 0; j < 3; j++)
        {
            arr[row_offset+1][column_offset+j] = mode == CLEAR ? 0 : CONVEX;
        }
        break;
    case CONVEX_90:
        arr[row_offset+1][column_offset] = mode == CLEAR ? 0 : CONVEX_90;
        for (int i = 0; i < 3; i++)
        {
            arr[row_offset+i][column_offset+1] = mode == CLEAR ? 0 : CONVEX_90;
        }
        break;
    case CONVEX_180:
        for (int j = 0; j < 3; j++)
        {
            arr[row_offset][column_offset+j] = mode == CLEAR ? 0 : CONVEX_180;
        }
        arr[row_offset+1][column_offset+1] = mode == CLEAR ? 0 : CONVEX_180;
        break;
    case CONVEX_270:
        for (int i = 0; i < 3; i++)
        {
            arr[row_offset+i][column_offset] = mode == CLEAR ? 0 : CONVEX_270;
        }
        arr[row_offset+1][column_offset+1] = mode == CLEAR ? 0 : CONVEX_270;
        break;
    default:
        break;
    }
    return;
}

static void draw_graph(void *_arr, int row, int column, int row_len, int column_len)
{
    int (*arr)[column_len] = _arr;
    for (int i = 0; i < row_len; i++)
    {
        for (int j = 0; j < column_len; j++)
        {
            switch (arr[i][j])
            {
            case SQUARE:
                printf("\033[%d;%dH\033[%dm[]\033[0m", i + row, j * 2 + column, 41);
                break;
            case OBLONG:
            case OBLONG_90:
                printf("\033[%d;%dH\033[%dm[]\033[0m", i + row, j * 2  + column, 42);
                break;
            case POSITIVE_Z:
            case POSITIVE_Z_90:
                printf("\033[%d;%dH\033[%dm[]\033[0m", i + row, j * 2  + column, 43);
                break;
            case NEGATIVE_Z:
            case NEGATIVE_Z_90:
                printf("\033[%d;%dH\033[%dm[]\033[0m", i + row, j * 2  + column, 44);
                break;
            case POSITIVE_7:
            case POSITIVE_7_90:
            case POSITIVE_7_180:
            case POSITIVE_7_270:
                printf("\033[%d;%dH\033[%dm[]\033[0m", i + row, j * 2  + column, 45);
                break;
            case NEGATIVE_7:
            case NEGATIVE_7_90:
            case NEGATIVE_7_180:
            case NEGATIVE_7_270:
                printf("\033[%d;%dH\033[%dm[]\033[0m", i + row, j * 2  + column, 46);
                break;
            case CONVEX:
            case CONVEX_90:
            case CONVEX_180:
            case CONVEX_270:
                printf("\033[%d;%dH\033[%dm[]\033[0m", i + row, j * 2  + column, 47);
                break;
            default:
                break;
            }
        }
    }
    return;
}

static void frame_show(int graph_num, int score, int level)
{
    int arr[SMALL_BOX_ROW_LEN][SMALL_BOX_COLUMN_LEN];
    memset(arr, 0, sizeof(arr));
    printf("\033[3;21Henter 'W' to rotate");
    printf("\033[4;21Henter 'S' to move down");
    printf("\033[5;21Henter 'A' to move left");
    printf("\033[6;21Henter 'D' to move right");
    printf("\033[7;21Henter 'Q' to quit");
    printf("\033[10;20H\033[46m--====================--");
    for (int i = 0; i < 20; i++)
    {
        printf("\033[%d;20H||\033[%d;42H||", (11 + i), (11 + i));
    }
    printf("\033[31;20H--====================--\033[0m");
    box_insert(arr, graph_num, 0, 0, SMALL_BOX_COLUMN_LEN, INSERT);
    draw_graph(arr, 11, 50, SMALL_BOX_ROW_LEN, SMALL_BOX_COLUMN_LEN);
    printf("\033[23;50HSCORE");
    printf("\033[25;52H%d", score);
    printf("\033[29;50HLEVEL");
    printf("\033[31;52H%d", level);
    return;
}

static int can_down_one(void *_arr, int graph_num, int row_offset, int column_offset, int BOX_LEN)
{
    int (*arr)[BOX_LEN] = _arr;
    switch (graph_num)
    {
    case SQUARE:
        if ((row_offset + 2) >= BIG_BOX_ROW_LEN || arr[row_offset+2][column_offset] != 0 
        || arr[row_offset+2][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case OBLONG:
        if ((row_offset + 1) >= BIG_BOX_ROW_LEN || arr[row_offset+1][column_offset] != 0 || arr[row_offset+1][column_offset+1] != 0 
        || arr[row_offset+1][column_offset+2] != 0 || arr[row_offset+1][column_offset+3] != 0)
            return 0;
        else
            return 1;
        break;
    case OBLONG_90:
        if ((row_offset + 4) >= BIG_BOX_ROW_LEN || arr[row_offset+4][column_offset] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_Z:
        if ((row_offset + 2) >= BIG_BOX_ROW_LEN || arr[row_offset+1][column_offset] != 0 || 
        arr[row_offset+2][column_offset+1] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_Z_90:
        if ((row_offset + 3) >= BIG_BOX_ROW_LEN || arr[row_offset+3][column_offset] != 0 
        || arr[row_offset+2][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_Z:
        if ((row_offset + 2) >= BIG_BOX_ROW_LEN || arr[row_offset+2][column_offset] != 0 
        || arr[row_offset+2][column_offset+1] != 0 || arr[row_offset+1][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_Z_90:
        if ((row_offset + 3) >= BIG_BOX_ROW_LEN || arr[row_offset+2][column_offset] != 0 
        || arr[row_offset+3][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7:
        if ((row_offset + 3) >= BIG_BOX_ROW_LEN || arr[row_offset+1][column_offset] != 0 
        || arr[row_offset+3][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7_90:
        if ((row_offset + 2) >= BIG_BOX_ROW_LEN || arr[row_offset+2][column_offset] != 0 
        || arr[row_offset+1][column_offset+1] != 0 || arr[row_offset+1][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7_180:
        if ((row_offset + 3) >= BIG_BOX_ROW_LEN || arr[row_offset+3][column_offset] != 0 
        || arr[row_offset+3][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7_270:
        if ((row_offset + 2) >= BIG_BOX_ROW_LEN || arr[row_offset+2][column_offset] != 0 
        || arr[row_offset+2][column_offset+1] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7:
        if ((row_offset + 3) >= BIG_BOX_ROW_LEN || arr[row_offset+3][column_offset] != 0 
        || arr[row_offset+1][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7_90:
        if ((row_offset + 2) >= BIG_BOX_ROW_LEN || arr[row_offset+2][column_offset] != 0 
        || arr[row_offset+2][column_offset+1] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7_180:
        if ((row_offset + 3) >= BIG_BOX_ROW_LEN || arr[row_offset+3][column_offset] != 0 
        || arr[row_offset+3][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7_270:
        if ((row_offset + 2) >= BIG_BOX_ROW_LEN || arr[row_offset+1][column_offset] != 0 
        || arr[row_offset+1][column_offset+1] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX:
        if ((row_offset + 2) >= BIG_BOX_ROW_LEN || arr[row_offset+2][column_offset] != 0 
        || arr[row_offset+2][column_offset+1] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX_90:
        if ((row_offset + 3) >= BIG_BOX_ROW_LEN || arr[row_offset+2][column_offset] != 0 
        || arr[row_offset+3][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX_180:
        if ((row_offset + 2) >= BIG_BOX_ROW_LEN || arr[row_offset+1][column_offset] != 0 
        || arr[row_offset+1][column_offset+2] != 0 || arr[row_offset+2][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX_270:
        if ((row_offset + 3) >= BIG_BOX_ROW_LEN || arr[row_offset+3][column_offset] != 0 
        || arr[row_offset+2][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    default:
        break;
    }
    return -1;
}

static int can_left_one(void *_arr, int graph_num, int row_offset, int column_offset, int BOX_LEN)
{
    int (*arr)[BOX_LEN] = _arr;
    if (graph_num == NONE)
        return NONE;
    if (column_offset == 0)
        return 0;
    switch (graph_num)
    {
    case SQUARE:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case OBLONG:
        if (arr[row_offset][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case OBLONG_90:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset-1] != 0
        || arr[row_offset+2][column_offset-1] != 0 || arr[row_offset+3][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_Z:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_Z_90:
        if (arr[row_offset][column_offset] != 0 || arr[row_offset+1][column_offset-1] != 0 
        || arr[row_offset+2][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_Z:
        if (arr[row_offset][column_offset] != 0 || arr[row_offset+1][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_Z_90:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset-1] != 0 
        || arr[row_offset+2][column_offset] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset] != 0
        || arr[row_offset+2][column_offset] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7_90:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7_180:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset-1] != 0
        || arr[row_offset+2][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7_270:
        if (arr[row_offset][column_offset+1] != 0 || arr[row_offset+1][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset-1] != 0
        || arr[row_offset+2][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7_90:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7_180:
        if (arr[row_offset][column_offset] != 0 || arr[row_offset+1][column_offset] != 0
        || arr[row_offset+2][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7_270:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX:
        if (arr[row_offset][column_offset] != 0 || arr[row_offset+1][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX_90:
        if (arr[row_offset][column_offset] != 0 || arr[row_offset+1][column_offset-1] != 0
        || arr[row_offset+2][column_offset] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX_180:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX_270:
        if (arr[row_offset][column_offset-1] != 0 || arr[row_offset+1][column_offset-1] != 0 
        || arr[row_offset+2][column_offset-1] != 0)
            return 0;
        else
            return 1;
        break;
    default:
        break;
    }
    return -1;
}

static int can_right_one(void *_arr, int graph_num, int row_offset, int column_offset, int BOX_LEN)
{
    int (*arr)[BOX_LEN] = _arr;
    if (graph_num == NONE)
        return NONE;
    switch (graph_num)
    {
    case SQUARE:
        if ((column_offset + 2) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+2] != 0 
        || arr[row_offset+1][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case OBLONG:
        if ((column_offset + 4) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+4] != 0)
            return 0;
        else
            return 1;
        break;
    case OBLONG_90:
        if ((column_offset + 1) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+1] != 0
        || arr[row_offset+1][column_offset+1] != 0 || arr[row_offset+2][column_offset+1] != 0
        || arr[row_offset+3][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_Z:
        if ((column_offset + 3) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+2] != 0 || 
        arr[row_offset+1][column_offset+3] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_Z_90:
        if ((column_offset + 2) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+2] != 0 
        || arr[row_offset+1][column_offset+2] != 0 || arr[row_offset+2][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_Z:
        if ((column_offset + 3) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+3] != 0 
        || arr[row_offset+1][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_Z_90:
        if ((column_offset + 2) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+1] != 0 
        || arr[row_offset+1][column_offset+2] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7:
        if ((column_offset + 2) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+2] != 0 
        || arr[row_offset+1][column_offset+2] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7_90:
        if ((column_offset + 3) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+3] != 0 
        || arr[row_offset+1][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7_180:
        if ((column_offset + 2) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+1] != 0 
        || arr[row_offset+1][column_offset+1] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case POSITIVE_7_270:
        if ((column_offset + 3) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+3] != 0 
        || arr[row_offset+1][column_offset+3] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7:
        if ((column_offset + 2) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+2] != 0 
        || arr[row_offset+1][column_offset+1] != 0 || arr[row_offset+2][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7_90:
        if ((column_offset + 3) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+1] != 0 
        || arr[row_offset+1][column_offset+3] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7_180:
        if ((column_offset + 2) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+2] != 0 
        || arr[row_offset+1][column_offset+2] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case NEGATIVE_7_270:
        if ((column_offset + 3) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+3] != 0 
        || arr[row_offset+1][column_offset+3] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX:
        if ((column_offset + 3) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+2] != 0 
        || arr[row_offset+1][column_offset+3] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX_90:
        if ((column_offset + 2) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+2] != 0 
        || arr[row_offset+1][column_offset+2] != 0 || arr[row_offset+2][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX_180:
        if ((column_offset + 3) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+3] != 0 
        || arr[row_offset+1][column_offset+2] != 0)
            return 0;
        else
            return 1;
        break;
    case CONVEX_270:
        if ((column_offset + 2) >= BIG_BOX_COLUMN_LEN || arr[row_offset][column_offset+1] != 0 
        || arr[row_offset+1][column_offset+2] != 0 || arr[row_offset+2][column_offset+1] != 0)
            return 0;
        else
            return 1;
        break;
    default:
        break;
    }
    return -1;
}

static void left(void)
{
    if (can_left_one(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN) == 1)
    {
        box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
        column_pos--;
        box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
    }
    return;
}

static void right(void)
{
    if (can_right_one(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN) == 1)
    {
        box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
        column_pos++;
        box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
    }
    return;
}

static void up(void)
{
    switch (graph_num)
    {
    case SQUARE:
        break;
    case OBLONG:
        if (row_pos != 0 && (row_pos + 3) <= BIG_BOX_ROW_LEN && arr[row_pos+1][column_pos+1] == 0
        && arr[row_pos+2][column_pos+1] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            row_pos--;
            column_pos++;
            graph_num = OBLONG_90;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case OBLONG_90:
        if (column_pos != 0 && (column_pos + 3) <= BIG_BOX_COLUMN_LEN 
        && arr[row_pos+1][column_pos-1] == 0 && arr[row_pos+1][column_pos+1] == 0 
        && arr[row_pos+1][column_pos+2] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            row_pos++;
            column_pos--;
            graph_num = OBLONG;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case POSITIVE_Z:
        if ((row_pos + 3) <= BIG_BOX_ROW_LEN && arr[row_pos+1][column_pos] == 0
        && arr[row_pos+2][column_pos] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            graph_num = POSITIVE_Z_90;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case POSITIVE_Z_90:
        if ((column_pos + 3) <= BIG_BOX_COLUMN_LEN && arr[row_pos][column_pos] == 0 
        && arr[row_pos+1][column_pos+2] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            graph_num = POSITIVE_Z;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case NEGATIVE_Z:
        if ((row_pos + 3) <= BIG_BOX_ROW_LEN && arr[row_pos][column_pos] == 0
        && arr[row_pos+2][column_pos+1] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            graph_num = NEGATIVE_Z_90;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case NEGATIVE_Z_90:
        if ((column_pos + 3) <= BIG_BOX_COLUMN_LEN && arr[row_pos][column_pos+1] == 0 
        && arr[row_pos][column_pos+2] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            graph_num = NEGATIVE_Z;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case POSITIVE_7:
        if (column_pos != 0 && arr[row_pos][column_pos-1] == 0 && arr[row_pos+1][column_pos-1] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            column_pos--;
            graph_num = POSITIVE_7_90;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case POSITIVE_7_90:
        if ((row_pos + 3) <= BIG_BOX_ROW_LEN && arr[row_pos+2][column_pos] == 0
        && arr[row_pos+2][column_pos+1] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            graph_num = POSITIVE_7_180;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case POSITIVE_7_180:
        if ((column_pos + 3) <= BIG_BOX_COLUMN_LEN && arr[row_pos+1][column_pos+2] == 0 
        && arr[row_pos+2][column_pos+2] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            row_pos++;
            graph_num = POSITIVE_7_270;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case POSITIVE_7_270:
        if (row_pos != 0 && arr[row_pos-1][column_pos+1] == 0
        && arr[row_pos-1][column_pos+2] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            row_pos--;
            column_pos++;
            graph_num = POSITIVE_7;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case NEGATIVE_7:
        if (column_pos != 0 && arr[row_pos+2][column_pos+1] == 0 && arr[row_pos+2][column_pos+2] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            row_pos++;
            graph_num = NEGATIVE_7_90;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case NEGATIVE_7_90:
        if (row_pos != 0 && arr[row_pos][column_pos+2] == 0 && arr[row_pos-1][column_pos+2] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            row_pos--;
            column_pos++;
            graph_num = NEGATIVE_7_180;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case NEGATIVE_7_180:
        if (column_pos != 0 && arr[row_pos][column_pos] == 0 && arr[row_pos][column_pos-1] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            column_pos--;
            graph_num = NEGATIVE_7_270;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case NEGATIVE_7_270:
        if ((row_pos + 3) <= BIG_BOX_ROW_LEN && arr[row_pos+1][column_pos] == 0
        && arr[row_pos+2][column_pos] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            graph_num = NEGATIVE_7;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case CONVEX:
        if ((row_pos + 3) <= BIG_BOX_ROW_LEN && arr[row_pos+2][column_pos+1] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            graph_num = CONVEX_90;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case CONVEX_90:
        if ((column_pos + 3) <= BIG_BOX_COLUMN_LEN && arr[row_pos+1][column_pos+2] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            row_pos++;
            graph_num = CONVEX_180;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case CONVEX_180:
        if (row_pos != 0 && arr[row_pos-1][column_pos+1] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            row_pos--;
            column_pos++;
            graph_num = CONVEX_270;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    case CONVEX_270:
        if (column_pos != 0 && arr[row_pos+1][column_pos-1] == 0)
        {
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
            column_pos--;
            graph_num = CONVEX;
            box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
        }
        break;
    default:
        break;
    }
    return;
}

static void down(void)
{
    while (can_down_one(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN) == 1)
    {
        box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
        row_pos++;
        box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
    }
    return;
}

void check_full(void)
{
    int row_full = 1;
    for (int i = 0; i < BIG_BOX_ROW_LEN; i++)
    {
        for (int j = 0; j < BIG_BOX_COLUMN_LEN; j++)
        {
            if (arr[i][j] == 0) 
                row_full = 0;
        }
        if (row_full == 1)
        {
            for (int k = i-1; k >= 0; k--)
            {
                for (int j = 0; j < BIG_BOX_COLUMN_LEN; j++)
                {
                    arr[k+1][j] = arr[k][j];
                }
            }
            for (int j = 0; j < BIG_BOX_COLUMN_LEN; j++)
            {
                arr[0][j] = 0;
            }
            score += 10;
        }
        row_full = 1;
    }
    return;
}

void alarm_handler(int s)
{
    if (can_down_one(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN) == 1)
    {
        box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, CLEAR);
        row_pos++;
        box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
    }
    else
    {
        check_full();
        if (row_pos == 0)
        {
            graph_num = NONE;
            return;
        }
        graph_num = next_graph_num;
        next_graph_num = rand() % 19 + 1; 
        row_pos = 0;
        column_pos = 4;
        box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);
    }
    printf("\033[2J");
    draw_graph(arr, 11, 22, BIG_BOX_ROW_LEN, BIG_BOX_COLUMN_LEN);
    frame_show(next_graph_num, score, level);
    fflush(NULL);
    alarm(1);
    return;
}

int main(int argc, char const *argv[])
{
    struct termios old, new;
    tcgetattr(0, &old);
    tcgetattr(0, &new);
    new.c_lflag = new.c_lflag & ~(ICANON | ECHO);
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new);

    srand((unsigned int)time(0));

    int ch;
    graph_num = rand() % 19 + 1;
    next_graph_num = rand() % 19 + 1;
    
    memset(arr, 0, sizeof(arr));
    box_insert(arr, graph_num, row_pos, column_pos, BIG_BOX_COLUMN_LEN, INSERT);

    signal(SIGALRM, alarm_handler);
    alarm(1);

    printf("\033[?25l");

    while (1)
    {
        printf("\033[2J");
        draw_graph(arr, 11, 22, BIG_BOX_ROW_LEN, BIG_BOX_COLUMN_LEN);
        frame_show(next_graph_num, score, level);
        fflush(NULL);
        ch = getchar();
        if (ch == 'q' || ch == 'Q')
            break;
        switch (ch)
        {
        case 'w':
        case 'W':
            up();
            break;
        case 's': 
        case 'S': 
            down();
            break;
        case 'a':
        case 'A':
            left(); 
            break;
        case 'd':
        case 'D':
            right();
            break;
        default:
            break;
        }
    }
    putchar('\n');
    printf("\33[?25h");

    tcsetattr(0, TCSANOW, &old);
    exit(0);
}