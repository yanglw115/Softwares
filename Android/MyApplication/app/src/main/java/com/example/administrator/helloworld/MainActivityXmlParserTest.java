package com.example.administrator.helloworld;

import android.app.Activity;
import android.content.res.XmlResourceParser;
import android.os.Bundle;
import android.widget.EditText;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import java.io.IOException;

/**
 * Created by Administrator on 2017/5/2.
 */

public class MainActivityXmlParserTest extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_xml_parser_test);

        /* XmlResourceParse是XmlPullParser的子类 */
        XmlResourceParser parser = getResources().getXml(R.xml.xml_parser_test);
        try {
            StringBuilder sb = new StringBuilder("");
            while (parser.getEventType() != XmlResourceParser.END_DOCUMENT) {
                if (parser.getEventType() == XmlPullParser.START_TAG) {
                    String strTagName = parser.getName();
                    /* 这里字符串的比较不能使用"=="，而是需要使用equals */
                    if (strTagName.equals("book")) {
                        String strPrice = parser.getAttributeValue(null, "price");
                        sb.append("价格: ");
                        sb.append(strPrice);

                        String strDate = parser.getAttributeName(1);
                        sb.append(", 出版日期: ");
                        sb.append(strDate);

                        sb.append(", 书名: ");
                        sb.append(parser.nextText());

                        sb.append("\n");
                    }
                }
                /* 这里一定要使用next，以对下一个标签进行解析 */
                parser.next();
            }
            EditText text = (EditText) findViewById(R.id.idXmlShow);
            text.setText(sb.toString());
        } catch (XmlPullParserException e) {
            e.printStackTrace();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
