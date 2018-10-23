static inline int
get_access_time(volatile char *addr)
{
unsigned long long time1, time2;
unsigned junk;
time1 = __rdtscp(&junk);
(void)*addr;
time2 = __rdtscp(&junk);
return time2 - time1;
}
