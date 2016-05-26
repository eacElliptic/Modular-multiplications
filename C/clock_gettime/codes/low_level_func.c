#include <stdio.h>
#include <time.h>
#include <gmp.h>
#include <stdint.h>
#include <stdlib.h>

#define BILLION 1000000000L


static gmp_randstate_t r;
static unsigned int nbiter, max_size, nb_limbs;
struct timespec start1;
struct timespec end1;
uint64_t diff1, diff2, diff3;

static mpz_t x, y, z, p;
static mp_limb_t *p_limbs, *x_limbs, *y_limbs, *z_limbs, *q_limbs;


/*
 * This function computes "2*nbiter" multiplications and modular reductions.
 * */
void mult() {
	int j;
	for (j = 0; j < nbiter; j++) {
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start1);
		
		mpn_mul_n (z_limbs, x_limbs, y_limbs, nb_limbs); // compute: z = x*y
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end1);
		diff1 += BILLION * (end1.tv_sec - start1.tv_sec) + end1.tv_nsec - start1.tv_nsec;

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start1);
		
		mpn_tdiv_qr (q_limbs, x_limbs, 0, z_limbs, (nb_limbs*2), p_limbs, nb_limbs);  // compute: x = z%p
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end1);
		diff2 += BILLION * (end1.tv_sec - start1.tv_sec) + end1.tv_nsec - start1.tv_nsec;

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start1);
		
		mpn_mul_n (z_limbs, y_limbs, x_limbs, nb_limbs); // compute: z = y*x
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end1);
		diff1 += BILLION * (end1.tv_sec - start1.tv_sec) + end1.tv_nsec - start1.tv_nsec;
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start1);
		
		mpn_tdiv_qr (q_limbs, y_limbs, 0, z_limbs, (nb_limbs*2), p_limbs, nb_limbs); // compute: y = z%p
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end1);
		diff2 += BILLION * (end1.tv_sec - start1.tv_sec) + end1.tv_nsec - start1.tv_nsec;
		
	}
    diff3=diff1+diff2 ;
}





int main(int argc, char *argv[]){

	int found, limbs_size;
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

	limbs_size = 8*sizeof(mp_limb_t); // size in bits
	nb_limbs = (max_size/limbs_size);
	if((max_size%limbs_size))
		nb_limbs++;

	p_limbs = mpz_limbs_modify (p, nb_limbs);
	x_limbs = mpz_limbs_modify (x, nb_limbs);
	y_limbs = mpz_limbs_modify (y, nb_limbs);
	
	z_limbs = (mp_limb_t*) calloc ((nb_limbs*2), sizeof(mp_limb_t));
	q_limbs = (mp_limb_t*) calloc ((nb_limbs+1), sizeof(mp_limb_t));
	

	nbiter = (1 << (atoi(argv[2])-1)); 

    diff1 = 0;
    diff2 = 0;
    diff3 = 0;
    
	printf("\n\nExecuting %u iterations for %d-bit integers ...\n", 2*nbiter, max_size);
	
	mult();
	
	printf("\nFinished, elapsed time (in nanoseconds) : \nmult : %llu  mod : %llu multmod : %llu\n", (long long unsigned int) diff1, (long long unsigned int) diff2, (long long unsigned int) diff3);

	
	free(z_limbs);
	free(q_limbs);
	mpz_clears(x, y, z, p, NULL);
	gmp_randclear(r);

	return 0;
}


