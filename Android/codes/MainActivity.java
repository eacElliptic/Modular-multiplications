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
        iteration.setText(String.valueOf(progress + 1)); // les opérations sont effectuées deux fois dans mult et add
    }


    public void go(View v) {

        Button bt = (Button) v;
        size = Integer.parseInt(bt.getText().toString());

        int selectedId = radioGroup.getCheckedRadioButtonId();
        radioButton = (RadioButton) findViewById(selectedId);

        p = BigInteger.probablePrime(size, rg);
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
            Debug.startMethodTracing(traceName, 600 * 1024 * 1024);
            mult();
            Debug.stopMethodTracing();
            Toast.makeText(this, "multiplications terminées", Toast.LENGTH_LONG).show();
        } else {
            traceName = "perfAdd_" + size + "_" + log2_nbIter;
            Debug.startMethodTracing(traceName, 700 * 1024 * 1024);
            add();
            Debug.stopMethodTracing();
            Toast.makeText(this, "additions terminées", Toast.LENGTH_LONG).show();
        }

        System.gc();
//        System.exit(0);
    }


    /***********************************************/

    public void mult() {
        for (j = 1; j <= nbiter; j++) {
            a = a.multiply(b);
            a = a.mod(p);
            b = b.multiply(a);
            b = b.mod(p);
        }
    }

    public void add() {
        for (j = 1; j <= nbiter; j++) {
            a = a.add(b);
            a = a.mod(p);
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
