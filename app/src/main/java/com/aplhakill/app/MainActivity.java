package com.aplhakill.app;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.VideoView;

public class MainActivity extends Activity implements Runnable {
    private static final String LOG_DEBUG = "MainActivity";
    private Button mForwardBtn;
    private Button mBackBtn;
    private VideoView mVideoView;

    private Thread thread = null;
    private boolean isrun = false;
    private Object sync = new Object();

    private native int copen(String url);
    private native int cclose();
    private native int cbackward();
    private native int cforward();

    static {
        System.loadLibrary("alphatest");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mForwardBtn = (Button) findViewById(R.id.forward_btn);
        mBackBtn = (Button) findViewById(R.id.back_btn);
        mVideoView = (VideoView) findViewById(R.id.video_view);
        mForwardBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                forwardVideo();
            }
        });
        mBackBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                backVideo();
            }
        });
    }

    private void forwardVideo() {
        Log.d(LOG_DEBUG, " forwardVideo ");
        //TODO Forward video
        start();
    }

    private void backVideo() {
        Log.d(LOG_DEBUG, " backVideo ");
        //TODO Back video
        cbackward();
    }

    @Override
    public void run() {
        copen("/system/media/test.ts");
    }

    public void start() {
        synchronized (sync) {
            if (!isrun) {
                thread = new Thread(this);
                if (thread != null) {
                    thread.start();
                    isrun = true;
                }
            }
        }
    }

    public void stop() {
        if (thread != null) {
            thread.interrupt();
            try {
                thread.join(1000);
            } catch (InterruptedException e) {
            }
            cclose();
        }
    }
}
