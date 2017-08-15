package com.example.administrator.helloworld;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

/**
 * Created by Administrator on 2017/4/25.
 */

public class SecondaryStartActivity extends MainActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_activity_start2);
        Button button2 = (Button) findViewById(R.id.idButton2);
        Button button3 = (Button) findViewById(R.id.idButton3);
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(SecondaryStartActivity.this, MainActivityStartActivity.class);
                startActivity(intent);
            }
        });

        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(SecondaryStartActivity.this, MainActivityStartActivity.class);
                startActivity(intent);
                finish();
            }
        });
    }
}
