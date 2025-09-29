# Feature included:

1. Simple shell with infinite loop, printout 
    min-shell:<current_dir>$ 

2. Built-in: list + change directory + exit:
    - ls <path>
    - cd <path>
    - exit: terminate the current process?

3. Execute pipeline: 
- break into different command
- break 1 command into word -> execute

# Flow

1. Flow:
- Break initial string into pipeline
- With each pipeline string, handle parsing into either built-in struct or command struct
- Handle built-in struct/command struct, and store memory output into a char** 
- parsing input into next built-in/command struct




# Current state
### 9/29
1. Testing: Don't know where is seg fault

2. Target:
    - At least fix all the current code -> only need to create fork()/exec() in exec_child_process...

3. Done:
    - Know how to use debugger with gdb: gdb ./min-shell -> run -> bt (run and backtrack to see where is the seg fault)
    - Know about pointers: 
        char * is a string - basically
        make pipeline_t to be a string (char*)
        sth** means a pointer to a pointer
        sth* ls could be access with ls[i] -> sth - as ls store a pointer to the start of the list !
    - Reformat the code with deeper understand of what I'm doing right now - builtin from command, not from args anymore


4. Next state:
    - implement execute_command with fork() & more
    - implement execute_ls -> see what's in the current directory (builtin)
