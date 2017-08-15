package com.example.administrator.helloworld;

import android.app.Activity;
import android.os.Bundle;

/**
 * Created by Administrator on 2017/5/3.
 */

public class MainActivityCavasTest extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ViewCanvasTest canvasTest = new ViewCanvasTest(this);
        this.setContentView(canvasTest);
    }
}
