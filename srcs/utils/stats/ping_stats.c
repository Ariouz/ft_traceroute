#include "ft_ping.h"

void    save_send_time(t_stats *stats, int sequence)
{
    t_sock_rtt new_rtt;
    struct timeval start;

    gettimeofday(&start, NULL);

    new_rtt.start = start;
    new_rtt.sequence = sequence;
    new_rtt.rtt = 0;
    new_rtt.status = SOCK_SENT;

    if (stats->sock_rtt_size >= stats->sock_rtt_total_size)
    {
        stats->sock_rtt_total_size += SOCK_RTT_BATCH_SIZE;
        stats->sock_rtt = realloc(stats->sock_rtt, stats->sock_rtt_total_size * sizeof(t_sock_rtt));
        if (!stats->sock_rtt) fatal_error("Failed to realloc ping stats array");
    }
    stats->sock_rtt[stats->sock_rtt_size++] = new_rtt;
}

float    save_rcv_time(t_stats *stats, int sequence)
{
    t_sock_rtt *sock_rtt;
    size_t index;
    float rtt;

    index = 0;
    while (index < stats->sock_rtt_size)
    {
        if (stats->sock_rtt[index].sequence == sequence)
        {
            sock_rtt = &stats->sock_rtt[index];

            gettimeofday(&stats->sock_rtt[index].end, NULL);
            rtt = (get_time_us(sock_rtt->end) - get_time_us(sock_rtt->start)) / (float) 1000.0;
            sock_rtt->rtt = rtt;
            sock_rtt->status = SOCK_RECEIVED;

            return rtt;
        }
        index++;
    }
    return 0;
}

t_stats    *init_stats()
{
    t_stats *stats;
    stats = (t_stats *) malloc(sizeof(t_stats));

    if (!stats) fatal_error("Failed to init ping stats");

    stats->avg_rtt = 0;
    stats->min_rtt = INT32_MAX;
    stats->max_rtt = INT32_MIN;
    stats->stddev = 0;
    stats->rcvd_sockets = 0;
    stats->sent_sockets = 0;
    stats->sock_rtt_size = 0;
    stats->sock_rtt_total_size = SOCK_RTT_BATCH_SIZE;
    stats->sock_rtt = malloc(sizeof(t_sock_rtt) * SOCK_RTT_BATCH_SIZE);

    return stats;
}

t_sock_rtt  *get_sock_rtt(t_stats *stats, int sequence)
{
    size_t index;

    index = 0;
    while (index < stats->sock_rtt_size)
    {
        if (stats->sock_rtt[index].sequence == sequence) return &stats->sock_rtt[index];
        index++;
    }
    return NULL;
}