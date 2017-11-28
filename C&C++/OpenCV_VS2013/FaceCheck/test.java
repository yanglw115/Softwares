
import java.util.*;
import subdir.*;

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
			try {
				System.loadLibrary("CureFaceRecognition");
			} catch (UnsatisfiedLinkError e) {
				System.err.println( "Cannot load CureFaceParser library:\n " + e.toString() ); 
			}
		}
		for (int i = 0; i < 10; ++i) {
			String strImage = "/home/yangliwei/OpenCV_Compile/FaceCheck/images/" + i + ".jpg";
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
