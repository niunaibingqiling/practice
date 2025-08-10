#include <stdio.h>
int main() {
    FILE *file = fopen("output.txt", "w+");  // 以读写方式打开文件

    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // 写入 1 到第 1~5 行
    for (int i = 1; i <= 5; i++) {
        fprintf(file, "1\n");
    }

    // 现在文件指针在第 6 行（即第 5 行末尾的换行符之后）
    // 我们要回退到第 5 行的开头，把 '1' 改成 '0'

    // 计算第 5 行的起始位置：
    // 每行是 "1\n"（2 字节），前 4 行占 8 字节，第 5 行从第 9 字节开始
    fseek(file, -3, SEEK_END);  // 移动到第 5 行的开头（0-based 计算）

    // 覆盖写入 '0'（替换原来的 '1'）
    fprintf(file, "0\n");
    for (int i = 1; i <= 5; i++) {
        fprintf(file, "1\n");
    }

    // // 文件指针现在在第 5 行的第 2 个字符（'\n' 之前）
    // // 如果想继续在第 6 行写入，可以再移动指针到末尾
    // fseek(file, 0, SEEK_END);  // 移动到文件末尾

    // 继续写入（可选）
    // fprintf(file, "New content after modification\n");

    fclose(file);  // 关闭文件
    return 0;
}