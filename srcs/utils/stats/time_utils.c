#include "ft_ping.h"

long long get_time_ms(struct timeval tv) {
    return (long long)(tv.tv_sec) * 1000 + (long long)(tv.tv_usec) / 1000;
}

long long get_time_us(struct timeval tv) {
    return (long long)(tv.tv_sec) * (1000*1000) + (long long)(tv.tv_usec);
}

