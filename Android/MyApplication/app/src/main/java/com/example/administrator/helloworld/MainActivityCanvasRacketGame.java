package com.example.administrator.helloworld;

import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by Administrator on 2017/5/3.
 */

public class MainActivityCanvasRacketGame extends Activity {
    /* 游戏主面板的宽与高 */
    private int m_widthTable;
    private int m_heightTable;
    /* 球拍y坐标 */
    private int m_yRacket;
    /* 球拍长与宽 */
    private final int f_RACKET_WIDTH = 90;
    private final int f_RACKET_HEIGHT = 30;
    private final int f_BALL_SIZE = 16;
    /* y轴速度 */
    private int m_ySpeed = 15;
    Random m_random = new Random();
    /* 返回一个-0.5~0.5的比率，用于控制小球的运行方向 */
    private double m_xyRate = m_random.nextDouble() - 0.5;
    /* 小球横向的运行速度 */
    private int m_xSpeed = (int)(m_ySpeed * m_xyRate * 2);
    private int m_xBall = m_random.nextInt(200) + 20;
    private int m_yBall = m_random.nextInt(10) + 20;
    private int m_xRacket = m_random.nextInt(200);
    private boolean m_bGameOver = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        /* 去掉窗口标题 */
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        /* 全屏显示 */
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        /* 创建GameView组件 */
        final GameView gameView = new GameView(this);
        setContentView(gameView);

        /* 获取窗口管理器 */
        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        m_widthTable = metrics.widthPixels;
        m_heightTable = metrics.heightPixels;
        m_yRacket = m_heightTable - 80;
        final Handler handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == 0x123) {
                    gameView.invalidate();
                }
            }
        };

        gameView.setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                /* 获取由哪个键触发的事件 */
                switch (event.getKeyCode()) {
                    /* 控制挡板左移 */
                    case KeyEvent.KEYCODE_A:
                        if (m_xRacket > 0)
                            m_xRacket -= 10;
                        break;
                    /* 控制挡板右移 */
                    case KeyEvent.KEYCODE_D:
                        if (m_xRacket < m_widthTable - f_RACKET_WIDTH)
                            m_xRacket += 10;
                        break;
                    default:
                        break;
                }
                /* 通知GameView组件重绘 */
                gameView.invalidate();
                return true;
            }
        });

        final Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                /* 如果小球碰到左边边框 */
                if (m_xBall <= 0 || m_xBall >= m_widthTable - f_BALL_SIZE) {
                    m_xSpeed = -m_xSpeed;
                }
                if ((m_yBall >= m_yRacket - f_BALL_SIZE) && (m_xBall < m_xRacket || m_xBall > m_xRacket + f_RACKET_WIDTH)) {
                    timer.cancel();
                    /* 设置游戏是否结束标志 */
                    m_bGameOver = true;
                } else if (m_yBall <= 0
                        || (m_yBall >= m_yRacket - f_BALL_SIZE
                            && m_xBall > m_xRacket
                            && m_xBall <= m_xRacket + f_RACKET_WIDTH)) {
                    /* 小球位于球拍之内，并且到达球拍位置，小球反弹 */
                    m_ySpeed = -m_ySpeed;
                }
                /* 小球坐标增加 */
                m_xBall += m_xSpeed;
                m_yBall += m_ySpeed;
                /* 发送消息，通知系统重绘组件 */
                handler.sendEmptyMessage(0x123);

            }
        }, 0, 100);
    }
    class GameView extends View {
        Paint m_paint = new Paint();

        public GameView(Context context) {
            super(context);
            setFocusable(true);
        }

        /* 重写View的OnDraw方法 */

        @Override
        protected void onDraw(Canvas canvas) {
            //super.onDraw(canvas);
            m_paint.setStyle(Paint.Style.FILL);
            m_paint.setAntiAlias(true);
            /* 如果游戏已经结束 */
            if (m_bGameOver) {
                m_paint.setColor(Color.RED);
                m_paint.setTextSize(40);
                canvas.drawText("游戏已经结束", m_widthTable / 2 - 100, 200, m_paint);
            } else {
                /* 如果游戏还未结束 */
                /* 设置颜色并绘制小球 */
                m_paint.setColor(Color.rgb(255, 0, 0));
                canvas.drawCircle(m_xBall, m_yBall, f_BALL_SIZE, m_paint);
                /* 设置颜色并绘制球拍 */
                m_paint.setColor(Color.rgb(80, 80, 200));
                canvas.drawRect(m_xRacket, m_yRacket, f_RACKET_WIDTH + m_xRacket, f_RACKET_HEIGHT + m_yRacket, m_paint);
            }
        }
    };
}
