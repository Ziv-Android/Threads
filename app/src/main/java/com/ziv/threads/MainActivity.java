package com.ziv.threads;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private EditText threadsEdit;
    private EditText iterationsEdit;
    private Button startBtn;
    private TextView logView;

    private LibraryAPI sdk;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        sdk = new LibraryAPI();
        sdk.nativeInit();

        viewInit();
    }

    private void viewInit() {
        threadsEdit = (EditText) findViewById(R.id.threads_edit);
        iterationsEdit = (EditText) findViewById(R.id.iterations_edit);
        startBtn = (Button) findViewById(R.id.start_button);
        logView = (TextView) findViewById(R.id.log_view);

        startBtn.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        int threads = getNumber(threadsEdit, 0);
        int iterations = getNumber(iterationsEdit, 0);

        if (threads > 0 && iterations > 0) {
//            startThreads(threads, iterations);
            sdk.posixThreads(threads, iterations);
        }
    }

    private void startThreads(int threads, int iterations) {
        javaThreads(threads, iterations);
    }

    private static int getNumber(EditText editText, int defaultValues) {
        int value;
        try {
            value = Integer.parseInt(editText.getText().toString());
        } catch (NumberFormatException e) {
            value = defaultValues;
        }
        return value;
    }

    private void javaThreads(int threads, final int iterations){
        // 为每一个worker创建一个基于java的线程
        for (int i = 0; i < threads; i++) {
            final int id = i;
            Thread thread = new Thread() {
                @Override
                public void run() {
                    sdk.nativeWorker(id, iterations);
                }
            };
            thread.start();
        }
    }

    @Override
    protected void onDestroy() {
        sdk.nativeFree();

        super.onDestroy();
    }

    private void onNativeMessage(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                logView.append(message);
                logView.append("\n");
            }
        });
    }
}