valgrind --leak-check=full  --trace-children=yes --track-origins=yes --track-fds=yes  --show-leak-kinds=all --suppressions=r.supp ./minishell
