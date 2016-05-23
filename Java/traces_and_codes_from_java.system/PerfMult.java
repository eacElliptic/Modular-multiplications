package perfmult;

import java.security.SecureRandom;

public class PerfMult {

    public static void main(String[] args) {

        Codes codes = new Codes(new SecureRandom());

        int [] size = {192, 256, 269, 358, 384, 512, 538, 717};
        int nbIter = (1 << 22);

        System.out.println("nbIter = " + nbIter + "\n");
      
        int j;
        System.out.println("                Mult time                   Mod time                    Total time");
        for (j = 0; j < size.length; j++)
            codes.go(size[j], nbIter);
        
        
        System.out.println("\nCompleted");

    }

}
