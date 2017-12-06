
# 最终生成的动态名称
GENERATE_LIBRARY = libCureFaceRecognition.so
VERSION_FILE = version.h

# jpeg和png图片的支持，由dlib依赖
DEFINES = -DDLIB_JPEG_SUPPORT -DDLIB_PNG_SUPPORT
INCLUDES = -I. -I./build/Linux/include -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el6_9.x86_64/include  -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el6_9.x86_64/include/linux
CFLAGS = -Wall $(INCLUDES) $(DEFINES)
DirProject=build/Linux/project

# 在编译test.java时，test.java中import后面必须是目录？不然不成功
DirJin = build/Linux/Jni
DirJniClasses = $(DirJin)/classes
DirJniSrc = $(DirJin)/src
NameJniEntry = com.youle.cosmetology.jni.test

# 打包文件夹
DirPackage = build/Linux/package
NamePackageShell = package.sh
NameInstaller = FaceParserInstall

NamePackageUpdateShell = package_update.sh
NameUpdateBin = FaceParserUpdate

# 为了测试opencv库和dlib库的链接而对写的测试.c文件进行编译链接
TEST_MAIN_EXE = test.exe

# 主要是opencv和dlib相关的库，可以去掉一些不必要的（这里的libjpeg.so和libpng.so使用的是系统的）
LIBS = -L./build/Linux/libs -lglog -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -lopenblas -ldlib -ljpeg -lpng16 -lpthread
#LIBS = -L./build/libsD -lopencv_calib3d -lopencv_core -lopencv_dnn -lopencv_features2d -lopencv_flann 
#LIBS += -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo
#LIBS += -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videoio -lopencv_video
#LIBS += -lopencv_videostab -ldlib -ljpeg -lpng16 -lpthread -lglog

# 这里指定了临时库存在目录，非当前目录
OBJS = $(patsubst %.cpp, $(DirProject)/%.o, $(wildcard *.cpp))

CC = /usr/local/gcc-7.2.0/bin/g++
# CC = g++
VERSION=$(shell svn info | grep "Last Changed Rev" | cut -f2 -d ":")

# 生成动态库，并且生成到compile目录当中
$(GENERATE_LIBRARY): $(VERSION_FILE) $(OBJS) 
	$(CC) -shared $(CFLAGS) $(OBJS) $(LIBS) -o $(DirProject)/$(GENERATE_LIBRARY)

$(VERSION_FILE):
	echo "static const int nVersion = $(VERSION);" > $(VERSION_FILE)

$(DirProject)/%.o: %.cpp
	$(CC) -fPIC $(CFLAGS) -c $*.cpp -o $@

clean:
	rm -rf $(DirProject)/*
	rm -rf $(DirJniClasses)/*
	rm -rf $(VERSION_FILE)
	rm -rf $(TEST_MAIN_EXE)

# java在编译时需要同时编译入口java与依赖java文件，即下面的*.java
# java运行的时候需要指定-classpath，到哪个路径下去找依赖的class
java: $(GENERATE_LIBRARY)
	javac -d $(DirJniClasses) $(DirJniSrc)/*.java
	java -classpath $(DirJniClasses) $(NameJniEntry)

test: $(GENERATE_LIBRARY)
	$(CC) -o $(DirProject)/$(TEST_MAIN_EXE) test.cxx $(CFLAGS) $(LIBS) -L./$(DirProject) -lCureFaceRecognition
	rm -rf $(TEST_MAIN_EXE)
	ln -sv $(DirProject)/$(TEST_MAIN_EXE) $(TEST_MAIN_EXE)

# 打包文件(调用相关脚本)
pack: $(GENERATE_LIBRARY)
	cd $(DirPackage) && bash $(NamePackageShell)

# 安装到本机(调用相关脚本)
install:
	cd $(DirPackage) && find ./ -name $(NameInstaller)"*.bin" -exec sudo {} \;

pack_update:
	cd $(DirPackage) && bash $(NamePackageUpdateShell)
	
update:
	cd $(DirPackage) && find ./ -name $(NameUpdateBin)"*.bin" -exec sudo {} \;


