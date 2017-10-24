

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <time.h>

#include "IR_msp.h"
#include "IR_utils.h"
#include "IR_audio_to_text_realtime.h"
#include "IR_text_to_audio.h"
#include "IR_speak_reply_object.h"
#include "IR_audio_player.h"
#include "IR_speak_use_text.h"

//const char* login_params         = "appid = 59e55824, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动
static const char* login_params         = "appid = 59e55824";//";//登录参数,appid与msc库绑定,请勿随意改动
#define g_nSizeReplys  5
static const char* g_strWakeUpReplys[g_nSizeReplys] = {
    "./resources/records/dao.wav",
    "./resources/records/gansha.wav",
    "./resources/records/shenma.wav",
    "./resources/records/wozaide.wav",
    "./resources/records/wozaine.wav"
};

static const char *g_strWakeUp = "叮咚叮咚";
static const char *g_strConvertWav = "convert.wav";
static const char *g_strConvertWav1 = "convert1.wav";
static const char *g_ptr = NULL;


#define MAX_EVENTS 10
int g_fd2[2] = {-1};

typedef struct {
    int nLen;
    const char *pStrData;
} struData;

static bool login_to_cloud()
{
    int nRet = MSPLogin(NULL, NULL, login_params);
    if (MSP_SUCCESS != nRet) {
        printf("MSP login failed: %d\n", nRet);
		return false;
    } else {
		printf("MSP login success!\n");
		return true;
    }

}

static void logout_from_cloud()
{
    int nRet = MSPLogout();
    if (MSP_SUCCESS == nRet) {
       	printf("MSP logout failed: %d\n", nRet);
    } else {
		printf("MSP logout success!\n");
	}

}

static void *thread_listen(void *arg)
{
    const char * pUserWordsFile = "./resources/userWords.txt";
    
    start_convert_from_mic(pUserWordsFile, (callback_listen_result)arg);
    return NULL;
}

static void handle_listen_result(const char *pStr)
{
    /* 这里可以对参数再作进一步优化处理，使listening线程可以多处进行回调，告知各种事件或状态 */
    struData data;
    data.nLen = strlen(pStr) + 1;
    data.pStrData = pStr;
    if (-1 == write(g_fd2[1], &data, sizeof(data))) {
        print_strerror("Wrtie");
        free_safe((void *)pStr);
    }
}

static void exit_function(void)
{
	logout_from_cloud();	
}

static void parseDataFromeListen(bool *pbWakeUp, char **ppStrCmd, const char *pData)
{
    char *ptr = NULL;
    if ((ptr = strstr_safe(pData, g_strWakeUp))) {
        *pbWakeUp = true;
        *ppStrCmd = get_trimmed_string(ptr + strlen(g_strWakeUp));
    } else {
        *pbWakeUp = false;
        *ppStrCmd = get_trimmed_string(pData);
    } 
}

static void handle_readed_data(struData *pData)
{
    bool bWakeUp = false;
    char *pStrCmd = NULL;

    printf("Readed data from pipe: %s\n", pData->pStrData);
    parseDataFromeListen(&bWakeUp, &pStrCmd, pData->pStrData);
    if (bWakeUp) {
        srandom(time(NULL));
        printf("播放唤醒回复...\n");
        play_audio(g_strWakeUpReplys[random() % sizeof(g_nSizeReplys)]);
        sleep(1);
    }

    printf("解析到的指令为: %s\n", pStrCmd);
    if (pStrCmd) {
        char *pStrOutput = NULL;
        if (0 < start_speak(pStrCmd, strlen(pStrCmd), &pStrOutput)) {
            struReplyObject *pReplyObject = (struReplyObject *)malloc(sizeof(*pReplyObject));
            parseReplyJsonObject(pStrOutput, pReplyObject);
            if (pReplyObject->pAnswerText) {
                /* 播放器占用文件问题 */
                if (!g_ptr) {
                    g_ptr = g_strConvertWav;
                } else if (g_ptr == g_strConvertWav) {
                    g_ptr = g_strConvertWav1;
                } else {
                    g_ptr = g_strConvertWav;
                }
                if (0 == text_to_audio(pReplyObject->pAnswerText, g_ptr)) {
                    play_audio(g_ptr);
                } else {
                    printf("语音合成失败!\n");
                }
                if (pReplyObject->pAnswerUrl) {
                    while (is_playing_audio()) {
                        sleep(1);
                    }
                    play_audio(pReplyObject->pAnswerUrl);
                }
            } else {
                printf("Answer内容为空!\n");
            }

            free_safe(pReplyObject->pAnswerText);
            free_safe(pReplyObject->pAnswerType);
            free_safe(pReplyObject->pAnswerUrl);
        } else {
            printf("语义解析请求失败!\n");
        }
        free_safe(pStrOutput);
    }
    
    free_safe((void *)pData->pStrData);
    free_safe((void *)pData);
    free_safe(pStrCmd);
}

int main(int argc, char **argvs)
{
    int fdPoll = -1;
    int fds = -1;
    struData *pDataRead = NULL;
    struct epoll_event event;  
    struct epoll_event events[MAX_EVENTS]; 

	register_exit_function(exit_function);
	login_to_cloud();

    pthread_t threadListen;
    if (!pthread_create(&threadListen, NULL, thread_listen, (void *)handle_listen_result)) {
        if (pthread_detach(threadListen)) {
            print_strerror("Thread listen detach");
            return -1;
        } 
    } else {
        print_strerror("Thread listen create");
        return -1;
    }

    if (-1 == (fdPoll = epoll_create1(0))) {
        print_strerror("Epoll create");
        return -1;
    }

    if (-1 == pipe(g_fd2)) {
        print_strerror("Create pipe");
        return -1;
    }
    event.data.fd = g_fd2[0];
    event.events = EPOLLIN;
    if (-1 == epoll_ctl(fdPoll, EPOLL_CTL_ADD, g_fd2[0], &event)) {
        print_strerror("Epoll ctrol");
        return -1;
    }

	while (1) {
        fds = epoll_wait(fdPoll, events, 1, -1);
        if (-1 == fds) {
            print_strerror("Epoll wait");
            break;
        }
        pDataRead = (struData *)malloc(sizeof(struData));
        if (-1 == read(g_fd2[0], pDataRead, sizeof(struData))) {
            print_strerror("Read");
            free_safe((void *)pDataRead);
        } else {
            handle_readed_data(pDataRead);
        }
	}

	return 0;
}

