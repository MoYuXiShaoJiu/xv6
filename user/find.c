
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, const char *target) 
{
  char buf[512], *p;
  int fd;//保存文件
  struct dirent de; // 目录项结构体
  struct stat st; // 目录项参数

  if((fd = open(path, 0)) < 0)
  { // 打开该路径
    fprintf(2, "ls: cannot open %s\n", path);//如果没有找到路径
    return;
  }

  if(fstat(fd, &st) < 0)
  { // 获取当前path的参数 
    fprintf(2, "ls: cannot stat %s\n", path);//不能获取参数
    close(fd);
    return;
  }

  switch(st.type)
  {
  case T_FILE: // 如果当前path是一个文件
    if (strcmp(target, path + strlen(path) - strlen(target)) == 0) 
    {
      printf("%s\n", path);
    }
    break;

  case T_DIR: // 如果当前path是一个目录
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de))
    { // 读取当前目录下所有的目录项
      if(de.inum == 0) // 若不排除inum为0会出现很多异常输出，暂不清楚inum为0代表的含义
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue; // 排除.以及..
      strcpy(p, de.name); // 拼接新路径
      find(buf, target); // 对于子目录项递归执行find
    }
    break;
  }
  close(fd);
}

int main (int argc, char *argv[]) 
{
  if (argc != 3) {
    printf("usage: find <path> <file name>\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
