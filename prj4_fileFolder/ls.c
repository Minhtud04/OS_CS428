#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>


void print_permissions(mode_t mode) {
    // File type character
    if (S_ISDIR(mode))       printf("d");
    else if (S_ISLNK(mode))  printf("l");
    else if (S_ISCHR(mode))  printf("c");
    else if (S_ISBLK(mode))  printf("b");
    else if (S_ISFIFO(mode)) printf("p");
    else if (S_ISSOCK(mode)) printf("s");
    else                     printf("-");
    
    // Owner permissions
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');
    
    // Group permissions
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');
    
    // Other permissions
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');
}

// use for full style combined with stat system call
void print_long_format(const char *dir_path, const char *filename) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);
    
    struct stat sb;                                 //
    if (lstat(full_path, &sb) == -1) {
        perror("stat");
        return;
    }
    
    // Permissions
    print_permissions(sb.st_mode);
    
    // Link count
    printf(" %3ld", (long)sb.st_nlink);
    
    // Owner
    struct passwd *pw = getpwuid(sb.st_uid);
    if (pw) {
        printf(" %-8s", pw->pw_name);
    } else {
        printf(" %-8d", sb.st_uid);
    }
    
    // Group
    struct group *gr = getgrgid(sb.st_gid);
    if (gr) {
        printf(" %-8s", gr->gr_name);
    } else {
        printf(" %-8d", sb.st_gid);
    }
    
    // Size
    printf(" %8ld", (long)sb.st_size);
    
    // Modification time
    char time_buf[64];
    struct tm *tm_info = localtime(&sb.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
    printf(" %s", time_buf);
    
    // Filename
    printf(" %s\n", filename);
}

/**
 * mainly wrap around opendir and readdir. manipulate given DIR struct 
 */
int main(int argc, char *argv[]) {
    int long_format = 0;
    char *dir_path = ".";
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            long_format = 1;
        } else {
            dir_path = argv[i];
        }
    }
    
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    
    // no skip any hidden file
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {        
        if (long_format) {
            print_long_format(dir_path, entry->d_name);
        } else {
            printf("%s\n", entry->d_name);
        }
    }
    
    closedir(dir);
    return 0;
}