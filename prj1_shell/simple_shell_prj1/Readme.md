# Min-Shell

## Current features
### Parsing
1. basic parsing with separation by space-delimiter " " and pipeline signal "|".
2. Unsupported spliting based on logical operator (&, ^,...)


### Built-in command:
    - exit
    - cd

### Pipeline execution
    - pipe() to connect input/output
    - list of pids to wait for process end
    - Input/Output redirection ">/>>"


**Notes:** Currently, I also ran the ./validate test from build-your-own-shell git repo, and pass some certain test case regarding what I wrote above.



## Citation
**Acknowledgments:**
1. I used "build-your-own-shell" repository to learn about basic and advanced shell features & tools to test my implementation.
https://github.com/tokenrove/build-your-own-shell

2. I referenced Professor Brian Howard's shell implementation to guide the structure and modular design of my code. 
https://github.com/bhoward/CSC428/tree/main/howsh



**AI Assistance:**
This project was completed individually with the help of Google's AI Studio. https://aistudio.google.com/prompts/new_chat".
My use of AI was focused on:
1. Deepening my understanding of pointer management.
2. Clarifying strategies for debugging segmentation faults with GDB.