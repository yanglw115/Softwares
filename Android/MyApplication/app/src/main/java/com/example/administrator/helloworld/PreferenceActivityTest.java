package com.example.administrator.helloworld;

import android.os.Bundle;
import android.preference.PreferenceActivity;
import android.preference.PreferenceFragment;
import android.widget.Button;
import android.widget.Toast;

import java.util.List;

/**
 * Created by Administrator on 2017/4/25.
 */

public class PreferenceActivityTest extends PreferenceActivity {
    @Override
    public void onBuildHeaders(List<Header> target) {
        /* 加载设置选项列表 */
        loadHeadersFromResource(R.xml.preference_headers, target);
    }

    @Override
    protected boolean isValidFragment(String fragmentName) {
        return true;
    }

    /* 定义PreferenceFragment，用来加载设置列表进行设置 */
    public static class Prefs1Fragment extends PreferenceFragment
    {
        @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            addPreferencesFromResource(R.xml.preferences);
        }
    }

    public static class Prefs2Fragment extends PreferenceFragment
    {
        @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            addPreferencesFromResource(R.xml.display_prefs);
            String website = getArguments().getString("website");
            Toast.makeText(getActivity(), "网站域名是: " + website, Toast.LENGTH_LONG).show();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        /* 为该界面设置一个标题按钮 */
        if (hasHeaders()) {
            Button button = new Button(this);
            button.setText("设置操作");
            setListFooter(button);
        }

    }
}
