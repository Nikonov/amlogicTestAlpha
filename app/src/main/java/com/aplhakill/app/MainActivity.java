package com.aplhakill.app;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.VideoView;

public class MainActivity extends Activity {
    private static final String LOG_DEBUG = "MainActivity";
    private Button mForwardBtn;
    private Button mBackBtn;
    private VideoView mVideoView;

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
    }

    private void backVideo() {
        Log.d(LOG_DEBUG, " backVideo ");
        //TODO Back video
    }

}
