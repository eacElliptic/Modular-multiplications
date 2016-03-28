package perfmult;

import java.security.SecureRandom;

public class PerfMult {

    public static void main(String[] args) {

        Codes codes = new Codes(new SecureRandom());

        int size = 717; // 256 358  384 538  512 717
        int nbIter = (1 << 20);

        System.out.println("nbIter = " + nbIter);
        System.out.println();

        codes.go(size, nbIter);

    }

}
