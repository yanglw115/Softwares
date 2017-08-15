package com.example.administrator.helloworld;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

/**
 * Created by Administrator on 2017/5/5.
 */

/* SQLiteOpenHelper主要用来管理数据库的打开、创建、更新与关闭等 */
public class MySQLiteOpenHelper extends SQLiteOpenHelper {
    final  String f_CRATE_TABLE_SQL = "create table dict(_id integer primary key autoincrement, word, detail)";

    public MySQLiteOpenHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        super(context, name, factory, version);

    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        /* 如果还没有创建对应的数据库表时则系统自动调用 */
        db.execSQL(f_CRATE_TABLE_SQL);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        System.out.println("-------------onUpgrade--------------" + oldVersion + ": " + newVersion);
    }
}
