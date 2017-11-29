@echo "Start to make java test ......"

@set ClassPath=.\classes
@set SrcPath=.\src
@set EntryName=com.youle.cosmetology.jni.test

@rem del 只能用来删除文件，无法删除文件夹；需要使用rd或rmdir
rmdir /s /q %ClassPath%
mkdir %ClassPath%

javac -d %ClassPath% %SrcPath%\*.java

java -classpath %ClassPath% %EntryName%

cmd