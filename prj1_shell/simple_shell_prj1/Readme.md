# Simple shell

## Current features:

### Parsing
1. basic parsing with separation by space-delimiter " " and pipeline signal "|".
2. Might need refractoring for future improve: Separating based on a list of sign, and include a stack/vector to store and perform logic operation



### Built-in command:
    - exit
    - cd

### pipeline execution
    - pipe() to connect input/output
    - list of pids to wait for process end
    - Input/Output redirection ">/>>"

Currently, I also ran the ./validate test from build-your-own-shell git repo, and pass some certain test case regarding what I wrote above.