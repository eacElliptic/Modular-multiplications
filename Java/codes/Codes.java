package perfmult;

import java.math.BigInteger;
import java.security.SecureRandom;

/**
 * Created by bri on 10/03/16.
 **/

public class Codes {


    private int  nbiter, size;
    private BigInteger a, b, p;
    private SecureRandom rg;

    public Codes(SecureRandom rg) {
        this.rg = rg;
    }


    public void go(int pSize, int pNbiter) {

        p = BigInteger.probablePrime(pSize, rg);
        a = new BigInteger(pSize, rg);
        b = new BigInteger(pSize, rg);
       
        a = a.mod(p);
        b = b.mod(p);
        
        nbiter = pNbiter;
        
        size = pSize - 1;
        a.setBit(size);
        b.setBit(size);
        
        
        mult();
        
//        add();

        System.out.println("Opérations terminées");

        System.gc();
    }


    public void mult() {
        int j;
        for (j = 1; j <= nbiter; j++) {
            a = a.multiply(b);
            a = a.mod(p);
               
            b = b.multiply(a);
            b = b.mod(p);
           
        }
    }

    public void add() {
        int j;
        for (j = 1; j <= nbiter; j++) {
            a = a.add(b);
            a = a.mod(p);
            
            b = b.add(a);
            b = b.mod(p);
        }
    }

}
