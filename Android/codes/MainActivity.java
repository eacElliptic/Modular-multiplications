package fr.android.bri.test_profiling;

import android.app.Activity;
import android.os.Bundle;
import android.os.Debug;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.math.BigInteger;
import java.security.SecureRandom;

public class MainActivity extends Activity implements SeekBar.OnSeekBarChangeListener {

    private SeekBar bar;
    private TextView iteration;
    private int j, nbiter, size;
    private BigInteger a, b, p;
    private SecureRandom rg = new SecureRandom();
    private RadioGroup radioGroup;
    private RadioButton radioButton;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_perf_mul);

        nbiter = 1;
        bar = (SeekBar) findViewById(R.id.seekBar1);
        bar.setOnSeekBarChangeListener(this);
        iteration = (TextView) findViewById(R.id.nbiter);
        radioGroup = (RadioGroup) findViewById(R.id.opGoup);


    }


    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        nbiter = (1 << progress);
        iteration.setText(String.valueOf(progress + 1)); 
    }


    public void go(View v) {

        Button bt = (Button) v;
        size = Integer.parseInt(bt.getText().toString());

        int selectedId = radioGroup.getCheckedRadioButtonId();
        radioButton = (RadioButton) findViewById(selectedId);

		// Generation of a (proba) prime number of size "size" bits. 
        p = BigInteger.probablePrime(size, rg);
        
		// Generation of two integers of values between 0 and (2^size-1).
        a = new BigInteger(size, rg);
        b = new BigInteger(size, rg);
        
        a = a.mod(p);
        b = b.mod(p);

        int log2_nbIter;
        if(iteration.getText().toString().equals(""))
            log2_nbIter = 0;
        else
            log2_nbIter = Integer.parseInt(iteration.getText().toString());

        String traceName;
        if (radioButton.getText().toString().equals("multiplication")) {
            traceName = "perfMul_" + size + "_" + log2_nbIter;
            Debug.startMethodTracing(traceName, 600 * 1024 * 1024);  	 // Start trace recording
            mult();
            Debug.stopMethodTracing();   );  							 // Stop trace recording
            Toast.makeText(this, "multiplications completed", Toast.LENGTH_LONG).show();
        } else {
            traceName = "perfAdd_" + size + "_" + log2_nbIter;
            Debug.startMethodTracing(traceName, 700 * 1024 * 1024);      // Start trace recording
            add();
            Debug.stopMethodTracing();									 // Stop trace recording
            Toast.makeText(this, "additions completed", Toast.LENGTH_LONG).show();
        }

        System.gc();
//        System.exit(0);
    }


    /***********************************************/
/*
 * This function computes "2*nbiter" multiplications and modular reductions.
 * */
    public void mult() {
        for (j = 1; j <= nbiter; j++) {
            a = a.multiply(b);   // compute: a = a*b
            a = a.mod(p);        // compute: a = a%p
            b = b.multiply(a);
            b = b.mod(p);
        }
    }

/*
 * This function computes "2*nbiter" additions and modular reductions.
 * */
    public void add() {
        for (j = 1; j <= nbiter; j++) {
            a = a.add(b);     // compute: a = a+b
            a = a.mod(p);     // compute: a = a%p
            b = b.add(a);
            b = b.mod(p);
        }
    }

    /***********************************************/


    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }


}
