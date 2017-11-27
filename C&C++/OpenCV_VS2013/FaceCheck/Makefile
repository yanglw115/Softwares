
# 最终生成的动态名称
GENERATE_LIBRARY = libCureFaceRecognition.so

# jpeg和png图片的支持，由dlib依赖
DEFINES = -DDLIB_JPEG_SUPPORT -DDLIB_PNG_SUPPORT
INCLUDES = -I. -I./build/include -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el6_9.x86_64/include  -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el6_9.x86_64/include/linux
CFLAGS = -Wall $(INCLUDES) $(DEFINES)

# 在编译test.java时，test.java中import后面必须是目录？不然不成功
JAVA_SUBDIR = subdir

# 为了测试opencv库和dlib库的链接而对写的测试.c文件进行编译链接
TEST_MAIN_EXE = test.exe

# 主要是opencv和dlib相关的库，可以去掉一些不必要的（这里的libjpeg.so和libpng.so使用的是系统的）
LIBS = -L./build/libs -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopenblas -ldlib -ljpeg -lpng16
#LIBS = -L./build/libs -lopencv_calib3d -lopencv_core -lopencv_dnn -lopencv_features2d -lopencv_flann 
#LIBS += -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo
#LIBS += -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videoio -lopencv_video
#LIBS += -lopencv_videostab -ldlib -ljpeg -lpng

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

CC = /usr/local/gcc-7.2.0/bin/g++
# CC = g++

$(GENERATE_LIBRARY): $(OBJS) 
	$(CC) -shared $(CFLAGS) $(OBJS) $(LIBS) -o $(GENERATE_LIBRARY)

%.o: %.cpp
	$(CC) -fPIC $(CFLAGS) -c $*.cpp

clean:
	rm -rf *.o
	rm -rf *.so*
	rm -rf *.class
	rm -rf ./$(JAVA_SUBDIR)/*.class
	rm -rf $(TEST_MAIN_EXE)

java: $(GENERATE_LIBRARY)
	javac Cure_JNI_FaceRecognition.java -d $(JAVA_SUBDIR)
	javac test.java

test: $(GENERATE_LIBRARY)
#test: $(GENERATE_LIBRARY)
	$(CC) -o $(TEST_MAIN_EXE) test.c $(CFLAGS) $(LIBS) -L. -lCureFaceRecognition


