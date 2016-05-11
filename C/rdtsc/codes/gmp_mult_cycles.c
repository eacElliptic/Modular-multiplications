#include <stdio.h>
#include <time.h>
#include <gmp.h>

#define SIZES 6


static gmp_randstate_t r;

static int b[SIZES] = {256, 358, 384, 512, 538, 717};

static mpz_t nb_clock_cycles[SIZES];
static mpf_t avg_clock_cycles[SIZES];


void init_nb_avg_clock_cycles(){
	int i;
	for(i=0; i<SIZES; i++){
		mpz_init (nb_clock_cycles[i]);
		mpf_init (avg_clock_cycles[i]);
	}
}

void clear_nb_avg_clock_cycles(){
	int i;
	for(i=0; i<SIZES; i++){
		mpz_clear (nb_clock_cycles[i]);
		mpf_clear (avg_clock_cycles[i]);
	}
}




static __inline__ unsigned long long rdtsc(void){
	unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}





int main(void){
	int i, j, found;
	unsigned long aa, bb, rd, seed, nbiter; 
	mpz_t x, y, z, p;
	
	nbiter = (1 << 20);
	seed = time(NULL); 
	gmp_randinit_default(r);
	gmp_randseed_ui(r, seed);
	mpz_inits (x, y, z, p, NULL);
	init_nb_avg_clock_cycles();
	
	for (i=0; i<SIZES; i++) {
		mpz_urandomb(x, r, b[i]);
		mpz_urandomb(y, r, b[i]);
		mpz_setbit (x, (b[i]-1));
		mpz_setbit (y, (b[i]-1));

		do{
			mpz_urandomb (p, r, b[i]); 
			mpz_setbit (p, (b[i]-1));
			found = mpz_probab_prime_p (p, 25);
		}while(!found);

		for (j=0; j<nbiter; j++) {
			aa = rdtsc();
			mpz_mul(z,x,y);
			mpz_mod(x,z,p);
			
			mpz_mul(z,y,x);
			mpz_mod(y,z,p);
			bb = rdtsc();
			mpz_add_ui (nb_clock_cycles[i], nb_clock_cycles[i], (bb-aa));
		}
	}
	
	printf("nbiter = %lu\n", (nbiter*2));
	for (i=0; i<SIZES; i++) {
		mpf_set_z (avg_clock_cycles[i], nb_clock_cycles[i]);
		mpf_div_ui (avg_clock_cycles[i], avg_clock_cycles[i], (nbiter*2));
		printf("%d  =>	", b[i]);
		gmp_printf("total : %Zd		", nb_clock_cycles[i]);
		gmp_printf("average : %Ff\n", avg_clock_cycles[i]);
	}
	
	printf("\nRatios:\n");    
	mpf_div (avg_clock_cycles[1], avg_clock_cycles[1], avg_clock_cycles[0]);
	gmp_printf("%d/%d => %Ff\n", b[1], b[0], avg_clock_cycles[1]);
	mpf_div (avg_clock_cycles[4], avg_clock_cycles[4], avg_clock_cycles[2]);
	gmp_printf("%d/%d => %Ff\n", b[4], b[2], avg_clock_cycles[4]);
	mpf_div (avg_clock_cycles[5], avg_clock_cycles[5], avg_clock_cycles[3]);
	gmp_printf("%d/%d => %Ff\n", b[5], b[3], avg_clock_cycles[5]);
	
	
	mpz_clears(x, y, z, p, NULL);
	clear_nb_avg_clock_cycles();
	gmp_randclear(r);
	
	return 0;
}












