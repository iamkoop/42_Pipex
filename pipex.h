#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include "libft_lib/libft.h"

typedef struct s_pipex
{
    
    int fds[2];
    int child1;
    int child2;
}   t_pipex;

//MAIN
int	main(int argc, char	**argv,  char **envp);

//ACCESS
char *get_path(char	*cmd, char **envp);

#endif