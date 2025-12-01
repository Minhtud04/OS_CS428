#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
/**
 * Wrap around System call, only try to format the struct data and print out
 * 
 */

/**
 * struct stat {
    dev_t     st_dev;      // Device ID containing the file
    ino_t     st_ino;      // Inode number
    mode_t    st_mode;     // File type + permissions (encoded in bits)
    nlink_t   st_nlink;    // Number of hard links
    uid_t     st_uid;      // Owner's user ID (just a number like 1000)
    gid_t     st_gid;      // Owner's group ID (just a number)
    dev_t     st_rdev;     // Device ID (if special file)
    off_t     st_size;     // File size in bytes
    blksize_t st_blksize;  // Block size for filesystem I/O
    blkcnt_t  st_blocks;   // Number of 512-byte blocks allocated
    time_t    st_atime;    // Last access time (seconds since 1970)
    time_t    st_mtime;    // Last modification time
    time_t    st_ctime;    // Last status change time
};
 */

void print_file_type(mode_t mode) {
    printf("  File type:            ");
    if (S_ISREG(mode))       printf("regular file\n");
    else if (S_ISDIR(mode))  printf("directory\n");
    else if (S_ISLNK(mode))  printf("symbolic link\n");
    else if (S_ISCHR(mode))  printf("character device\n");
    else if (S_ISBLK(mode))  printf("block device\n");
    else if (S_ISFIFO(mode)) printf("FIFO/pipe\n");
    else if (S_ISSOCK(mode)) printf("socket\n");
    else                     printf("unknown\n");
}

void print_permissions(mode_t mode) {
    printf("  Permissions:          ");
    
    //Owner
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');  // r
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');  // w
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');  // ex
    
    //group
    printf("%c", (mode & S_IRGRP) ? 'r' : '-'); 
    printf("%c", (mode & S_IWGRP) ? 'w' : '-'); 
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');  
    
    //other
    printf("%c", (mode & S_IROTH) ? 'r' : '-');  
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');  
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');  
    
    // Print octal representation (e.g., 644, 755)
    // 0777 masks out just the permission bits (9 lowest bits)
    printf(" (%o)\n", mode & 0777);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_or_directory>\n", argv[0]);
        return 1;
    }

    struct stat sb;
    
    if (stat(argv[1], &sb) == -1) { // given by system call
        perror("stat");
        return 1;
    }

    printf("  File: %s\n", argv[1]);
    print_file_type(sb.st_mode);
    printf("  Size:                 %ld bytes\n", (long)sb.st_size);
    printf("  Blocks allocated:     %ld\n", (long)sb.st_blocks);
    printf("  Block size:           %ld bytes\n", (long)sb.st_blksize);
    printf("  Inode:                %ld\n", (long)sb.st_ino);
    printf("  Link count:           %ld\n", (long)sb.st_nlink);
    
    // Get owner name
    struct passwd *pw = getpwuid(sb.st_uid);
    if (pw) {
        printf("  Owner: %s (UID: %d)\n", pw->pw_name, sb.st_uid);
    } else {
        printf("  Owner UID: %d\n", sb.st_uid);
    }
    
    // Get group name
    struct group *gr = getgrgid(sb.st_gid);
    if (gr) {
        printf("  Group: %s (GID: %d)\n", gr->gr_name, sb.st_gid);
    } else {
        printf("  Group GID: %d\n", sb.st_gid);
    }
    
    print_permissions(sb.st_mode);
    
    printf("  Last access:          %s", ctime(&sb.st_atime));
    printf("  Last modification:    %s", ctime(&sb.st_mtime));
    printf("  Last status change:   %s", ctime(&sb.st_ctime));

    return 0;
}