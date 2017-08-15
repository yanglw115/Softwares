package com.example.administrator.helloworld;

import android.os.Bundle;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;

/**
 * Created by Administrator on 2017/4/28.
 */

public class ActivityAnimationDrawableTest extends MainActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_animation_drawable_test);

        final ImageView imageView = (ImageView) findViewById(R.id.idAnimationImage);
        Button button = (Button) findViewById(R.id.idButtonStart);
        final Animation animation = (Animation) AnimationUtils.loadAnimation(this, R.anim.animation_drawable_test);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                imageView.startAnimation(animation);
            }
        });
    }
}
