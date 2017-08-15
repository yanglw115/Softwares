package com.example.administrator.helloworld;

import android.app.Activity;
import android.os.Bundle;
import android.widget.QuickContactBadge;

/**
 * Created by Administrator on 2016/10/25.
 */

public class MainActivityQuickContactBadge extends Activity {
    QuickContactBadge contactBadge;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_quickcontactbadge);
        contactBadge = (QuickContactBadge) findViewById(R.id.idBadge);
//        将QucickContactBadge与电话号码绑定关系，当点击QuickContactBadge时，自动打开对应联系人界面
        contactBadge.assignContactFromPhone("18866666666", false);
    }
}
