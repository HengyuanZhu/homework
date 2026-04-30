#include<stdio.h>
int main()
{
    char input[100];
    printf("请从键盘输入任意内容，不超过100个字符！\n");
    fgets(input,101,stdin);
    printf("你输出的内容为：\n%s",input);
    return 0;
}