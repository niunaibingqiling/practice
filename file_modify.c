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
    // 每行是 "1\n\r"(3 字节)
    fseek(file, -3, SEEK_END);  // 移动到第 5 行的开头（0-based 计算）

    // 覆盖写入 '0'（替换原来的 '1'）
    fprintf(file, "0\n\r");
    for (int i = 1; i <= 5; i++) {
        fprintf(file, "1\n");
    }

    fclose(file);  // 关闭文件
    return 0;
}
