package com.example.administrator.helloworld;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

/**
 * Created by Administrator on 2017/4/20.
 */

public class MainActivityExpandableListView extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        super.setContentView(R.layout.layout_expandable_list_view);
        ExpandableListAdapter listAdapter = new BaseExpandableListAdapter() {
            int[] logos = new int[] {
                    R.drawable.car,
                    R.drawable.image1,
                    R.drawable.image2
            };
            private String[] armTypes = new String[] {
                    "A", "B", "C"
            };
            private String[][] arms = new String[][] {
                    {"A1", "A2", "A3", "A4"},
                    {"B1", "B2", "B3", "B4"},
                    {"C1", "C2", "C3", "C4"}
            };
            @Override
            public int getGroupCount() {
                return arms.length;
            }

            @Override
            public int getChildrenCount(int groupPosition) {
                return arms[groupPosition].length;
            }

            @Override
            public Object getGroup(int groupPosition) {
                return arms[groupPosition];
            }

            @Override
            public Object getChild(int groupPosition, int childPosition) {
                return arms[groupPosition][childPosition];
            }

            @Override
            public long getGroupId(int groupPosition) {
                return groupPosition;
            }

            @Override
            public long getChildId(int groupPosition, int childPosition) {
                return childPosition;
            }

            @Override
            public boolean hasStableIds() {
                return true;
            }

            @Override
            public View getGroupView(int groupPosition, boolean isExpanded, View convertView, ViewGroup parent) {
                LinearLayout ll = new LinearLayout(MainActivityExpandableListView.this);
                ll.setOrientation(LinearLayout.VERTICAL);
                ImageView imageView = new ImageView(MainActivityExpandableListView.this);
                imageView.setImageResource(logos[groupPosition]);
                ll.addView(imageView);
                TextView textView = new TextView(MainActivityExpandableListView.this);
                textView.setText(armTypes[groupPosition]);
                ll.addView(textView);
                return ll;
            }

            @Override
            public View getChildView(int groupPosition, int childPosition, boolean isLastChild, View convertView, ViewGroup parent) {
                TextView textView = new TextView(MainActivityExpandableListView.this);
                textView.setText(arms[groupPosition][childPosition]);
                return textView;
            }

            @Override
            public boolean isChildSelectable(int groupPosition, int childPosition) {
                return true;
            }
        };
        ExpandableListView listView = (ExpandableListView) findViewById(R.id.idExpandableListView);
        listView.setAdapter(listAdapter);
    }
}
