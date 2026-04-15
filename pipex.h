#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>

typedef struct s_pipex
{
    
    int fds[2];
    int child1;
    int child2;
}   t_pipex;
#endif