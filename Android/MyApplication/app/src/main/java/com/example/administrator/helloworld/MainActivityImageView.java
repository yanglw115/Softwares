package com.example.administrator.helloworld;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

/**
 * Created by Administrator on 2016/10/19.
 */

public class MainActivityImageView extends Activity {
    int [] images = new int[] {
            R.mipmap.a,
            R.mipmap.b,
            R.mipmap.c,
            R.mipmap.d,
            R.mipmap.e
    };

    int currentImg = 2;
    int alpha  = 255;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_image_view);

        final Button buttonPlus = (Button) findViewById(R.id.idButtonPlus);
        final Button buttonMinus = (Button) findViewById(R.id.idButtonMinus);
        Button buttonNext = (Button) findViewById(R.id.idButtonNext);
        final ImageView imageView1 = (ImageView) findViewById(R.id.idImageView01);
        final ImageView imageView2 = (ImageView) findViewById(R.id.idImageView02);

        buttonNext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                imageView1.setImageResource(images[++currentImg % images.length]);
            }
        });

        View.OnClickListener listener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (v == buttonPlus) {
                    alpha += 20;
                } else if (v == buttonMinus) {
                    alpha -= 20;
                }
                if (alpha > 255)
                    alpha = 255;
                else if (alpha < 0)
                    alpha = 0;
                imageView1.setImageAlpha(alpha);
            }
        };

        buttonPlus.setOnClickListener(listener);
        buttonMinus.setOnClickListener(listener);

        imageView1.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                BitmapDrawable bitmapDrawable = (BitmapDrawable) imageView1.getDrawable();
                Bitmap bitmap = bitmapDrawable.getBitmap();
                double scale = 1.0 * bitmap.getHeight() / imageView1.getHeight();
                int x = (int)(event.getX() * scale);
                int y = (int)(event.getY() * scale);
                if (x + 120 > imageView1.getWidth())
                    x -= 120;
                if (y + 120 > imageView1.getHeight())
                    y -= 120;
                imageView2.setImageBitmap(Bitmap.createBitmap(bitmap, x, y, 120, 120));
                imageView2.setImageAlpha(alpha);
                return false;
            }
        });

    }
}
