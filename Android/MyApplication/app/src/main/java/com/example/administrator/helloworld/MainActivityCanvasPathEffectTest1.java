package com.example.administrator.helloworld;

import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ComposePathEffect;
import android.graphics.CornerPathEffect;
import android.graphics.DashPathEffect;
import android.graphics.DiscretePathEffect;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PathDashPathEffect;
import android.graphics.PathEffect;
import android.os.Bundle;
import android.view.View;

/**
 * Created by Administrator on 2017/5/3.
 */

public class MainActivityCanvasPathEffectTest1 extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new MyView(this));
    }

    public class MyView extends View {
        float m_phase;
        PathEffect[] m_effects = new PathEffect[7];
        int[] m_colors;
        private Paint m_paint = new Paint();
        Path m_path = new Path();

        @Override
        protected void onDraw(Canvas canvas) {
           super.onDraw(canvas);
            /* 将背景填充成白色 */
            canvas.drawColor(Color.WHITE);
            /* ------------下面开初始化7种路径效果------------ */
            /* 不使用路径效果 */
            m_effects[0] = null;
            /* 使用CornerPathEffect */
            m_effects[1] = new CornerPathEffect(10);
            /* 初始化DiscretePathEffect */
            m_effects[2] = new DiscretePathEffect(3.0f, 5.0f);
            /* 初始化DashPathEffect */
            m_effects[3] = new DashPathEffect(new float[] {20, 10, 5, 10}, m_phase);
            /* 初始化PathDashPathEffect */
            m_path.addRect(0, 0, 8, 8, Path.Direction.CCW);
            m_effects[4] = new PathDashPathEffect(m_path, 12, m_phase, PathDashPathEffect.Style.ROTATE);
            /* 初始化ComposePathEffect */
            m_effects[5] = new ComposePathEffect(m_effects[2], m_effects[4]);
            m_effects[6] = new ComposePathEffect(m_effects[2], m_effects[3]);
            /* 将画面移到(8, 8)处开始绘制 */
            canvas.translate(8, 8);

            /* 依次使用7种不同的路径效果、7种不同的颜色来绘制路径 */
            for (int i = 0; i < m_effects.length; ++i) {
                m_paint.setPathEffect(m_effects[i]);
                m_paint.setColor(m_colors[i]);
                canvas.drawPath(m_path, m_paint);
                canvas.translate(0, 60);
            }
            /* 改变phase值，形成动画效果 */
            m_phase += 1;
            invalidate();
        }

        public MyView(Context context) {
            super(context);
            m_paint.setStyle(Paint.Style.STROKE);
            m_paint.setStrokeWidth(4);
            /* 创建并初始化Path */
            m_path.moveTo(0, 0);
            for (int i = 1; i <= 40; ++i) {
                /* 生成40个点，随机生成他们的y坐标，并将他们连成一条Path */
                m_path.lineTo(i * 20, (float)Math.random() * 60);
            }
            /* 初始化7个颜色 */
            m_colors = new int[] {Color.BLACK, Color.BLUE, Color.CYAN, Color.GREEN, Color.MAGENTA, Color.RED, Color.YELLOW};

        }
    };
}
