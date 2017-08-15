package com.example.administrator.helloworld;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ListView;
import android.widget.SimpleAdapter;

import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2017/5/5.
 */
public class ActivitySQLiteOpenHelperResult extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_sqliteopenhelper_result);

        ListView listView = (ListView) findViewById(R.id.idHelpResultListView);

        Intent intent = getIntent();
        Bundle data = intent.getExtras();

        /* @SupressWarnings下面那句先写了才可以使用这句: 不显示相关的警告 */
        @SuppressWarnings("unchecked")
        List<Map<String, String>> list = (List<Map<String, String>>)data.getSerializable("data");

        SimpleAdapter simpleAdapter = new SimpleAdapter(ActivitySQLiteOpenHelperResult.this, list,
                R.layout.layout_sqliteopenhelper_resulte_line, new String[] {"word", "detail"},
                new int[] {R.id.idHelperLineWord, R.id.idHelperLineDetail});
        listView.setAdapter(simpleAdapter);


    }
}
