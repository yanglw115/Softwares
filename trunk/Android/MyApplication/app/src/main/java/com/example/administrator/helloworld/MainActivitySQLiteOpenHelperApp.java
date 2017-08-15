package com.example.administrator.helloworld;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by Administrator on 2017/5/5.
 */

public class MainActivitySQLiteOpenHelperApp extends Activity {
    MySQLiteOpenHelper m_dbHelper = null;
    Button m_buttonInsert = null;
    Button m_buttonSearch = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_sqliteopenhelper_main);

        /* 创建SQLiteOpenHelper实例，并在其中创建对应数据库表 */
        m_dbHelper = new MySQLiteOpenHelper(this, "my.db4", null, 1);

        m_buttonInsert = (Button) findViewById(R.id.idHelperButtonAdd);
        m_buttonSearch = (Button) findViewById(R.id.idHelperButtonSearch);

        m_buttonInsert.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String strWorld = ((EditText) findViewById(R.id.idHelperWord)).getText().toString();
                String strDetail = ((EditText) findViewById(R.id.idHelperDetail)).getText().toString();

                insertData(m_dbHelper.getReadableDatabase(), strWorld, strDetail);

                Toast.makeText(MainActivitySQLiteOpenHelperApp.this, "添加生词成功!", Toast.LENGTH_SHORT).show();
            }
        });

        m_buttonSearch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String strKey = ((EditText) findViewById(R.id.idHelperSearchWord)).getText().toString();
                /* 执行查询sql */
                Cursor cursor = m_dbHelper.getReadableDatabase().rawQuery("select * from dict where word like ? or detail like ?",
                        new String[]{"%" + strKey + "%", "%" + strKey + "%"});

                /* 创建一个Bundle对象 */
                Bundle data = new Bundle();
                data.putSerializable("data", convertCursorToList(cursor));

                /* 创建一个Intent */
                Intent intent = new Intent(MainActivitySQLiteOpenHelperApp.this,
                        ActivitySQLiteOpenHelperResult.class);
                intent.putExtras(data);
                /* 启动Activity */
                startActivity(intent);
            }
        });
    }

    private Serializable convertCursorToList(Cursor cursor)
    {
        ArrayList<Map<String, String>> arrayList = new ArrayList<Map<String, String>>();
        /* 遍历Cursor结果集 */
        while (cursor.moveToNext()) {
            Map<String, String> listItem = new HashMap<String, String>();
            listItem.put("word", cursor.getString(1));
            listItem.put("detail", cursor.getString(2));
            arrayList.add(listItem);
        }
        return arrayList;
    }

    private void insertData(SQLiteDatabase readableDatabase, String strWorld, String strDetail)
    {
        readableDatabase.execSQL("insert into dict values(null, ?, ?)", new String[] {strWorld, strDetail});
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (m_dbHelper != null) {
            /* SQLDatabase这时通过SQLiteOpenHelper进行关闭 */
            m_dbHelper.close();
        }
    }
}
