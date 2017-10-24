

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>

#include "IR_utils.h"


void print_strerror(const char *pPrefix)
{
    if (pPrefix)
	    printf("%s: %s\n", pPrefix? pPrefix: "", strerror(errno));
    else
        printf("Input parameter is NULL!\n");
}

bool is_file_exist(const char *pFile)
{
    if (pFile)
	    return (0 == access(pFile? pFile: "", F_OK));

    printf("Input parameter is NULL!\n");
    return false;
}


bool remove_file(const char *pFile)
{
    if (pFile) {
    	if (0 == remove(pFile? pFile: "")) {
    		return true;
    	}
    	print_strerror("Remove file failed");
    } else {
        printf("Input parameter is NULL!\n");
    }

	return false;
}

bool remove_dir(const char *path)
{
    if (path) {
    	if (0 == rmdir(path? path: "")) {
    		return true;
    	}
    	print_strerror("Remove dir");
    } else {
        printf("Input parameter is NULL!\n");
    }
	return false;
}


bool rename_file(const char *pOldPath, const char *pNewPath)
{
    if (pOldPath && pNewPath) {
    	if (0 == rename(pOldPath, pNewPath)) {
    		return true;
    	}
    	print_strerror("Rename file failed");
    } else {
        printf("Input parameter is NULL!\n");
    }
	return false;
}

bool register_exit_function(func_exit fun)
{
    if (fun)
	    return (0 == atexit(fun));
    return false;
}

long string_to_integer(const char *pStr, const int nDefault)
{
	char *endptr = NULL;
	long val = nDefault;
	if (pStr) {
		val = strtol(pStr, &endptr, 10);
		if ((ERANGE == errno && (LONG_MAX == val || LONG_MIN == val))
			|| (0 != errno && 0 == val)) {
			perror("strtol");
		}
		if (endptr == pStr) {
			printf("No digits was found!\n");
		}
	} else {
        printf("Input parameter is NULL!\n");
    }
	return val;
}

char *string_duplicate(const char *s)
{
    if (s) {
        return strdup(s);
    } else {
       printf("Input parameter is NULL!\n"); 
    }
    return strdup("");
}

void string_duplicate_ex(char **dest, const char *s)
{
    free_safe(*dest);

    if (s) {
        *dest = string_duplicate(s);
    } else {
        printf("Input parameter is NULL!\n"); 
        *dest = string_duplicate("");
    }
}

bool strcat_ex(char **dest, const char *s)
{
    int nSize = 0;
    int nInit = 0;
    char *ptr = NULL;
    if (s && strlen(s) > 0) {
        nSize = strlen(s);
        if (*dest) {
            nInit = strlen(*dest);
            nSize += nInit;
        }
        ptr = (char *)realloc(*dest, nSize + 1);
        if (ptr) {
            memcpy(ptr + nInit, s, strlen(s));
            ptr[nSize] = '\0';
            *dest = ptr;
            return true;
        }
    }
    return false;
}

void free_safe(void *var)
{
    if (var) {
        free(var);
        var = NULL;
    } else {
        printf("Input parameter is NULL!\n"); 
    }
}

char* strstr_safe(const char *pData, const char *pFind)
{
    if (pData && pFind) {
        return strstr(pData, pFind);
    } else {
        printf("Input parameter is NULL!\n"); 
    }
    return NULL;
}

/* UTF-8 */
static bool head_str(const char *pData, const char *str)
{
    if (pData && str) {
        if (strstr(pData, str) == pData)
            return true;
    } else {
        printf("Input parameter is NULL!\n"); 
    }
    return false;
}
static symbolType get_symbol_type(const char *pData)
{
    if (pData) {
        if (head_str(pData, ".") || head_str(pData, " ") || head_str(pData, "?") || head_str(pData, "!") || head_str(pData, ",")
            || head_str(pData, "-") || head_str(pData, "~") || head_str(pData, "]") || head_str(pData, "[")) {
            return Symbol_En;
        }
        if (head_str(pData, "。") || head_str(pData, "？") || head_str(pData, "！") || head_str(pData, "，")
            || head_str(pData, "】") || head_str(pData, "【")) {
            return Symbol_Cn;
        }
    }
    return Symbol_None;
}

char * get_trimmed_string(const char *pData)
{
    const char *ptrHead = NULL;
    const char *ptrTail = NULL;
    int i = 0;
    if (pData) {
        ptrHead = pData;
        ptrTail = pData + strlen(pData) - 1;
        for (i = 0; i < strlen(pData); ++i) {
            symbolType type = get_symbol_type(pData + i);
            if (Symbol_En == type) {
                ++ptrHead;
            } else if (Symbol_Cn == type) {
                i += 2;
                ptrHead += 3;
            } else {
                break;
            }
        }
        for (i = 0; i < strlen(pData); ++i) {
            symbolType type = get_symbol_type(pData + strlen(pData) - i - 1);
            if (Symbol_En == type) {
                --ptrTail;
            } else if (Symbol_Cn == type) {
                i += 2;
                ptrTail -= 3;
            } else {
                break;
            }
        }
        i = ptrTail - ptrHead;
        if (i > 0) {
            char *pRet = (char *)malloc(i + 2);
            memcpy(pRet, ptrHead, i + 1);
            pRet[i + 1] = '\0';
            return pRet;
        }
    } else {
        printf("Input parameter is NULL!\n"); 
    }
    return NULL;
}


