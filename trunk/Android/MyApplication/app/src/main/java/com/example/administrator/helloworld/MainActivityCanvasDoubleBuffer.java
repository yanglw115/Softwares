package com.example.administrator.helloworld;

import android.app.Activity;
import android.graphics.BlurMaskFilter;
import android.graphics.Color;
import android.graphics.EmbossMaskFilter;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.ViewConfiguration;
import android.widget.LinearLayout;

import java.lang.reflect.Field;

/**
 * Created by Administrator on 2017/5/3.
 */

public class MainActivityCanvasDoubleBuffer extends Activity {
    EmbossMaskFilter m_emboss;
    BlurMaskFilter m_blur;
    ViewCanvasDoubleBuffer m_drawView;

    private void getOverFlowMenu()
    {
        ViewConfiguration viewConfiguration = ViewConfiguration.get(this);
        try {
            Field overFlowMenuFiled = ViewConfiguration.class.getDeclaredField("sHasPermanentMenuKey");
            if (null != overFlowMenuFiled) {
                overFlowMenuFiled.setAccessible(true);
                overFlowMenuFiled.setBoolean(viewConfiguration, false);
            }
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //getOverFlowMenu();
        //requestWindowFeature(Window.FEATURE_OPTIONS_PANEL | Window.FEATURE_ACTION_BAR);
        LinearLayout layout = new LinearLayout(this);
        DisplayMetrics displayMetrics = new DisplayMetrics();
        /* 获取创建的宽度和高度 */
        getWindowManager().getDefaultDisplay().getRealMetrics(displayMetrics);
        /* 创建一个自定义的DrawView，宽度与高度与该Activity相同 */
        m_drawView = new ViewCanvasDoubleBuffer(this, displayMetrics.widthPixels, displayMetrics.heightPixels);
        layout.addView(m_drawView);
        setContentView(layout);
        m_emboss = new EmbossMaskFilter(new float[] {1.5f, 1.5f, 1.5f}, 0.6f, 6, 4.2f);
        m_blur = new BlurMaskFilter(8, BlurMaskFilter.Blur.NORMAL);
    }

    /* menu菜单并未显示出来，不知道为什么 */
    /* 负责创建菜单项 */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = new MenuInflater(this);
        /* 加载菜单并添加到menu当中 */
        inflater.inflate(R.menu.menu_canvas_doublebuffer, menu);
        return true;//super.onCreateOptionsMenu(menu);
    }

    /* 菜单项被单击后的回调方法 */

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.idColorRed:
                m_drawView.m_paint.setColor(Color.RED);
                item.setChecked(true);
                break;
            case R.id.idColorGreen:
                m_drawView.m_paint.setColor(Color.GREEN);
                item.setChecked(true);
                break;
            case R.id.idColorBlue:
                m_drawView.m_paint.setColor(Color.BLUE);
                item.setChecked(true);
                break;
            case R.id.idWidth1:
                m_drawView.m_paint.setStrokeWidth(1);
                break;
            case R.id.idWidth3:
                m_drawView.m_paint.setStrokeWidth(3);
                break;
            case R.id.idWidth5:
                m_drawView.m_paint.setStrokeWidth(5);
                break;
            case R.id.idBlur:
                m_drawView.m_paint.setMaskFilter(m_blur);
                break;
            case R.id.idEmboss:
                m_drawView.m_paint.setMaskFilter(m_emboss);
                break;
        }
        return true;
    }
}
