package com.example.administrator.helloworld;

import android.graphics.drawable.ClipDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.ImageView;

import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by Administrator on 2017/4/28.
 */

public class ActivityClipDrawableTest extends MainActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_clipdrawable_test);

        ImageView imageView = (ImageView) findViewById(R.id.idImageClip);
        final ClipDrawable clipDrawable = (ClipDrawable) imageView.getDrawable();

        final Handler handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == 0x123) {
                    clipDrawable.setLevel(clipDrawable.getLevel() + 200);
                }
            }
        };

        final Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                Message message = new Message();
                message.what = 0x123;
                handler.sendMessage(message);
                /* level的值是从0到10000 */
                if (clipDrawable.getLevel() >= 10000) {
                    timer.cancel();
                }
            }
        }, 0, 300);

    }
}
