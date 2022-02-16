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
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
tree(char *path, int level)
{
  int i;
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  fprintf(2, "%s", fmtname(path));

  if((fd = open(path, 0)) < 0){
    fprintf(2, " [error opening dir]\n");
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, " [error opening dir]\n");
    close(fd);
    return;
  }

  if(st.type != T_DIR){
    fprintf(2, " [error opening dir]\n");
    close(fd);
    return;
  }
  fprintf(2, "\n");

  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    stat(buf, &st);

    if(st.type == T_DIR || st.type == T_FILE){
      for(i=0; i<level; i++){
        fprintf(2, "|   ");
      }
      fprintf(2, "|\n");

      for(i=0; i<level; i++){
        fprintf(2, "|   ");
      }
      fprintf(2, "+-- ");
    }

    if(st.type == T_DIR){
      tree(buf, level+1);
    }
    else if(st.type == T_FILE){
      fprintf(2, "%s\n", fmtname(buf));
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  // add your code!
  tree(argv[1], 0);
  exit(0);
}
