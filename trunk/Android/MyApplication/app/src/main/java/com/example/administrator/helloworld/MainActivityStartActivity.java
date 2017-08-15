package com.example.administrator.helloworld;

import android.content.ComponentName;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

/**
 * Created by Administrator on 2017/4/25.
 */

public class MainActivityStartActivity extends MainActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_activity_start1);

        Button button = (Button) findViewById(R.id.idButton1);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /* 下面为什么要用MainActivityStartActivity.this而不是直接的this，是因为如果使用this的话，
                 * 则该this的对象是匿名的，但是这里会发生歧义，因为有两个对象存在，一个是MainActivityStartActivity，
                  * 另一个是new出来的View.onClickListener */
                /* 另外说明：Activity也是继承Context的，所以可以在ComponetName中直接使用它作为参数 */
                ComponentName componentName = new ComponentName(MainActivityStartActivity.this, SecondaryStartActivity.class);
                Intent intent = new Intent();
//                intent.setComponent(componentName);
//                intent.setAction("administrator.intent.action.ACTION_SECONDARY");
                /* 也可以单独通过data属性来启动Activity */
                intent.setData(Uri.parse("yangliwei://www.baidu.com/test"));
                Toast.makeText(MainActivityStartActivity.this, intent.toString(), Toast.LENGTH_LONG).show();
                {
                    /* 或者使用下面的方法是一样的，都叫显式Intent，都设置了包与类 */
//                  Intent intent = new Intent(MainActivityStartActivity.this, SecondaryStartActivity.class);
                }
                startActivity(intent);
            }
        });
    }
}
