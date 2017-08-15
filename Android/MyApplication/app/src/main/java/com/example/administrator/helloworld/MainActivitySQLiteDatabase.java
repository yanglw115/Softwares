package com.example.administrator.helloworld;

import android.app.Activity;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CursorAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

/**
 * Created by Administrator on 2017/5/4.
 */

public class MainActivitySQLiteDatabase extends Activity {
    SQLiteDatabase m_db = null;
    ListView m_listView = null;
    EditText m_editTextTitle = null;
    EditText m_editTextContent = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_sqlite_database);

        m_listView = (ListView) findViewById(R.id.idDBListView);
        Button buttonInsert = (Button) findViewById(R.id.idDBButtonInsert);
        m_editTextTitle = (EditText) findViewById(R.id.idDBEditTitle);
        m_editTextContent = (EditText) findViewById(R.id.idDBEditContent);

        /* 使用默认的CursorFactory */
        m_db = SQLiteDatabase.openOrCreateDatabase(this.getFilesDir().toString() + "my.db3", null);

        buttonInsert.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String strTitle = m_editTextTitle.getText().toString();
                String strContent = m_editTextContent.getText().toString();

                try {
                    insertData(m_db, strTitle, strContent);
                    /* SQLiteDatabase.rawQuery返回的就是一个Cursor */
                    Cursor cursor = m_db.rawQuery("select * from news_inf", null);
                    inflateList(cursor);

                } catch (SQLiteException e) {
                    /* 建表 */
                    /* 如果之前没有建表，使用此方法避免出错，如果使用SQLOpenHelper的子类则不必要 */
                    m_db.execSQL("create table news_inf(_id integer primary key autoincrement, " +
                            "news_title varchar(50), " +
                            "news_content varchar(255))");
                    /* 再次执行插入 */
                    insertData(m_db, strTitle, strContent);
                    /* 执行查询 */
                    Cursor cursor = m_db.rawQuery("select * from news_inf", null);
                    inflateList(cursor);
                }
            }
        });
    }

    private void inflateList(Cursor cursor)
    {
        /* 填充SimpleCursorAdapter */
        /* 使用SimpleCursorAdapter需要注意数据库表的主键列的列名只能是_id,否则程序会出错 */
        SimpleCursorAdapter adapter = new SimpleCursorAdapter(MainActivitySQLiteDatabase.this,
                R.layout.layout_sqlite_database_line, cursor,
                new String[] {"news_title", "news_content"}, new int[]{R.id.idDBLineTitle, R.id.idDBLineContent},
                CursorAdapter.FLAG_REGISTER_CONTENT_OBSERVER);
        m_listView.setAdapter(adapter);
        m_editTextTitle.setText("");
        m_editTextContent.setText("");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        /* 对象销毁时需要close数据库，否则会出现资源泄漏 */
        if (m_db != null && m_db.isOpen()) {
            m_db.close();
        }
    }

    private void insertData(SQLiteDatabase db, String strTitle, String strContent)
    {
        /* 插入执行语句 */
        /* ?是点位符，其值在后面的String数组中给出 */
        db.execSQL("insert into news_inf values(null, ?, ?)", new String[] {strTitle, strContent});
    }
}
