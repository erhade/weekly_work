#include <stdio.h>
#include <stdlib.h>

// tmpfile创建临时文件 匿名文件不会导致文件名冲突 fclose之后即销毁
// 不及时fclose会导致内存消耗 磁盘消耗 以及容易达到打开文件个数上线

int main(int argc, char const *argv[])
{
    FILE *file = tmpfile();
    fclose(file);
    exit(0);
}
