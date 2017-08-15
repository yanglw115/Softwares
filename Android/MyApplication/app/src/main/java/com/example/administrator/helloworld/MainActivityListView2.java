package com.example.administrator.helloworld;

import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

/**
 * Created by Administrator on 2016/11/7.
 */
// 这里也可以直接继承ListActivity类，这样就不需要setContentView()，而是直接setAdapter
public class MainActivityListView2 extends MainActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_listview2);

        String[] array1 = {"孙悟空", "猪八戒", "牛魔王"};
        ArrayAdapter<String> adapter1 = new ArrayAdapter<String>(this, R.layout.layout_array_textview1, array1);
        ListView listView1 = (ListView) findViewById(R.id.idListView01);
        listView1.setAdapter(adapter1);

        String[] array2 = {"Week", "Spring", "Month"};
        ArrayAdapter<String> adapter2 = new ArrayAdapter<String>(this, R.layout.layout_array_textview2, array2);
        ListView listView2 = (ListView) findViewById(R.id.idListView02);
        listView2.setAdapter(adapter2);
    }
}
