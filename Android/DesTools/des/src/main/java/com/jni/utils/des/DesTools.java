package com.jni.utils.des;

/**
 * Created by Administrator on 2018/4/4.
 */

public class DesTools {
    static {
        System.loadLibrary("JNIDes");
    }
    public native String encrypt(String strInput);
    public native String decrypt(String strInput);
}
