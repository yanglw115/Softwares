package com.example.administrator.helloworld;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.RandomAccessFile;

/**
 * Created by Administrator on 2017/5/4.
 */

public class MainActivityFileInputOutputStreamTest extends Activity {
    final String f_strFileName = "FileReadWriteTest.bin";
    private EditText editTextRead = null;
    private EditText editTextWrite = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_fileinput_fileoutput_stream);

        Button buttonWrite = (Button) findViewById(R.id.idButtonFileWrite);
        Button buttonRead = (Button) findViewById(R.id.idButtonFileRead);

        editTextRead = (EditText) findViewById(R.id.idEditTextFileRead);
        editTextWrite = (EditText) findViewById(R.id.idEditTextFileWrite);

        buttonWrite.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                write();
                editTextWrite.setText("");
            }
        });

        buttonRead.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                read();
            }
        });
    }

    private void write()
    {
        /* 判断SD卡是否存在，需要在Manifest.xml设置相关权限 */
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            /* 获取SD卡路径 */
            File dirSDCard = Environment.getExternalStorageDirectory();
            try {
                /* 注意这里获取文件夹路径后，需要加上"/"，否则文件路径出错 */
                File fileTarget = new File(dirSDCard.getCanonicalPath() + "/" + f_strFileName);
                RandomAccessFile raf = new RandomAccessFile(fileTarget, "rw");
                raf.seek(fileTarget.length());
                raf.write(editTextWrite.getText().toString().getBytes());
                raf.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    private void read()
    {
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            File dirSDCard = Environment.getExternalStorageDirectory();
            try {
                FileInputStream inputStream = new FileInputStream(dirSDCard.getCanonicalPath() + "/" + f_strFileName);
                BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));
                StringBuilder sb = new StringBuilder("");
                String line = null;
                while ((line = br.readLine()) != null) {
                    sb.append(line);
                }
                br.close();
                editTextRead.setText(sb.toString());
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
