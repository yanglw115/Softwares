package com.example.administrator.helloworld;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;

/**
 * Created by Administrator on 2017/4/24.
 */

public class MainActivityAsyncTask extends Activity {
    private TextView textShow;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        super.setContentView(R.layout.layout_asynctask);
        textShow = (TextView) findViewById(R.id.idTextViewAsyncTask);
    }

    public void onStartClick(View source)
    {
        DownTask task = new DownTask(this);
        try {
            task.execute(new URL("http://www.crazyit.org/ethos.php"));
        } catch (Exception e) {
            e.printStackTrace();
        }
     }

    class DownTask extends AsyncTask<URL, Integer, String> {
        ProgressDialog progressDialog;
        int hasRead;
        Context mContext;
        public DownTask(Context context)
        {
            mContext = context;
        }

        @Override
        protected String doInBackground(URL... params) {
            StringBuilder sb = new StringBuilder();
            try {
                URLConnection conn = params[0].openConnection();
                BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream(), "utf-8"));
                String line = null;
                while ((line = br.readLine()) != null) {
                    sb.append(line + "\n");
                    hasRead++;
                    publishProgress(hasRead);
                    Thread.currentThread().sleep(200);
                }
                return sb.toString();
            } catch (Exception e) {
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onPreExecute() {
            progressDialog = new ProgressDialog(mContext);
            progressDialog.setTitle("任务正在执行中");
            progressDialog.setMessage("任务正在执行中，请耐心等待...");
            progressDialog.setCancelable(false);
            progressDialog.setMax(202);
            progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
            progressDialog.setIndeterminate(false);
            progressDialog.show();
        }

        @Override
        protected void onPostExecute(String s) {
            textShow.setText(s);
            progressDialog.dismiss();
        }

        @Override
        protected void onProgressUpdate(Integer... values) {
            textShow.setText("已经读取了["+ values[0] +"]行");
            progressDialog.setProgress(values[0]);
        }
    };
}
