package com.example.administrator.helloworld;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import java.text.AttributedCharacterIterator;

/**
 * Created by Administrator on 2016/10/13.
 */

public class DrawView extends View {
    public float currentX = 40;
    public float currentY = 50;

    Paint p = new Paint();
    public DrawView(Context context)
    {
        super(context);
    }

    public DrawView(Context context, AttributeSet set)
    {
        super(context, set);
    }

    @Override
    public void onDraw(Canvas canvas)
    {
        super.onDraw(canvas);

        p.setColor(Color.RED);
        canvas.drawCircle(currentX, currentY, 15, p);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        currentX = event.getX();
        currentY = event.getY();
        invalidate();

        return true;
    }
}
