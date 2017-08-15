package com.example.administrator.helloworld;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by Administrator on 2017/5/3.
 */

public class MainActivitySharedPreferenceTest extends Activity {
    SharedPreferences m_shared = null;
    SharedPreferences.Editor m_edit = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_sharedpreference);

        /* SharedPreference不能实例化，只能通过getSharedPreferences获取 */
        m_shared = getSharedPreferences("VersionConfig", MODE_PRIVATE);
        m_edit = m_shared.edit();

        Button buttonRead = (Button) findViewById(R.id.idButtonReadShare);
        Button buttonWrite = (Button) findViewById(R.id.idButtonWriteShare);

        buttonRead.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String strTime = null;
                int nRandNum = -1;
                /* 读取数据的时候可以直接使得SharedPreferences的getxxx()方法 */
                if (m_shared.contains("time")) {
                    strTime = m_shared.getString("time", null);
                }
                if (m_shared.contains("random")) {
                    nRandNum = m_shared.getInt("random", -1);
                }
                String strResult = strTime == null? "你暂时还未写入数据": "写入时间为: " + strTime
                        + ", 写入的日期为: " + nRandNum;
                /* 使用Toast提示信息 */
                Toast.makeText(MainActivitySharedPreferenceTest.this, strResult, Toast.LENGTH_SHORT).show();

            }
        });

        buttonWrite.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SimpleDateFormat sdf = null;
                /* 写数据的时候只能使用SharedPreferences.edit()的相关方法进行写 */
                sdf = new SimpleDateFormat("yyyy年mm月dd日 " + "hh:mm:ss");
                /* 存入当前时间 */
                m_edit.putString("time", sdf.format(new Date()));
                m_edit.putInt("random", (int) (Math.random() * 100));
                m_edit.commit();
            }
        });
    }
}
