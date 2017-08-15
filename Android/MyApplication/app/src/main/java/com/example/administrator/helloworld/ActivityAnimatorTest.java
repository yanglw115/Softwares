package com.example.administrator.helloworld;

import android.animation.AnimatorInflater;
import android.animation.ArgbEvaluator;
import android.animation.ObjectAnimator;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.LinearLayout;

/**
 * Created by Administrator on 2017/4/28.
 */

public class ActivityAnimatorTest extends MainActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_animator_test);

        LinearLayout linearLayout = (LinearLayout) findViewById(R.id.idLayoutAnimator);
        linearLayout.addView(new MyanimationView(this));

    }
    public class MyanimationView extends View
    {
        public MyanimationView(Context context)
        {
            super(context);
            /* 加载动画 */
            ObjectAnimator animator = (ObjectAnimator) AnimatorInflater.loadAnimator(ActivityAnimatorTest.this, R.animator.animator_test);
            animator.setEvaluator(new ArgbEvaluator());
            /* 对VIEW本身使用动画 */
            animator.setTarget(this);
            /* 开始指定动画 */
            animator.start();
        }
    };
}
