package com.example.administrator.helloworld;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

/**
 * Created by Administrator on 2017/4/27.
 */

public class MainActivityIntent_Action_Data extends MainActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_intent_action_data);

//        getWindow().setBackgroundDrawableResource(R.drawable.car);
//        getWindow().setTitle(getResources().getText(R.string.ok));

        Button buttonOpen = (Button) findViewById(R.id.idButtonOpen);
        Button buttonEdit = (Button) findViewById(R.id.idButtonEdit);
        Button buttonDial = (Button) findViewById(R.id.idButtonDial);

        buttonOpen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setAction(Intent.ACTION_VIEW);
                intent.setData(Uri.parse("http://www.baidu.com"));
                startActivity(intent);
            }
        });

        buttonEdit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setAction(Intent.ACTION_EDIT);
                intent.setData(Uri.parse("content://com.android.contacts/contacts/1"));
                startActivity(intent);
            }
        });

        buttonDial.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setAction(Intent.ACTION_DIAL);
                intent.setData(Uri.parse("tel:12345678911"));
                startActivity(intent);
            }
        });
    }
}
