#include "kernel/types.h"

#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "user/user.h"
void mkfile(char *filename) {
    int fd = open(filename, O_CREATE | O_RDWR);
    write(fd, "hi", 3);
    close(fd);
}

void mkd(char *dirname) {
    if (mkdir(dirname) < 0) {
        fprintf(2, "mkdir %s failed.", dirname);
        exit(1);
    }
}
// void test0() {
//     mkd("os2022");

//     mkd("os2022/d1");
//     mkd("os2022/d2");
//     mkd("os2022/d3");

//     mkd("os2022/d2/a");
//     mkd("os2022/d2/b");
//     mkfile("os2022/d2/c.txt");
//     mkd("os2022/d2/d");

//     mkd("os2022/d2/d/1");
//     mkfile("os2022/d2/d/2.txt");
//     mkd("os2022/d2/d/3");

//     mkfile("os2022/d2/d/1/11.txt");
//     mkfile("os2022/d2/d/1/12.txt");
//     mkd("os2022/d2/d/1/13");

//     mkd("os2022/d2/b/x");
//     mkd("os2022/d2/b/y");
//     mkd("os2022/d2/b/z");

//     mkd("os2022/d2/b/y/p");
//     mkfile("os2022/d2/b/y/q.txt");
//     mkd("os2022/d2/b/y/r");
//     mkfile("os2022/d2/b/y/s.txt");

//     mkfile("os2022/d2/b/z/1.txt");

//     mkd("os2022/d3/a");
//     mkfile("os2022/d3/b.txt");
// }
void test0() {
    mkd("os2022");

    mkd("os2022/d1");
    mkd("os2022/d2");
    mkd("os2022/d3");

    mkd("os2022/d2/a");
    mkd("os2022/d2/b");
    mkfile("os2022/d2/c");

    mkd("os2022/d3/a");
    mkfile("os2022/d3/b");
}

void test1() {
    mkd("a");
    mkd("a/0");
    mkd("a/1");
    mkd("a/2");
    mkd("a/3");
    mkd("a/4");
    mkd("a/5");
    mkd("a/6");
    mkd("a/7");
    mkd("a/8");
    mkd("a/9");
    mkd("a/10");
}

int main(int argc, char *argv[]) {
    test0();
    test1();
    exit(0);
}
