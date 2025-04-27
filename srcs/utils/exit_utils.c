#include "ft_ping.h"

bool    is_runnning()
{
    bool run;

    pthread_mutex_lock(&g_opts_mutex);
    run = g_opts->is_running;
    pthread_mutex_unlock(&g_opts_mutex);
    return run;
}

void    free_stats(t_stats *stats)
{
    if (!g_stats) return ;
    
    free(stats->sock_rtt);
    free(stats);
}

void    clean_free()
{
    if (!g_opts) return;

    free_options(g_opts->options, g_opts->size);
    free_stats(g_stats);
    free(g_opts);
}

void    free_options(t_option **options, size_t opt_len)
{
    size_t  index = 0;
    if (!options) return ;

    while (index < opt_len)
    {
        free(options[index]);
        index++;
    }
    free(options);
}

void    clean_exit(int exit_code)
{
    if (g_opts->sockfd != -1) close(g_opts->sockfd);
	clean_free();
    exit(exit_code);
}