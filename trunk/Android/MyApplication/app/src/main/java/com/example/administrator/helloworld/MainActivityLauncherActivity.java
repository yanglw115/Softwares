package com.example.administrator.helloworld;

import android.app.LauncherActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;

/**
 * Created by Administrator on 2017/4/25.
 */

public class MainActivityLauncherActivity extends LauncherActivity {
    String names[] = new String[] {
            "设置程序参数", "查看星际兵种"
    };
    /* 通过Intent来启动其他Activity */
    Class<?>[] clazzs = {PreferenceActivityTest.class, ExpandableListActivityTest.class};

    @Override
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                /* 注意：下面使用的是Android SDK自带的layout */
                android.R.layout.simple_list_item_1, names);
        setListAdapter(adapter);
    }

    @Override
    protected Intent intentForPosition(int position) {
        return new Intent(MainActivityLauncherActivity.this, clazzs[position]);
    }
}
