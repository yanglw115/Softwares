package com.example.administrator.helloworld;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

import java.io.IOException;

/**
 * Created by Administrator on 2017/5/2.
 */

public class MainActivityResourceRawAssetTest extends Activity {
    MediaPlayer player1 = null;
    MediaPlayer player2 = null;
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_raw_asset);

        /* R.raw这里不能使用，在R类中并没有生成此成员，暂未查出原因 */
        //player1 =  MediaPlayer.create(this, R.raw.LetUs);
        //player1 = new MediaPlayer();

        AssetManager am = getAssets();
        try {
            AssetFileDescriptor afd1 = am.openFd("MusicSmile.mp3");
            AssetFileDescriptor afd2 = am.openFd("LetUs.mp3");
            player1 = new MediaPlayer();
            player2 = new MediaPlayer();
            player1.setDataSource(afd1.getFileDescriptor());
            player2.setDataSource(afd2.getFileDescriptor());
            player1.prepare();
            player2.prepare();
        } catch (IOException e) {
            e.printStackTrace();
        }

        Button button1 = (Button) findViewById(R.id.idButtonMusic1);
        Button button2 = (Button) findViewById(R.id.idButtonMusic2);
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                if (player1.isPlaying()) {
//                    return;
//                }
//                if (player2.isPlaying()) {
//                    player2.stop();
//                }
                player1.start();
            }
        });

        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                if (player2.isPlaying()) {
//                    return;
//                }
//                if (player1.isPlaying()) {
//                    player1.stop();
//                }
                player2.start();
            }
        });
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    public Action getIndexApiAction() {
        Thing object = new Thing.Builder()
                .setName("MainActivityResourceRawAssetTest Page") // TODO: Define a title for the content shown.
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
