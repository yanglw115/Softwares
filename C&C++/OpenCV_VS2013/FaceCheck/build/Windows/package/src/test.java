
package com.youle.cosmetology.jni;
import java.util.*;

public class test implements Runnable {
	String strImage;
	public void run() {
		CureJniFaceRecognition parser = new CureJniFaceRecognition();
		String strResult = parser.recogni(strImage);
		System.out.println("Get result: " + strResult);
	}
	public test(String strInput)
	{
		strImage = strInput;
	}
	
	public static void main(String argv[])
	{
		{
			String[] split = System.getProperty("java.library.path").split(";");  
			for (String string : split) {  
				System.out.println(string);  
			}  
			try {
				System.loadLibrary("CureFaceParserWin32");
			} catch (UnsatisfiedLinkError e) {
				System.err.println( "Cannot load CureFaceParserWin32 library:\n " + e.toString() ); 
			}
		}
		for (int i = 0; i < 1; ++i) {
			String strImage = "src/5.jpg";
			test t = new test(strImage);
			//Thread t1 = new Thread(t);
			//t1.start();
			t.run();
		}
		while (true) {
			try {
				Thread.sleep(500);
			} catch (Exception  e) {

			}
		}
	}
	
}
