#include <stdio.h>
#include <time.h>
#include <gmp.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#define BILLION 1000000000L

static mpz_t x;
static mpz_t y;
static mpz_t z;
static mpz_t p;
static gmp_randstate_t r;
static unsigned nbiter, max_size;
struct timespec start1;
struct timespec end1;
uint64_t diff1, diff2, diff3;


/*
 * This function computes "2*nbiter" multiplications and modular reductions.
 * */
void mult() {
	int j;
	for (j = 0; j < nbiter; j++) {
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start1);
		
		mpz_mul (z, x, y); // compute: z = x*y
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end1);
		diff1 += BILLION * (end1.tv_sec - start1.tv_sec) + end1.tv_nsec - start1.tv_nsec;

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start1);
		
		mpz_mod (x, z, p); // compute: x = z%p
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end1);
		diff2 += BILLION * (end1.tv_sec - start1.tv_sec) + end1.tv_nsec - start1.tv_nsec;

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start1);
		
		mpz_mul (z, y, x); // compute: z = y*x
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end1);
		diff1 += BILLION * (end1.tv_sec - start1.tv_sec) + end1.tv_nsec - start1.tv_nsec;
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start1);
		
		mpz_mod (y, z, p); // compute: y = z%p
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end1);
		diff2 += BILLION * (end1.tv_sec - start1.tv_sec) + end1.tv_nsec - start1.tv_nsec;
		
	}
    diff3=diff1+diff2 ;
}



int main(int argc, char *argv[]){

	int found;
	unsigned long seed = time(NULL);

	// Initialisation of the random generator
	gmp_randinit_default(r);
	gmp_randseed_ui(r, seed);

	mpz_inits (x, y, z, p, NULL);

	max_size = atoi(argv[1]) ; 

	// Generation of two integers of size max_size bits.
	mpz_urandomb(x, r, max_size);    mpz_setbit (x, (max_size-1));
	mpz_urandomb(y, r, max_size);    mpz_setbit (y, (max_size-1));

	// Generation of a (proba) prime number of size max_size bits. 
	do{
		mpz_urandomb (p, r, max_size);
		mpz_setbit (p, (max_size-1));
		found = mpz_probab_prime_p (p, 25);
	}while(!found);

	nbiter = (1 << (atoi(argv[2]) - 1)); 

    diff1 = 0;
    diff2 = 0;
    diff3 = 0;
    
	printf("\nExecuting %u iterations for %d-bit integers ...\n", 2*nbiter, atoi(argv[1]));
	
	mult();

	printf("Finished, elapsed time = mult : %llu  mod : %llu multmod : %llu nanoseconds\n\n", (long long unsigned int) diff1, (long long unsigned int) diff2, (long long unsigned int) diff3);


	mpz_clears(x, y, z, p, NULL);
	gmp_randclear(r);

	return 0;
}
 
 
 
 
