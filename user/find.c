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
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("find: cannot stat %s\n", buf);
                continue;
            }

            if (strcmp(de.name, file_name) == 0)
            {
                printf("%s\n", buf);
            } else if (st.type == T_DIR)
            {
                char sub_path[512];
                memmove(sub_path, buf, strlen(buf));
                find(sub_path, file_name);
            }
        }
        break;
    default:
        fprintf(2, "find: path must be a directory");
        break;
    }

    close(fd);
}


int 
main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "Usage: find path pattern");
        exit(1);
    }

    char *path = argv[1];
    char *file_name = argv[2];
    find(path, file_name);
    return 0;
}
