#include <stdio.h>
#include <gmp.h>
#include <openssl/bn.h>
#include <openssl/rand.h>

#define SIZES 6


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
	int i, j;
	unsigned long aa, bb, rd, nbiter; 
	BN_CTX *tmp;
	BIGNUM *x, *y, *p, *z, *r;
	mpz_t tmpC;
	mpz_init (tmpC);

	tmp = BN_CTX_new();
	BN_CTX_init(tmp);
	x = BN_new();
	y = BN_new();
	p = BN_new();
	z = BN_new();
	r = BN_new();
	init_nb_avg_clock_cycles();
	RAND_load_file("/dev/urandom", 50);
  
	nbiter = (1 << 20);
	
	for (i=0; i<SIZES; i++) {
		BN_rand(x, b[i], 0, 0);  
		BN_rand(y, b[i], 0, 0); 

		BN_generate_prime_ex(p, b[i], 0, NULL, NULL, NULL);

		for (j=0; j<nbiter; j++) {
			aa = rdtsc();
			BN_mul(z,x,y,tmp);
			BN_nnmod(x,z,p,tmp);
			
			BN_mul(z,y,x,tmp);
			BN_nnmod(y,z,p,tmp);
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

	

	BN_free(x);
	BN_free(y);
	BN_free(p);
	BN_free(z);
	BN_free(r);
	BN_CTX_free(tmp);
	clear_nb_avg_clock_cycles();
	mpz_clear (tmpC);

	return 0;
}





