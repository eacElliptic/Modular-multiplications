package fr.android.bri.test_profiling;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import java.math.BigInteger;
import java.security.SecureRandom;

public class MainActivity extends Activity implements SeekBar.OnSeekBarChangeListener {

    private SeekBar bar;
    private TextView iteration, result;
    private int nbiter;
    private int size;
    private BigInteger a, b, p;
    private SecureRandom rg = new SecureRandom();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_perf_mul);

        nbiter = 1;
        bar = (SeekBar) findViewById(R.id.seekBar1);
        bar.setOnSeekBarChangeListener(this);
        iteration = (TextView) findViewById(R.id.nbiter);

        result = (TextView) findViewById(R.id.result);

    }


    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        nbiter = (1 << progress);
        iteration.setText(String.valueOf(progress + 1)); // les opérations sont effectuées deux fois dans mult et add
    }


    public void go(View v) {

        Button bt = (Button) v;
        size = Integer.parseInt(bt.getText().toString());

        p = BigInteger.probablePrime(size, rg);
        a = new BigInteger(size, rg);
        b = new BigInteger(size, rg);
        a = a.mod(p);
        b = b.mod(p);

        int j;
        String stR;
        long aa, bb, cc, dd, ee ,t_mul, t_mod ;
        BigInteger intMultTime, intModTime, intTotalTime;
        intMultTime = BigInteger.ZERO;
        intModTime = BigInteger.ZERO;

        for (j=0; j<nbiter; j++) {
            aa = System.currentTimeMillis();
            a = a.multiply(b);
            bb = System.currentTimeMillis();
            a = a.mod(p);
            cc = System.currentTimeMillis();
            b = b.multiply(a);
            dd = System.currentTimeMillis();
            b = b.mod(p);
            ee = System.currentTimeMillis();

            t_mul = (bb-aa) + (dd-cc);
            t_mod = (cc-bb) + (ee-dd);

            intMultTime = intMultTime.add(BigInteger.valueOf(t_mul));
            intModTime = intModTime.add(BigInteger.valueOf(t_mod));
        }
        intTotalTime = intMultTime.add(intModTime);

        stR = "Integer size : " + size + "\n\n";
        stR += "Times in milliseconds\n\n";
        stR += "Total time : " + intTotalTime + "\n";
        stR += "Mult time : " + intMultTime + "\n";
        stR += "Mod time : " + intModTime;

        result.setText(stR);

        System.gc();
    }



    /***********************************************/


    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }


}
