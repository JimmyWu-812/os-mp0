#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "errno.h"

int numFile = 0, numDir = 0;
char hasBar[5] = "00000";

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
tree(char *path, int level)
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
    // printf("====================\n");
    // printf("de1.name: %s\n", de1.name);
    // printf("de2.name: %s\n", de2.name);
    // printf("de1.inum: %d\n", de1.inum);
    // printf("de2.inum: %d\n", de2.inum);
    if(de1.inum == 0 || !strcmp(de1.name, ".") || !strcmp(de1.name, "..")){
      r1 = r2;
      de1 = de2;
      r2 = read(fd, &de2, sizeof(de2));
      continue;
    }
    if(!strcmp(de2.name, de1.name)){
      // printf("last item\n");
      hasBar[level] = '1';
    }
    // printf("de.name: -%s-\n", de1.name);
    memmove(p, de1.name, DIRSIZ);
    p[DIRSIZ] = 0;
    stat(buf, &st);
    if(st.type == T_DIR || st.type == T_FILE){
      for(i=0; i<level; i++){
        // printf("i: %d\n", i);
        // printf("hasBar[i]: %c\n", hasBar[i]);
        if(hasBar[i] == '0'){
          fprintf(2, "|   ");
        }
        else if(hasBar[i] == '1'){
          fprintf(2, "    ");
        }
      }
      fprintf(2, "|\n");
      for(i=0; i<level; i++){
        if(hasBar[i] == '0'){
          fprintf(2, "|   ");
        }
        else if(hasBar[i] == '1'){
          fprintf(2, "    ");
        }
      }
      fprintf(2, "+-- ");
    }
    if(st.type == T_DIR){
      numDir++;
      tree(buf, level+1);
    }
    else if(st.type == T_FILE){
      numFile++;
      fprintf(2, "%s\n", fmtname(buf));
    }
    if(!strcmp(de2.name, de1.name)){
      hasBar[level] = '0';
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
  //     tree(buf, level+1, hasBar);
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
  int numFileParent, numDirParent;
  int fd[2];
  pipe(fd);
  int p = fork();
  if(p > 0){
    read(fd[0], &numFileParent, sizeof(int));
    read(fd[0], &numDirParent, sizeof(int));
    fprintf(2, "\n%d directories, %d files\n", numDirParent, numFileParent);
  }
  else{
    tree(argv[1], 0);
    write(fd[1], &numFile, sizeof(int));
    write(fd[1], &numDir, sizeof(int));
  }
  close(fd[0]);
  close(fd[1]);
  exit(0);
}
