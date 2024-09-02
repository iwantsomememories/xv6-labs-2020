#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *file_name){
    int fd;
    struct dirent de;
    struct stat st;
    char buf[512], *p;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }

        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de)) 
        {
            if (de.inum == 0)
            {
                continue;
            }
            
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("find: cannot stat %s\n", buf);
                continue;
            }

            if (strcmp(p, file_name) == 0)
            {
                printf("%s\n", buf);
                // 不在“.”和“..”目录中递归
            } else if (st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0)
            {
                find(buf, file_name);
            }
        }
        break;
    default:
        // find的第一个参数必须是目录
        fprintf(2, "usage: find <directory> <filename>\n");
        break;
    }

    close(fd);
}


int 
main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "usage: find <directory> <filename>\n");
        exit(1);
    }

    char *path = argv[1];
    char *file_name = argv[2];
    find(path, file_name);
    exit(0);
}
