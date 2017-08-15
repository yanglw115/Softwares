package com.example.administrator.helloworld;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {


    int [] images = new int [] {
            R.mipmap.a,
            R.mipmap.b,
            R.mipmap.c,
            R.mipmap.d,
            R.mipmap.c,
            R.mipmap.e
    };
    int currentImg = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_analog_text_clock);
//        LinearLayout main = (LinearLayout) findViewById(R.id.root);

/* iamge show */
//        final ImageView image = new ImageView(this);
//        main.addView(image);
//        image.setImageResource(images[0]);
//        image.setOnClickListener(new View.OnClickListener()
//        {
//            @Override
//            public void onClick(View v)
//            {
//                image.setImageResource(images[++currentImg % images.length]);
//            }
//        }
//        );

        /* point move */
//        final DrawView view = new DrawView(this);
//        view.setMinimumWidth(300);
//        view.setMinimumHeight(500);
//        main.addView(view);
    }
//    @Override
//    public boolean onCreateOptionsMenu(Menu menu)
//    {
//        getMenuInflater().inflate(R.menu.menu_main, menu);
//        return true;
//    }

}
