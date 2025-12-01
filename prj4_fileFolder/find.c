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

#define MAX_PATH 4096

/**
 * struct __dirstream {
 * int fd;              // File descriptor
 * char *data;          // Directory block buffer
 * size_t allocation;   // Space allocated
 * size_t size;         // Total valid data
 * size_t offset;       // Current offset
 * off_t filepos;       // Next entry position
 * int errcode;         // Delayed error code
 * void *lock;          // Mutex for thread safety
 * };
 * 
 * struct dirent {
 * ino_t d_no               // inode number - file serial number?
 * char d_name              // filename
 * unsigned char d_type     // type of the file - DIR or REG? -> file or directory
 * off_t d_off              // offset to next dir entry?
 * unsigned short d_reclen
 * }
 */


typedef struct {
    int long_format;
    char *name_filter; // search for substring match --- "-name" flag
    char type_filter;  // search for match type (file or dir) --- "-type" flag
} SearchOptions;




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

void print_long_format(const char *path, const char *name, struct stat *sb) {

    print_permissions(sb->st_mode);
    printf(" %3ld", (long)sb->st_nlink);
    
    // Owner
    struct passwd *pw = getpwuid(sb->st_uid);
    printf(" %-8s", pw ? pw->pw_name : "unknown");
    
    // Group
    struct group *gr = getgrgid(sb->st_gid);
    printf(" %-8s", gr ? gr->gr_name : "unknown");
    
    // Size
    printf(" %8ld", (long)sb->st_size);
    
    // Time
    char time_buf[64];
    struct tm *tm_info = localtime(&sb->st_mtime);
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
    printf(" %s", time_buf);
    
    // Print the full relative path instead of just filename
    printf(" %s\n", path);
}

void search_recursive(const char *cur_path, SearchOptions *opts){
    
    //opendir + readdir
    DIR *dir = opendir(cur_path);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        //cur_path + "/" + entry->d_name
        char full_path[MAX_PATH];
        if (snprintf(full_path, sizeof(full_path), "%s/%s", cur_path, entry->d_name) >= MAX_PATH) {
            fprintf(stderr, "Path too long: %s/%s\n", cur_path, entry->d_name);
            continue;
        }

        struct stat sb;
        if (lstat(full_path, &sb) == -1) {  // lstat() instead of stat() -> won't enter shortcut link (symbolic Link)
            perror("lstat");
            continue;
        }

        //Filter logic
        int match = 1;

        if (opts->name_filter) {
            if (fnmatch(opts->name_filter, entry->d_name, 0) != 0) {    //fnmatch instead of strstr for substring.
                match = 0;
            }
        }

        if (opts->type_filter){
            if (opts->type_filter == 'f' && !S_ISREG(sb.st_mode)) match = 0;
            if (opts->type_filter == 'd' && !S_ISDIR(sb.st_mode)) match = 0;
        }

        if (match) {
            if (opts->long_format) {
                print_long_format(full_path, entry->d_name, &sb);
            } else {
                printf("%s\n", full_path);
            }
        }

         // Recursive on directory
        if (S_ISDIR(sb.st_mode)) {
            search_recursive(full_path, opts);
        }
    }
    
    closedir(dir);
}

/**
 * mainly wrap around opendir and readdir. manipulate given DIR struct 
 * Currently, there are only 2 flags:
 *  -name -> substring match
 *  -type -> all file or folder(dir)
 * 
 * Could do more flag like searching for all "*.c | *.txt"
 */
int main(int argc, char *argv[]) {
    SearchOptions opts = {0, NULL, 0};
    char *root_path = ".";
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            opts.long_format = 1;
        } else if (strcmp(argv[i], "-name") == 0 && i + 1 < argc) {
            opts.name_filter = argv[++i];
        } 
        else if (strcmp(argv[i], "-type") == 0 && i + 1 < argc) {
            opts.type_filter = argv[++i][0];
        } else {
            root_path = argv[i];
        }
    }
    
    search_recursive(root_path, &opts);
    return 0;
}