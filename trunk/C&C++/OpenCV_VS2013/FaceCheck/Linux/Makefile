
# 最终生成的动态名称
GENERATE_LIBRARY = libCureFaceRecognition.so

# jpeg和png图片的支持，由dlib依赖
DEFINES = -DDLIB_JPEG_SUPPORT -DDLIB_PNG_SUPPORT
INCLUDES = -I. -I./build/include -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el6_9.x86_64/include  -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el6_9.x86_64/include/linux
CFLAGS = -Wall $(INCLUDES) $(DEFINES)
DirCompile=compile

# 在编译test.java时，test.java中import后面必须是目录？不然不成功
DirJin = Jni
DirJniClasses = $(DirJin)/classes
DirJniSrc = $(DirJin)/src
NameJniEntry = com.youle.cosmetology.jni.test

# 打包文件夹
DirPackage = build/package
NamePackageShell = package.sh
NameInstaller = FaceParserInstall

# 为了测试opencv库和dlib库的链接而对写的测试.c文件进行编译链接
TEST_MAIN_EXE = test.exe

# 主要是opencv和dlib相关的库，可以去掉一些不必要的（这里的libjpeg.so和libpng.so使用的是系统的）
LIBS = -L./build/libs -lglog -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -lopenblas -ldlib -ljpeg -lpng16 -lpthread
#LIBS = -L./build/libsD -lopencv_calib3d -lopencv_core -lopencv_dnn -lopencv_features2d -lopencv_flann 
#LIBS += -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo
#LIBS += -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videoio -lopencv_video
#LIBS += -lopencv_videostab -ldlib -ljpeg -lpng16 -lpthread -lglog

OBJS = $(patsubst %.cpp, $(DirCompile)/%.o, $(wildcard *.cpp))

CC = /usr/local/gcc-7.2.0/bin/g++
# CC = g++

$(GENERATE_LIBRARY): $(OBJS) 
	$(CC) -shared $(CFLAGS) $(OBJS) $(LIBS) -o $(DirCompile)/$(GENERATE_LIBRARY)

 $(DirCompile)/%.o: %.cpp
	$(CC) -fPIC $(CFLAGS) -c $*.cpp -o $@

clean:
	rm -rf $(DirCompile)/*
	rm -rf $(DirJniClasses)/*

java: $(GENERATE_LIBRARY)
	javac -d $(DirJniClasses) $(DirJniSrc)/*.java
	java -classpath $(DirJniClasses) $(NameJniEntry)

test: $(GENERATE_LIBRARY)
	$(CC) -o $(DirCompile)/$(TEST_MAIN_EXE) test.c $(CFLAGS) $(LIBS) -L. -lCureFaceRecognition

pack:
	cd $(DirPackage) && bash $(NamePackageShell)

# 安装到本机
install:
	cd $(DirPackage) && find ./ -name "*.bin" -exec sudo {} \;


