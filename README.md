# minishell_main
This project is about creating a simple shell. The goal is to learn a lot about processes and file descriptors.

## How to compile
+ Open folder `minishell_main/`
+ To compile:
  + `make bonus`
+ To remove objects:
  + `make clean`
+ To remove objects and binary file (program):
  + `make fclean`
+ To re-compile:
  + `make re`
+ Execute:
  + `./minishell` and then enter you command
  
  Example: `./minishell`  `cd ..` 

## This shell:
+ Displayes a prompt when waiting for a new command.
+ Has a working history.
+ Can dearch and launch the right executable (based on the PATH variable or using a relative or an absolute path).
+ Not interpret unclosed quotes or special characters which are not required by the subject such as \ (backslash) or ; (semicolon).
+ Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
+ Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
+ Implements redirections.
+ Implements pipes (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
+ Handle $? which should expand to the exit status of the most recently executed foreground pipeline.
+ Implements the following builtins:
  + echo with option -n
  + cd with only a relative or absolute path
  + pwd with no options
  + export with no options
  + unset with no options
  + env with no options or arguments
  + exit with no options
+ Implements wildcards *.
