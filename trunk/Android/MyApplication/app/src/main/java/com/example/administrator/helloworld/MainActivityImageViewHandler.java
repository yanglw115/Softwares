package com.example.administrator.helloworld;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.ImageView;

import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by Administrator on 2017/4/24.
 */

public class MainActivityImageViewHandler extends Activity {
    int[] images = new int[] {
            R.drawable.image1, R.drawable.image2, R.drawable.image3
    };
    int imageIndex = 0;

    ImageView imageView;

    final Handler myHandler = new Handler()
    {
        @Override
        public void handleMessage(Message msg) {
            if (msg.what == 0x123) {
                imageView.setImageResource(images[imageIndex++ % images.length]);
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        super.setContentView(R.layout.layout_imageview_handler);

        imageView = (ImageView) findViewById(R.id.idImageViewHandler);

        new Timer().schedule(new TimerTask() {
            @Override
            public void run() {
                myHandler.sendEmptyMessage(0x123);
            }
        }, 0, 2000);
    }
}
