

#ifndef __IR_UTILS_H__
#define __IR_UTILS_H__

typedef int bool;
#define true 1
#define false 0

typedef enum {
    Symbol_None = 0,
    Symbol_Cn,
    Symbol_En
} symbolType;

/* Linux下面main函数退出时的回调 */
typedef void (*func_exit)(void);
/* 仅仅是应用中的一个回调函数类型申明          */
typedef void (*callback_listen_result)(const char *);

/* Linux下strerror的输出封装，传入参数只是提示哪个函数调用即可，比如malloc */
void print_strerror(const char *pPrefix);

/* 文件是否存在的判断 */
bool is_file_exist(const char *pFile);

/* 删除文件 */
bool remove_file(const char *pFile);

/* 删除空目录，目录必须为空才可 */
bool remove_dir(const char *path);

/* 重命名文件 */
bool rename_file(const char *pOldPath, const char *pNewPath);

/* 注册main函数退出时的回调函数  */
bool register_exit_function(func_exit fun);

/* string转整型，需要传入失败后的默认值 */
long string_to_integer(const char *pStr, const int nDefault);

/* 拷贝字符串到新的内存地址并返回新malloc的内存地址 */
char *string_duplicate(const char *s);

/* 拷贝字符串到新的内存地址，如果原dest地址不为空，则先free掉 */
void string_duplicate_ex(char **dest, const char *s);

/* 在字符串后面追加字符串，并增加新的内存 */
bool strcat_ex(char **dest, const char *s);

/* 安全释放内存，不需要判断传入的参数是否合法 */
void free_safe(void *);

/* 安全进行strstr查找，不需要判断传入的参数是否合法 */
char * strstr_safe(const char *pData, const char *pFind);

/* 去除字符串两端的符号内容，并返回其余内容拷贝到的新地址。这里处理的符号有限，后续可以添加 */
char * get_trimmed_string(const char *pData);

//settime

#endif // __IR_UTILS_H__

