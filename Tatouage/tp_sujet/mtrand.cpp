/*
	MT19937 random generator - v. 0.1 - Fev. 10
 
	From http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/MTARCOK/mt19937ar-cok.c
 
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be
	useful, but WITHOUT ANY WARRANTY; without even the implied
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	PURPOSE.  See the GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
	MA 02111-1307, USA.
*/        

#include "mtrand.h"

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UMASK 0x80000000UL /* most significant w-r bits */
#define LMASK 0x7fffffffUL /* least significant r bits */
#define MIXBITS(u,v) ( ((u) & UMASK) | ((v) & LMASK) )
#define TWIST(u,v) ((MIXBITS(u,v) >> 1) ^ ((v)&1UL ? MATRIX_A : 0UL))


static unsigned long state[N]; // State vector 
static int left  = 1;
static int initf = 0;
static unsigned long *next;


void mtsrand(unsigned long s)
{
    int j;
    state[0]= s & 0xffffffffUL;
    for (j=1; j<N; j++) 
	{
        state[j] = (1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array state[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        state[j] &= 0xffffffffUL;  /* for >32 bit machines */
    }

    left = 1; initf = 1;
}

static void next_state(void)
{
    unsigned long *p = state;
    int j;

    /* if init_genrand() has not been called, */
    /* a default initial seed is used         */
    if (initf==0) mtsrand(5489UL);

    left = N;
    next = state;
    
    for (j=N-M+1; --j; p++) *p = p[M] ^ TWIST(p[0], p[1]);

    for (j=M; --j; p++) *p = p[M-N] ^ TWIST(p[0], p[1]);

    *p = p[M-N] ^ TWIST(p[0], state[0]);
}

unsigned long mtrand()
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}


