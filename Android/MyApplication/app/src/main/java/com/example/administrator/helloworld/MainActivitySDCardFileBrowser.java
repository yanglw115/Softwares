package com.example.administrator.helloworld;

import android.app.Activity;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2017/5/4.
 */

public class MainActivitySDCardFileBrowser extends Activity {
    ListView m_listView = null;
    TextView m_textView = null;
    /* 记录当前的父文件夹 */
    File m_folderParent = null;
    File[] m_fileAll = null;
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_sdcard_file_browser);

        m_listView = (ListView) findViewById(R.id.idListAllFiles);
        m_textView = (TextView) findViewById(R.id.idTextViewCurrentPath);

        /* 获取系统SD卡路径 */
        String strSDCard = Environment.getExternalStorageDirectory().toString();
        File dirRoot = new File(strSDCard);
        /* 如果SD卡路径存在 */
        if (dirRoot.exists()) {
            m_folderParent = dirRoot;
            m_fileAll = dirRoot.listFiles();
            inflateListView(m_fileAll);
        }

        /* 为ListView的列表项的单击事件绑定监听器 */
        m_listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                if (m_fileAll[position].isFile())
                    return;
                File[] tmp = m_fileAll[position].listFiles();
                if (tmp == null || tmp.length == 0) {
                    Toast.makeText(MainActivitySDCardFileBrowser.this, "当前路径下没有文件或不可访问",
                            Toast.LENGTH_SHORT).show();
                } else {
                    /* 获取当前单击路径的文件夹为父文件夹 */
                    m_folderParent = m_fileAll[position];
                    m_fileAll = tmp;
                    inflateListView(m_fileAll);
                }
            }
        });

        Button button = (Button) findViewById(R.id.idButtonReturn);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    if (!m_folderParent.getCanonicalPath().equals("/mnt/sdcard")) {
                        m_folderParent = m_folderParent.getParentFile();
                        m_fileAll = m_folderParent.listFiles();
                        inflateListView(m_fileAll);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    private void inflateListView(File[] files) {
        /* 创建一个List集合，list集合的元素是map */
        List<Map<String, Object>> listItems = new ArrayList<Map<String, Object>>();

        for (int i = 0; i < files.length; ++i) {
            Map<String, Object> listItem = new HashMap<String, Object>();
            /* 如果当前是文件夹则使用文件夹图标，否则使用文件图标 */
            if (files[i].isFile()) {
                listItem.put("icon", R.drawable.file);
            } else {
                listItem.put("icon", R.drawable.folder);
            }

            listItem.put("fileName", files[i].getName());
            listItems.add(listItem);
        }

        /* 创建一个SimpleAdapter */
        SimpleAdapter simpleAdapter = new SimpleAdapter(this, listItems, R.layout.layout_sdcard_file_browser_line,
                new String[]{"icon", "fileName"}, new int[]{R.id.idFileIcon, R.id.idFileName});
        /* 为ListView设置Adapter */
        m_listView.setAdapter(simpleAdapter);

        try {
            m_textView.setText("当前路径为: " + m_folderParent.getCanonicalPath());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    public Action getIndexApiAction() {
        Thing object = new Thing.Builder()
                .setName("MainActivitySDCardFileBrowser Page") // TODO: Define a title for the content shown.
                // TODO: Make sure this auto-generated URL is correct.
                .setUrl(Uri.parse("http://[ENTER-YOUR-URL-HERE]"))
                .build();
        return new Action.Builder(Action.TYPE_VIEW)
                .setObject(object)
                .setActionStatus(Action.STATUS_TYPE_COMPLETED)
                .build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        AppIndex.AppIndexApi.start(client, getIndexApiAction());
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        AppIndex.AppIndexApi.end(client, getIndexApiAction());
        client.disconnect();
    }
}
