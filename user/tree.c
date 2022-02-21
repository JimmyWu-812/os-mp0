#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\0', DIRSIZ-strlen(p));
  return buf;
}

void
tree(char *path, int level, char *bar)
{
  int i, r1, r2;
  char buf[512], *p;
  int fd;
  struct dirent de1, de2;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "%s [error opening dir]\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "%s [error opening dir]\n", path);
    close(fd);
    return;
  }
  if(st.type != T_DIR){
    fprintf(2, "%s [error opening dir]\n", path);
    close(fd);
    return;
  }
  if(level == 0){
    fprintf(2, "%s\n", path);
  }
  else{
    fprintf(2, "%s\n", fmtname(path));
  }
  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';
  r1 = read(fd, &de1, sizeof(de1));
  r2 = read(fd, &de2, sizeof(de2));
  while(r1 == sizeof(de1)){
    printf("de2.name: %s\n", de2.name);
    if(de1.inum == 0 || !strcmp(de1.name, ".") || !strcmp(de1.name, "..")){
      r1 = r2;
      de1 = de2;
      r2 = read(fd, &de2, sizeof(de2));
      continue;
    }
    if(!strcmp(de2.name, "")){
      printf("xxx\n");
      bar[level] = '1';
    }
    // printf("de.name: -%s-\n", de1.name);
    memmove(p, de1.name, DIRSIZ);
    p[DIRSIZ] = 0;
    stat(buf, &st);
    if(st.type == T_DIR || st.type == T_FILE){
      for(i=0; i<level; i++){
        // printf("i: %d\n", i);
        // printf("bar[i]: %c\n", bar[i]);
        if(bar[i] == '0'){
          fprintf(2, "|   ");
        }
        else{
          fprintf(2, "    ");
        }
      }
      fprintf(2, "|\n");
      for(i=0; i<level; i++){
        if(bar[i] == '0'){
          fprintf(2, "|   ");
        }
        else{
          fprintf(2, "    ");
        }
      }
      fprintf(2, "+-- ");
    }
    if(st.type == T_DIR){
      tree(buf, level+1, bar);
    }
    else if(st.type == T_FILE){
      fprintf(2, "%s\n", fmtname(buf));
    }
    if(!strcmp(de2.name, "")){
      bar[level] = '0';
    }
    r1 = r2;
    de1 = de2;
    r2 = read(fd, &de2, sizeof(de2));
  }
  // while(read(fd, &de, sizeof(de)) == sizeof(de)){
  //   printf("de.name: %s\n", de.name);
  //   if(de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
  //     continue;
  //   memmove(p, de.name, DIRSIZ);
  //   p[DIRSIZ] = 0;
  //   stat(buf, &st);
  //   if(st.type == T_DIR || st.type == T_FILE){
  //     for(i=0; i<level; i++){
  //       fprintf(2, "|   ");
  //     }
  //     fprintf(2, "|\n");
  //     for(i=0; i<level; i++){
  //       fprintf(2, "|   ");
  //     }
  //     fprintf(2, "+-- ");
  //   }
  //   if(st.type == T_DIR){
  //     tree(buf, level+1, bar);
  //   }
  //   else if(st.type == T_FILE){
  //     fprintf(2, "%s\n", fmtname(buf));
  //   }
  // }
  close(fd);
}

int
main(int argc, char *argv[])
{
  // add your code!

  char bar[5] = {'0'};
  tree(argv[1], 0, bar);
  exit(0);
}
