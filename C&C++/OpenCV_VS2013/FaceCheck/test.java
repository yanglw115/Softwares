
import java.util.*;
import pa.*;

public class test {
	public test()
	{
		CureFaceParser parser = new CureFaceParser();
		parser.getFaceParseResult("/home/yangliwei/OpenCV/program/FaceCheck/liudehua.jpg");
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