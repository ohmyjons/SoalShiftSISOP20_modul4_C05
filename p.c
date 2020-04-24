#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>

static  const  char *dirpath = "/home/iqbalhumam73/Downloads";

static  int  xmp_getattr(const char *path, struct stat *stbuf){
    int res;
    char fpath[1000];
    printf ("dirpath : %s\n path : %s\n", dirpath, path);
    sprintf(fpath,"%s%s",dirpath,path);
    res = lstat(fpath, stbuf);
    if (res == -1){
        // printf ("dirpath : %s\n path : %s\n", dirpath, path);
        return -errno;
    }
    return 0;
}

static int xmp_rename(const char *from, const char *to) {
	int res;
    char fpath[1000];
    if (!strcmp(from, "/")) {
        from = dirpath;
        sprintf(fpath, "%s", from);
    }
    else {
        sprintf(fpath, "%s%s", dirpath, from);
    }

	res = rename(fpath, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
off_t offset, struct fuse_file_info *fi){
    char fpath[1000];
    if(strcmp(path,"/") == 0) {
        path=dirpath;
        sprintf(fpath,"%s",path);
        }
    else{
        // printf ("dirpath : %s\n path : %s\n", dirpath, path);
        sprintf(fpath, "%s%s",dirpath,path);
    }
    int res = 0;
    DIR *dp;
    
    struct dirent *de;
    (void) offset;
    (void) fi;
    dp = opendir(fpath);
    if (dp == NULL){
        return -errno;
    }
    while ((de = readdir(dp)) != NULL) {
        if (de->d_name[0]=='e' && de->d_name[1]=='n' && de->d_name[2]=='c' && de->d_name[3]=='v' && de->d_name[4]=='1' && de->d_name[5]=='_'){
            struct stat st;
            memset(&st, 0, sizeof(st));
            st.st_ino = de->d_ino;
            st.st_mode = de->d_type << 12;
            // if (strcmp(d_name, ""))

            res = (filler(buf, de->d_name, &st, 0));
            if(res!=0) break;
        }
    }
    closedir(dp);
    return 0;
}

// untuk membuka file pada folder hasil backup (fuse)
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
struct fuse_file_info *fi){
    char fpath[1000];
    if(strcmp(path,"/") == 0) {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    int res = 0;
    int fd = 0 ;
    (void) fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1){
        return -errno;
    }
    res = pread(fd, buf, size, offset);
    if (res == -1){
        res = -errno;
    }
    close(fd);
    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fullpath[1000];
    if (!strcmp(fullpath, "/"))
    {
        path = dirpath;
        strcpy(fullpath, path);
    }
    else
    {
        sprintf(fullpath, "%s%s", dirpath, path);
    }
    printf("1%s\n", fullpath);
    int fd;
    int res;
    (void)fi;
    fd = open(fullpath, O_WRONLY);
    if (fd == -1)
        return -errno;

    char backupname[1000];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char noext[1000];
    strcpy(noext, path);
    char *point = rindex(noext, '.');
    if (point)
    {
        *point = '\0';
        sprintf(backupname, "%s/backup%s.%02d:%02d:%02d:%02d-%02d-%d.%s", dirpath, noext, tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, point + 1);
    }
    else
    {
        sprintf(backupname, "%s/backup%s.%02d:%02d:%02d:%02d-%02d-%d", dirpath, noext, tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    }
    char backupfolder[1000];
    sprintf(backupfolder, "%s/backup/", dirpath);
    mkdir(backupfolder, 0755);

    res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;
    close(fd);
    pid_t child = fork();
    if (child == 0)
    {
        execl("/bin/cp", "cp", fullpath, backupname, NULL);
    }
    int status;
    while (wait(&status) > 0);
    chmod(backupname, 0400);
    return res;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
    char fullpath[1000];
    sprintf(fullpath, "%s%s", dirpath, path);
	res = truncate(fullpath, size);
	if (res == -1)
		return -errno;
	return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .truncate = xmp_truncate,
    .write = xmp_write,
    .rename = xmp_rename,
};

int  main(int  argc, char *argv[]){
    // de = opendir(/home/iqbalhumam73/Downloads)
    // printf ("Jalan\n");
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}