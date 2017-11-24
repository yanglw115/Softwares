
import java.util.*;
import subdir.*;

public class test {
	public test()
	{
		CureFaceParser parser = new CureFaceParser();
		String strResult = parser.getFaceParseResult("/home/yangliwei/OpenCV_Compile/FaceCheck/images/baby.jpg");
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
