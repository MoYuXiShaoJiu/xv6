#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) 
{
  if (argc < 2) 
  {
    printf("xargs <command>\n");
    exit(1);
  }

  // 添加命令运行参数的二维数组
  char* commandArgv[32];
  int commandSize = argc - 1;
  // 将原本argv中的参数拷贝到新的参数数组中
  for (int i = 0; i < commandSize; ++i)
   {
    commandArgv[i] = argv[i + 1];
  }

  char inputBuffer[512]; // 输入缓冲
  char inputChar; // 输入字符
  int inputNum = 0; // 输入字符计数
  while (read(0, &inputChar, sizeof(char)) > 0) 
  {
    if (inputChar == '\n') 
    { // 遇到回车时执行
      // 将当前指令添加到运行参数的二维数组中
      inputBuffer[inputNum] = 0; // 在字符数组最后添加'\0'
      commandArgv[commandSize++] = inputBuffer; // 将标准输入获取的参数拼接到参数数组中 
      commandArgv[commandSize] = 0; // 添加参数数组的结尾
      if (fork() == 0) 
      {
        exec(argv[1], commandArgv); // 子进程中执行目标命令
      }
      wait(0); // 等待子进程命令执行结束
      commandSize = argc - 1; // 初始化，保留argv中参数，准备执行下一行
      inputNum = 0;
    }
     else if (inputChar == ' ') 
     {
      inputBuffer[inputNum++] = 0; // 遇到空格添加分割符
    } 
    else 
    {
      inputBuffer[inputNum++] = inputChar; // 正常字符输入
    }
    
  }
  exit(0);
}
