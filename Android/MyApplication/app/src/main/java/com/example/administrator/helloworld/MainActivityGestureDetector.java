package com.example.administrator.helloworld;

import android.app.Activity;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.widget.Toast;

/**
 * Created by Administrator on 2017/5/5.
 */

public class MainActivityGestureDetector extends Activity
        implements GestureDetector.OnGestureListener {
    /* 使用GestureDetector需要实例化，并且需要实现OnGestureListener接口，这里采用implements来进来实现 */
    GestureDetector m_gestureDetector = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_gesture_detector);
        /* 监听者也是此Activity */
        /* 此处对GestureDetector进行实例化 */
        m_gestureDetector = new GestureDetector(this, this);
    }

    /*  这里对Activity的onTouchEvent进行重写 */
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return m_gestureDetector.onTouchEvent(event);
    }

    @Override
    public boolean onDown(MotionEvent e) {
        /* 碰触事件按下时触发 */
        Toast.makeText(this, "onDown", Toast.LENGTH_SHORT).show();
        return false;
    }

    @Override
    public void onShowPress(MotionEvent e) {
        /* 当用户在触屏上按下，而且还未移动和松动开时触发该方法 */
        Toast.makeText(this, "onShowPress", Toast.LENGTH_SHORT).show();
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        /* 用户在触屏上的轻击事件会触发 */
        Toast.makeText(this, "onSingleTapUp", Toast.LENGTH_SHORT).show();
        return false;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        /* 用户在触屏上滚动时触发 */
        Toast.makeText(this, "onScroll", Toast.LENGTH_SHORT).show();
        return false;
    }

    @Override
    public void onLongPress(MotionEvent e) {
        /* 用户在触屏上长按时触发 */
        Toast.makeText(this, "onLongPress", Toast.LENGTH_SHORT).show();

    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        /* 用户在触屏上"拖过"时触发，参数中有拖过时的水平与垂直方向的速度 */
        Toast.makeText(this, "onFling", Toast.LENGTH_SHORT).show();
        return false;
    }
}
