package com.example.administrator.helloworld;

import android.content.CursorLoader;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

/**
 * Created by Administrator on 2017/4/26.
 */

public class MainActivityIntentGetContact extends MainActivity {
    final int PICK_CONTACT = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_intent_get_contact);

        Button button = (Button) findViewById(R.id.idButtonGet);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                /***
                 * 获取手机联系人信息，需要在AndroidManifest.xml中增加相应权限
                intent.setAction(Intent.ACTION_GET_CONTENT);
                intent.setType("vnd.android.cursor.item/phone");
                */

                /* 回到桌面 */
                intent.setAction(Intent.ACTION_MAIN);
                intent.addCategory(Intent.CATEGORY_HOME);

                startActivityForResult(intent, PICK_CONTACT);
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_CONTACT) {
            if (resultCode == RESULT_OK) {
                /* 详细可以参考Android ContentProvider的内容 */
                Uri contactData = data.getData();
                CursorLoader cursorLoader = new CursorLoader(this, contactData, null, null, null, null);

                Cursor cursor = cursorLoader.loadInBackground();
                if (cursor.moveToFirst()) {
                    String contactID = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID));
                    String name = cursor.getString(cursor.getColumnIndexOrThrow(ContactsContract.Contacts.DISPLAY_NAME));
                    String phoneNumer = "此联系人暂未输入电话号码";

                    Cursor phones = getContentResolver().query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI, null,
                            ContactsContract.CommonDataKinds.Phone.CONTACT_ID + " = " + contactID, null, null);
                    if (phones.moveToFirst()) {
                        phoneNumer = phones.getString(phones.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
                    }

                    phones.close();
                    EditText showName = (EditText) findViewById(R.id.idTextName);
                    showName.setText(name);
                    EditText showPhone = (EditText) findViewById(R.id.idTextPhone);
                    showPhone.setText(phoneNumer);
                }
                cursor.close();
            }
        }
    }
}
