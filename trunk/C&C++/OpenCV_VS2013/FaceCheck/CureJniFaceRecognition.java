
/**
 * 面部识别
 * 调用c++面部识别接口
 * Created by kedong on 2017/11/27.
 */
public class CureJniFaceRecognition {

    // 给动态库提动接口函数
    public native String recogni(String imagePath);
}
