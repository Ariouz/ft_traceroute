#include "ft_ping.h"

void    check_timeouts()
{
    size_t          index;
    struct timeval  now;
    t_sock_rtt      *rtt;

    
    pthread_mutex_lock(&g_stats_mutex);

    gettimeofday(&now, NULL);

    index = 0;
    while (index < g_stats->sock_rtt_size)
    {
        rtt = &g_stats->sock_rtt[index];
        if (rtt->status != SOCK_SENT)
        {
            index++;
            continue;
        }

        long diff = get_time_ms(now) - get_time_ms(rtt->start);
        if (diff >= get_option(OPT_LINGER)->value * 1000)
        {
            rtt->status = SOCK_TIMEOUT;
            if (get_option(OPT_VERBOSE)->value == true && get_option(OPT_QUIET)->value == false)
                printf("Request timeout for icmp_seq %d\n", rtt->sequence);
        }

        index++;
    }

    
    pthread_mutex_unlock(&g_stats_mutex);
}