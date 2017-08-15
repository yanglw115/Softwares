package com.example.administrator.helloworld;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.view.MotionEvent;
import android.view.View;

/**
 * Created by Administrator on 2017/5/3.
 */

public class ViewCanvasDoubleBuffer extends View {
    float m_preX, m_preY;
    private Path m_path;
    public Paint m_paint = null, m_paintReal = null;
    Bitmap m_bitmapCache = null;
    Canvas m_canvasCache = null;

    public ViewCanvasDoubleBuffer(Context context, int width, int height) {
        super(context);

        /* 创建一个与View大小相等的缓冲区 */
        m_bitmapCache = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        m_canvasCache = new Canvas();
        m_path = new Path();
        /* m_canvasCache将会将内容绘制到内存缓冲区 */
        m_canvasCache.setBitmap(m_bitmapCache);
        /* 设置画笔颜色 */
        m_paint = new Paint(Paint.DITHER_FLAG);
        m_paint.setColor(Color.RED);
        /* 设置画笔风格 */
        m_paint.setStyle(Paint.Style.STROKE);
        m_paint.setStrokeWidth(1);
        /* 反锯齿 */
        m_paint.setAntiAlias(true);
        m_paint.setDither(true);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        //return super.onTouchEvent(event);
        float x = event.getX();
        float y = event.getY();
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                m_path.moveTo(x, y);
                m_preX = x;
                m_preY = y;
                break;
            case MotionEvent.ACTION_MOVE:
                /* 贝塞尔曲线 */
                m_path.quadTo(m_preX, m_preY, x, y);
                m_preX = x;
                m_preY = y;
                break;
            case MotionEvent.ACTION_UP:
                m_canvasCache.drawPath(m_path, m_paint);
                break;
        }
        invalidate();
        return true;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        m_paintReal = new Paint();
        /* 将bitmapCache绘制到该canvas上面,即绘制已经存在的内容 */
        canvas.drawBitmap(m_bitmapCache, 0, 0, m_paintReal);
        /* 当前画的内容 */
        canvas.drawPath(m_path, m_paintReal);
    }
}
