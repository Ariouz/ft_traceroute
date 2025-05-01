#include "ft_ping.h"

void    set_min_rtt(t_stats *stats, float rtt)
{
    if (rtt < stats->min_rtt) stats->min_rtt = rtt;
}


void    set_max_rtt(t_stats *stats, float rtt)
{
    if (rtt > stats->max_rtt) stats->max_rtt = rtt;
}

void    process_rtt(float rtt)
{
    t_stats *stats;
    stats = g_stats;

    set_min_rtt(stats, rtt);
    set_max_rtt(stats, rtt);
}