#include "ft_ping.h"

void    handle_sig_int(int code)
{
    (void) code;
    
    pthread_mutex_lock(&g_opts_mutex);
    g_opts->is_running = false;
    pthread_mutex_unlock(&g_opts_mutex);
}

void handle_signals()
{
    if (signal(SIGINT, handle_sig_int) == SIG_ERR)
        fatal_error("Failed to register sigint handler");
}