package perfmult;

import java.math.BigInteger;
import java.security.SecureRandom;


public class Codes {


    private int  nbiter;
    private BigInteger a, b, p;
    private SecureRandom rg;

    public Codes(SecureRandom rg) {
        this.rg = rg;
    }


    public void go(int pSize, int pNbiter) {

		// Generation of a (proba) prime number of size pSize bits. 
        p = BigInteger.probablePrime(pSize, rg);
        
        // Generation of two integers of size pSize bits.
        a = new BigInteger(pSize, rg);
        b = new BigInteger(pSize, rg);
        a.setBit(pSize - 1);
        b.setBit(pSize - 1);
       
        a = a.mod(p);
        b = b.mod(p);
        
        nbiter = pNbiter;
        
        
        mult();

        System.out.println("Finished");

        System.gc();
    }

/*
 * This function computes "2*nbiter" multiplications and modular reductions.
 * */
    public void mult() {
        int j;
        for (j = 1; j <= nbiter; j++) {
            a = a.multiply(b);       // compute: a = a*b
            a = a.mod(p);	    // compute: a = a%p
               
            b = b.multiply(a);
            b = b.mod(p);
           
        }
    }

/*
 * This function computes "2*nbiter" additions and modular reductions.
 * */
    public void add() {
        int j;
        for (j = 1; j <= nbiter; j++) {
            a = a.add(b);			// compute: a = a+b
            a = a.mod(p);			// compute: a = a%p
            
            b = b.add(a);
            b = b.mod(p);
        }
    }

}
