#include <Fract/Fract.h>

/*
-----------------------------
 -> Fract Random Functions <- 
-----------------------------
*/

// Pseudo Random Number Generator

static unsigned int global_seed = 0;

unsigned int rand_uint(unsigned int num)
{
    num = (num << 13) ^ num;
    return ((num * (num * num * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

unsigned int rand_num() 
{
    return rand_uint(global_seed);
}

unsigned int rand_next()
{
    return rand_uint(global_seed++);
}

void rand_seed(unsigned int seed) 
{
    global_seed = seed;
}