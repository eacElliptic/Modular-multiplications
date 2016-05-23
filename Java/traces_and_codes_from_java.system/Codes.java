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
        
        size = pSize;
        nbiter = pNbiter;
        
        a.setBit(pSize - 1);
        b.setBit(pSize - 1);
        
        int j;
        long aa, bb, cc, dd, ee, t_mul, t_mod;
        BigInteger intMultTime, intModTime, intTotalTime;
        intMultTime = BigInteger.ZERO;
        intModTime = BigInteger.ZERO;
        
        
        for (j = 0; j < nbiter; j++) {
            aa = System.currentTimeMillis();
            a = a.multiply(b);
            bb = System.currentTimeMillis();
            a = a.mod(p);
            cc = System.currentTimeMillis();
            b = b.multiply(a);
            dd = System.currentTimeMillis();
            b = b.mod(p);
            ee = System.currentTimeMillis();

            t_mul = (bb - aa) + (dd - cc);
            t_mod = (cc - bb) + (ee - dd);

            intMultTime = intMultTime.add(BigInteger.valueOf(t_mul));
            intModTime = intModTime.add(BigInteger.valueOf(t_mod));
        }
        intTotalTime = intMultTime.add(intModTime);
        
        System.out.print("Size : " + size);
        System.out.print("              " + intMultTime);
        System.out.print("              " + intModTime);
        System.out.println("                " + intTotalTime);
        

     
    }


    
  
}
