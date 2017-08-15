package com.example.administrator.helloworld;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Administrator on 2017/5/2.
 */

public class MainActiviyBitmapTest extends Activity {
    String[] images = null;
    ImageView imageView = null;
    TextView textView = null;
    Button button = null;
    AssetManager am = null;
    int indexImg = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_bitmap_test);

        button = (Button)findViewById(R.id.idBitmapShowNext);
        imageView = (ImageView)findViewById(R.id.idBitmapView);
        textView = (TextView)findViewById(R.id.idBitmapName);

        am = getAssets();
        try {
            images = am.list("");
        } catch (IOException e) {
            e.printStackTrace();
        }

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (indexImg >= images.length) {
                    indexImg = 0;
                }
                while (!images[indexImg].endsWith(".png")
                        && !images[indexImg].endsWith(".jpg")
                        && !images[indexImg].endsWith(".gif")) {
                    ++indexImg;
                    if (indexImg >= images.length) {
                        indexImg = 0;
                    }
                }
                InputStream assetFile = null;
                try {
                    /* assetManager提供了open与openFd()两种方法获取原始数据 */
                    assetFile = am.open(images[indexImg++]);
                } catch (IOException e) {
                    e.printStackTrace();
                }

                BitmapDrawable drawable = (BitmapDrawable) imageView.getDrawable();
                if (drawable != null && drawable.getBitmap().isRecycled()) {
                    drawable.getBitmap().recycle();
                }
                imageView.setImageBitmap(BitmapFactory.decodeStream(assetFile));
                textView.setText(images[indexImg]);
            }
        });

    }
}
