package com.example.administrator.helloworld;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.SimpleAdapter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2017/4/20.
 */

public class MainActivityImagePreview extends Activity {
//    int [] imageIDs = new int[] {
//            R.drawable.image1, R.drawable.image2, R.drawable.image3, R.drawable.image4,
//            R.drawable.image5, R.drawable.image6, R.drawable.image7, R.drawable.image8,
//            R.drawable.image9, R.drawable.image10, R.drawable.image11, R.drawable.image12
//    };
    int[] imageIDs = new int[]{
            R.drawable.image1, R.drawable.image2, R.drawable.image3, R.drawable.image4,
            R.drawable.image5, R.drawable.image6, R.drawable.image7, R.drawable.image8,
            R.drawable.image9, R.drawable.image10, R.drawable.image11, R.drawable.image12
    };
    GridView gridView;
    ImageView imageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_imagepreview);
        List<Map<String, Object>> listItems = new ArrayList<Map<String, Object>>();
        for (int i = 0; i < imageIDs.length; ++i) {
            Map<String, Object> item = new HashMap<String, Object>();
            item.put("image", imageIDs[i]);
            listItems.add(item);
        }

        imageView = (ImageView) findViewById(R.id.idImageView);
        SimpleAdapter simpleAdapter = new SimpleAdapter(this, listItems, R.layout.layout_single_image,
                new String[] {"image"}, new int[] {R.id.idImageShow});

        gridView = (GridView) findViewById(R.id.idGridView);
        gridView.setAdapter(simpleAdapter);

        gridView.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener()
        {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                imageView.setImageResource(imageIDs[position]);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        gridView.setOnItemClickListener(new AdapterView.OnItemClickListener()
        {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                imageView.setImageResource(imageIDs[position]);
            }
        });
    }
}
