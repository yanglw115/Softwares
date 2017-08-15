package com.example.administrator.helloworld;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RectF;
import android.graphics.Shader;
import android.view.View;

/**
 * Created by Administrator on 2017/5/3.
 */

public class ViewCanvasTest extends View {
    Paint m_paint = new Paint();
    RectF m_rectF1 = new RectF();
    RectF m_rectF2 = new RectF();
    Path m_path1 = new Path();
    Path m_path2 = new Path();
    Path m_path3 = new Path();
    Path m_path4 = new Path();
    Shader m_shader = new LinearGradient(0, 0, 40, 60, new int[] {Color.RED, Color.GREEN, Color.BLUE, Color.YELLOW},
            null, Shader.TileMode.REPEAT);

    public ViewCanvasTest(Context context) {
        super(context);
    }
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        canvas.drawColor(Color.WHITE);
        //Paint m_paint = new Paint();
        /* 去锯齿 */
        m_paint.setAntiAlias(true);
        m_paint.setColor(Color.BLUE);
        m_paint.setStyle(Paint.Style.STROKE);
        m_paint.setStrokeWidth(4);

        int viewWidth = this.getWidth();
        /* 绘制圆形 */
        canvas.drawCircle(viewWidth * 3 / 10 + 10, /* 圆心x坐标 */
                viewWidth / 10 + 10, /* 圆心y坐标 */
                viewWidth / 10, m_paint); /* 半径 <= 0时什么也画不出 */

        /* 绘制正方形 */
        canvas.drawRect(viewWidth / 5 + 20, /* 左上角X坐标 */
                viewWidth / 5 + 20, /* 左上角y坐标 */
                viewWidth * 2 / 5 + 20, /* 右下角x坐标 */
                viewWidth * 2 / 5 + 20, m_paint); /* 右下角y坐标 */

        /* 绘制矩形 */
        canvas.drawRect(viewWidth / 5 + 20, /* 左上角X坐标 */
                viewWidth *2 / 5 + 30, /* 左上角y坐标 */
                viewWidth * 2 / 5 + 20, /* 右下角x坐标 */
                viewWidth / 2 + 30, m_paint); /* 右下角y坐标 */

        /* 绘制圆角矩形 */
        m_rectF1.set(viewWidth / 5 + 20,
                viewWidth / 2 + 40,
                viewWidth * 2 / 5 + 20,
                viewWidth * 3 / 5 + 40);
        canvas.drawRoundRect(m_rectF1,
                15, /* x方向半径 */
                15, m_paint); /* y方向半径 */

        /* 绘制椭圆 */
        m_rectF2.set(20 + viewWidth / 5,
                viewWidth * 3 / 5 + 50,
                20 + viewWidth * 2 / 5,
                viewWidth * 7 / 10 + 50);
        canvas.drawOval(m_rectF2, m_paint);

        /* 定义一个path对象，封闭成一个三角形 */
        m_path1.moveTo(20 + viewWidth / 5, viewWidth * 9 / 10 + 60);
        m_path1.lineTo(viewWidth * 2 / 5 + 20, viewWidth * 9 / 10 + 60);
        m_path1.lineTo(viewWidth * 3 / 10 + 20, viewWidth * 7 / 10 + 60);
        m_path1.close();
        /* 根据path进行绘制，绘制三角形 */
        canvas.drawPath(m_path1, m_paint);

        /* 根据Path绘制五角形 */
        m_path2.moveTo(20 + viewWidth * 4 / 15, viewWidth * 9 / 10 + 70);
        m_path2.lineTo(20 + viewWidth / 3, viewWidth * 9 / 10 + 70);
        m_path2.lineTo(20 + viewWidth * 2 / 5, viewWidth + 70);
        m_path2.lineTo(20 + viewWidth * 3 / 10, viewWidth * 11 / 10 + 70);
        m_path2.lineTo(20 + viewWidth / 5, viewWidth + 70);
        m_path2.close();
        canvas.drawPath(m_path2, m_paint);

        /* --------------设置渐变器后绘制--------------------*/
        m_paint.setShader(m_shader);
        /* 设置阴影 */
        m_paint.setShadowLayer(25, 20, 20, Color.GRAY);
        /* 绘制圆形 */
        canvas.drawCircle(viewWidth / 2 + 30, viewWidth / 10 + 10, viewWidth / 10, m_paint);
        /* 绘制正方形 */
        canvas.drawRect(viewWidth * 2 / 5 + 30, viewWidth / 5 + 20, viewWidth * 3 / 5 + 20, viewWidth * 2 / 5 + 20, m_paint);
        /* 绘制矩形 */
        canvas.drawRect(viewWidth * 2 / 5 + 30, viewWidth * 2 / 5 + 30, viewWidth * 3 / 5 + 30, viewWidth / 2 + 30, m_paint);
        /* 绘制圆角矩形 */
        /* m_rectF1与m_rectF2可以与上面的共用一个 */
        m_rectF1.set(viewWidth * 2 / 5 + 30, viewWidth / 2 + 40, 30 + viewWidth * 3 / 5, viewWidth * 3 / 5 + 40);
        canvas.drawRoundRect(m_rectF1, 15, 15, m_paint);
        /* 绘制椭圆 */
        m_rectF2.set(30 + viewWidth * 2 / 5, viewWidth * 3 / 5 + 50, 30 + viewWidth * 3 / 5, viewWidth * 7 / 10 + 50);
        canvas.drawOval(m_rectF2, m_paint);
        /* 绘制三角形 */
        /* m_path3与m_path4不能与上面的m_path1,m_path2共用，否则上面的也变了 */
        m_path3.moveTo(30 + viewWidth * 2 / 5, viewWidth * 9 / 10 + 60);
        m_path3.lineTo(viewWidth * 3 / 5 + 30, viewWidth * 9 / 10 + 60);
        m_path3.lineTo(viewWidth / 2 + 30, viewWidth * 7 / 10 + 60);
        m_path3.close();
        canvas.drawPath(m_path3, m_paint);
        /* 绘制五角形 */
        m_path4.moveTo(30 + viewWidth * 7 / 15, viewWidth * 9 / 10 + 70);
        m_path4.lineTo(30 + viewWidth * 8 / 15, viewWidth * 9 / 10 + 70);
        m_path4.lineTo(30 + viewWidth * 3 / 5, viewWidth + 70);
        m_path4.lineTo(30 + viewWidth / 2, viewWidth * 11 / 10 + 70);
        m_path4.lineTo(30 + viewWidth * 2 / 5, viewWidth + 70);
        m_path4.close();
        canvas.drawPath(m_path4, m_paint);
    }
}
