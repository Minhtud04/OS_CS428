# Ls, Find, Stat

## Current features
### ls


### find
    -name <filename> <dir>: search for specific file type ("*.c) or specific filename
    -type <type> <dir>: list all dir or file in dir

### stat
    -l <path>: list all metadata (name, permission,...)



## Citation
**Acknowledgments:**
1. Mainly relies on https://pages.cs.wisc.edu/~remzi/OSTEP/file-intro.pdf to understand the struct and the purpose of each system call (stat(), lstat(), opendir(), readdir()). 

For me, this project is fairly easy, mostly string manipulation and formatting, since the system call API does most of the work.




**AI Assistance:**
This project was completed individually with the help of Gemini, though not that much
My use of AI was focused on:
1. Deepening my understanding of each system call, and the struct I should use to parse the result on.
2. print formatting in C, and refine some syntax in C