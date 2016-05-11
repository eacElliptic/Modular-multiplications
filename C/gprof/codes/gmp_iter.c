#include <stdio.h>
#include <time.h>
#include <gmp.h>


static mpz_t x;
static mpz_t y;
static mpz_t z;
static mpz_t p;
static gmp_randstate_t r;
static unsigned nbiter, max_size;

/*
 * This function computes "2*nbiter" multiplications and modular reductions.
 * */
void mult() {
	int j;
	for (j = 0; j < nbiter; j++) {
		mpz_mul (z, x, y); // compute: z = x*y
		mpz_mod (x, z, p); // compute: x = z%p
		
		mpz_mul (z, y, x); // compute: z = y*x
		mpz_mod (y, z, p); // compute: y = z%p
	}
}



int main(void){

  int found;
  unsigned long seed = time(NULL); 
  
  // Initialisation of the random generator
  gmp_randinit_default(r);
  gmp_randseed_ui(r, seed);

  mpz_inits (x, y, z, p, NULL);
  
  
  max_size = 538 ; // 256 358  384 538  512 717
  
  // Generation of two integers of size max_size bits.
  mpz_urandomb(x, r, max_size);    mpz_setbit (x, (max_size-1));
  mpz_urandomb(y, r, max_size);    mpz_setbit (y, (max_size-1));

  // Generation of a (proba) prime number of size max_size bits. 
  do{
	mpz_urandomb (p, r, max_size); 
	mpz_setbit (p, (max_size-1));
	found = mpz_probab_prime_p (p, 25);
  }while(!found);
  
  
  nbiter = (1 << 28);  // nbiter = 2^28
  mult();

  printf("\nFinished\n");
  
  mpz_clears(x, y, z, p, NULL);
  gmp_randclear(r);


  return 0;
}











