package com.example.administrator.helloworld;

import android.app.ActionBar;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.AdapterViewFlipper;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;

/**
 * Created by Administrator on 2017/4/24.
 */

public class MainActivityAdapterViewFlipper extends Activity {
    AdapterViewFlipper adapterViewFlipper;
//    ActionBar actionBar;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        super.setContentView(R.layout.layout_adapterviewflipper);

//        actionBar = getActionBar();
//        actionBar.show();

        adapterViewFlipper = (AdapterViewFlipper) findViewById(R.id.idAdapterViewFlipper);

        final int[] images = new int[] {
                R.drawable.image1, R.drawable.image2, R.drawable.image3, R.drawable.image4,
                R.drawable.image4, R.drawable.image5, R.drawable.image6, R.drawable.image7
        };

        Adapter adapter = new BaseAdapter() {
            @Override
            public int getCount() {
                return images.length;
            }

            @Override
            public Object getItem(int position) {
                return position;
            }

            @Override
            public long getItemId(int position) {
                return position;
            }

            @Override
            public View getView(int position, View convertView, ViewGroup parent) {
                ImageView imageView = new ImageView(MainActivityAdapterViewFlipper.this);
                imageView.setImageResource(images[position]);
                imageView.setScaleType(ImageView.ScaleType.FIT_XY);
                imageView.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));

                return imageView;
            }
        };
        adapterViewFlipper.setAdapter(adapter);

        Button button1 = (Button) findViewById(R.id.idButtonPreview);
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                prev();
            }
        });

        Button button2 = (Button) findViewById(R.id.idButtonNext);
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                next();
            }
        });

        Button button3 = (Button) findViewById(R.id.idButtonAuto);
        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                auto();
            }
        });
    }
    public void prev() {
        adapterViewFlipper.showPrevious();
        adapterViewFlipper.stopFlipping();
    }
    public void next() {
        adapterViewFlipper.showNext();
        adapterViewFlipper.stopFlipping();
    }
    public void auto() {
        adapterViewFlipper.startFlipping();
    }

}
