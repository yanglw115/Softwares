
import java.util.*;
import subdir.*;

public class test {
	public test()
	{
		CureFaceParser parser = new CureFaceParser();
		String strResult = parser.getFaceParseResult("/home/yangliwei/caffe/OpenCV/FaceCheck/liudehua.jpg");
		System.out.println("Get result: " + strResult);
	}
	static {
		try {
			System.loadLibrary("CureFaceParser");
		} catch (UnsatisfiedLinkError e) {
			System.err.println( "Cannot load CureFaceParser library:\n " + e.toString() ); 
		}
	}
	
	public static void main(String argv[])
	{
		test t = new test();
	}
	
}