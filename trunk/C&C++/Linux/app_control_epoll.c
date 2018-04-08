
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <time.h>
#include <stdio.h>
#include <sys/syslog.h>
#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "debug.h"
#include <json.h>
#include <nvram_linux.h>
#include "jw_des.h"
#include "trust.h"
#include "conf.h"
#include "notify_rc.h"
#include "firewall.h"
#include "util.h"
#include "app_control.h"

#define MAX_EVENTS 10
#define TIMEOUT_EPOLL -1

#define KEY_DES "~^3*H(@K"
#define True 1
#define False 0
#define GATEWAY_INTERFACE "br0"
#define WIFI_INTERFACE "ra0"

int g_nMaxCloseTime = 0; /* guest wifi最长关闭时间，从flash中获取 */
static int g_nExitEpoll = False;
static int g_fdUDPServer = -1;
static int g_fdTCPServer = -1;
static int g_fdEpoll = -1;
static time_t g_timeStampKey = 0;
int g_nGuestSwitchState = -1;
int g_nCloseTimeUsed = 0; // unit: minute
time_t g_nTimeStampToday = 0;

static struct sockaddr_in g_addrUdpClient;
static socklen_t g_lenUdpClient;

static char g_ipRouter[17] = {0};
static char g_macRouter[18] = {0};


typedef enum {
	TYPE_NET_AUTO = 1,
	TYPE_NET_STATIC = 2,
	TYPE_NET_PPPOE = 3
} NetType_t;

typedef enum {
	TYPE_ROUTER_IP = 100,
	TYPE_SHARE_INFO = 200,
	TYPE_SHARE_SET = 210,
	TYPE_CONNECT_INFO = 300,
	TYPE_CONNECT_SET = 310,
	TYPE_USER_WIFI_INFO = 400,
	TYPE_USER_WIFI_SET = 410,
	TYPE_GUEST_WIFI_INFO = 500,
	TYPE_GUEST_WIFI_SET = 510,
	TYPE_INVALID_KEY = 1111,
	TYPE_INVALID_PARAM = 1112,
} ResponseType_t;


typedef struct {
	char ip[16];
	char mask[16];
	char gateway[16];
	char dns1[16];
	char dns2[16];
} struConfigStatic_t;

typedef struct {
	char user[128];
	char passwd[256];
} struConfigPPPoE_t;

typedef union {
	struConfigStatic_t configStatic;
	struConfigPPPoE_t configPPPoE;
} unionConfigNetwork_t;

static void close_fd(int *fd)
{
	if (-1 != *fd) {
		close(*fd);
		*fd = -1;
	}
}

static char *strip_whitespace(char *ptr)
{
	char *p1 = NULL, *p2 = NULL;
	if (ptr) {
		p1 = ptr;
		while (*p1 && isspace(*p1)) {
			++p1;
		}
		p2 = ptr + strlen(ptr) - 1;
		while (*p2 && isspace(*p2)) {
			*p2 = '\0';
			--p2;
		}
	}
	return p1;
}


static unsigned char *strip_whitespace_uchar(unsigned char *ptr)
{
	unsigned char *p1 = NULL, *p2 = NULL;
	if (ptr) {
		p1 = ptr;
		while (*p1 && isspace(*p1)) {
			++p1;
		}
		p2 = ptr + strlen(ptr) - 1;
		while (*p2 && isspace(*p2)) {
			*p2 = '\0';
			--p2;
		}
	}
	return p1;
}

static int is_ip_object_valid(struct json_object *pObject, char *buffer, int lenBuffer)
{
	if (!is_error(pObject)) {
		const char *pIP = json_object_get_string(pObject);
		if (!is_error(pIP)) {
			char *ptr = NULL;
			char temp[32] = {0};
			strncpy(temp, pIP, sizeof(temp) - 1);
			ptr = strip_whitespace(temp);
			if (check_ip_format(ptr)) {
				strncpy(buffer, ptr, lenBuffer - 1);
				return True;
			}
		}
	}
	return False;
}

static int is_string_object_valid(struct json_object *pObject, char *buffer, int lenBuffer)
{
	if (!is_error(pObject)) {
		const char *pString = json_object_get_string(pObject);
		if (!is_error(pString)) {
			strncpy(buffer, pString, lenBuffer - 1);
			return True;
		}
	}
	return False;
}

static int get_key_encrypt(time_t timeStamp, unsigned char *out)
{
	unsigned char in[12] = {0};
	snprintf(in, sizeof(in), "%ld", timeStamp);
	if (0 == des_encryption(in, KEY_DES, out)) {
		return 0;
	}
	debug(LOG_ERR, "Des encrypt failed!");
	return -1;
}

static void free_json_object_safe(struct json_object* pObject)
{
	if (pObject) {
		json_object_put(pObject);
		pObject = NULL;
	}
}

static time_t get_key_real(unsigned char *pKey)
{
	if (pKey) {
		unsigned char out[17] = {0};
		unsigned char in[33] = {0};
		strncpy(in, pKey, strlen(pKey));
		if (0 == des_decryption(strip_whitespace_uchar(in), KEY_DES, out)) {
			return atol(out);
		}
		debug(LOG_ERR, "Des decryte failed!");
	}
	return 0;
}

static int update_network_share_state(SwitchState_t state)
{
	/* 关闭ra1、清除防火墙规则、清除client_list */
	if (g_nGuestSwitchState == state) {
		debug(LOG_INFO, "Guest share switch state is the same, no need to change.");
		return 0;
	}
	g_nGuestSwitchState = state;
	nvram_set_int("rt_guest_enable", state);
	nvram_commit();
	notify_rc("control_wifi_guest_rt");
	fw_set_guest_wifi_enable(state);
	return 0;
}

static int update_network_connect_type(NetType_t type, unionConfigNetwork_t *unionConfig)
{
	unsigned char buffer[256] = {0};
	switch (type) {
		case TYPE_NET_AUTO:
			nvram_set("wan_proto", "dhcp");
		    nvram_set("wan_dnsenable_x", "1");
		break;
		case TYPE_NET_STATIC:
			nvram_set("wan_ipaddr", "static");
	        nvram_set("wan_ipaddr", unionConfig->configStatic.ip);
	        nvram_set("wan_netmask", unionConfig->configStatic.mask);
	        nvram_set("wan_gateway", unionConfig->configStatic.gateway);

	        if(strlen(unionConfig->configStatic.dns1)) {   
	            nvram_set("wan_dns1_x", unionConfig->configStatic.dns1);                   
	        }
			if(strlen(unionConfig->configStatic.dns2)) {   
	            nvram_set("wan_dns2_x", unionConfig->configStatic.dns2);                   
	        }
		break;
		case TYPE_NET_PPPOE:
			if (0 != des_decryption(unionConfig->configPPPoE.passwd, KEY_DES, buffer)) {
				debug(LOG_ERR, "DES decrypt failed for pppoe passwd.");
				return -1;
			}
			nvram_set("wan_proto","pppoe");
		    nvram_set("wan_pppoe_passwd", buffer);
		    nvram_set("wan_pppoe_username", unionConfig->configPPPoE.user);
		break;
		default:
			return -1;
		break;
	}
	nvram_commit();
	notify_rc("restart_whole_wan");
	return 0;
}

static int update_user_wifi_config(char *pSSID, char *pPasswd)
{
	char *pSSIDCurrent = nvram_get("rt_ssid");
	char *pPasswdCurrent = nvram_get("rt_guest_wpa_psk");
	unsigned char buffer[256] = {0};
	if (0 != des_decryption(pPasswd, KEY_DES, buffer)) {
		debug(LOG_ERR, "DES decrypt for user wifi passwd failed!");
		return -1;
	}
	if (pSSIDCurrent && pPasswdCurrent &&
		(0 == strcmp(pSSIDCurrent, pSSID)) && (0 == strcmp(pPasswdCurrent, buffer))) {
		debug(LOG_INFO, "The current user wifi SSID and PASSWD is the same, no need to set.");
		return 0;
	}
	nvram_set("rt_ssid", pSSID);
	nvram_set("rt_wpa_psk", buffer);
	nvram_set("rt_auth_mode","psk");
	nvram_set("rt_wpa_mode","2");
	nvram_set("rt_crypto","aes");
	
	nvram_commit();
	notify_rc("restart_wifi_rt");
	return 0;
}

static int update_guest_wifi_suffix(const char *pSSIDSuffix)
{
	char *pSuffixCurrent = nvram_get("rt_guest_ssid_suffix");
	char *pPrefixGuest = nvram_get("rt_guest_ssid_prefix");
	if (0 != strcmp(pSuffixCurrent, pSSIDSuffix)) {
		char buffer[512] = {0};
		snprintf(buffer, sizeof(buffer), "%s%s", pPrefixGuest, pSSIDSuffix);
		nvram_set("rt_guest_ssid_suffix", pSSIDSuffix);
		nvram_set("rt_guest_ssid", buffer);
		nvram_commit();
		notify_rc("control_wifi_guest_rt");
	} else {
		debug(LOG_INFO, "Guest wifi ssid suffix is the same, no need to change.");
	}

	return 0;
}

static int set_network_share_state(struct json_object *pJsonObject)
{
	struct json_object *pObject = json_object_object_get(pJsonObject, "switches");
	if (!is_error(pObject)) {
		const char *pSwitch = json_object_get_string(pObject);
		if (!is_error(pSwitch)) {
			char buffer[20] = {0};
			debug(LOG_INFO, "Get switch object success!");
			strncpy(buffer, pSwitch, sizeof(buffer) - 1);
			if (0 == strcasecmp("close", strip_whitespace(buffer))) {
				update_network_share_state(CLOSE_SHARE);
				return 0;
			} else if (0 == strcasecmp("open", strip_whitespace(buffer))) {
				update_network_share_state(OPEN_SHARE);
				return 0;
			}
		}
	}
	debug(LOG_INFO, "Get switch object failed!");
	return -1;
}

static int set_network_connect_type(struct json_object *pJsonObject)
{
	int nRet = -1;
	int nConfigValid = 0;
	struct json_object *pObject = json_object_object_get(pJsonObject, "netType");
	if (!is_error(pObject)) {
		unionConfigNetwork_t unionConfig;
		char buffer[256] = {0};
		int nNetType = json_object_get_int(pObject);
		switch (nNetType) {
			case TYPE_NET_AUTO:
				debug(LOG_INFO, "Start to set wan connect with dhcp...");
				update_network_connect_type(nNetType, NULL);
				nRet = 0;
			break;
			case TYPE_NET_STATIC:
				memset(&unionConfig, 0, sizeof(unionConfig));
				pObject = json_object_object_get(pJsonObject, "ip");
				if (is_ip_object_valid(pObject, buffer, sizeof(buffer))) {
					debug(LOG_INFO, "Get ip object success!");
					strncpy(unionConfig.configStatic.ip, buffer, sizeof(unionConfig.configStatic.ip) - 1);
					nConfigValid |= 0x01;
				}
				pObject = json_object_object_get(pJsonObject, "mask");
				if (is_ip_object_valid(pObject, buffer, sizeof(buffer))) {
					debug(LOG_INFO, "Get mask object success!");
					strncpy(unionConfig.configStatic.mask, buffer, sizeof(unionConfig.configStatic.mask) - 1);
					nConfigValid |= 0x10;
				}
				pObject = json_object_object_get(pJsonObject, "gateway");
				if (is_ip_object_valid(pObject, buffer, sizeof(buffer))) {
					debug(LOG_INFO, "Get gateway object success!");
					strncpy(unionConfig.configStatic.gateway, buffer, sizeof(unionConfig.configStatic.gateway) - 1);
					nConfigValid |= 0x100;
				}
				pObject = json_object_object_get(pJsonObject, "dns1");
				if (is_ip_object_valid(pObject, buffer, sizeof(buffer))) {
					debug(LOG_INFO, "Get dns1 object success!");
					strncpy(unionConfig.configStatic.dns1, buffer, sizeof(unionConfig.configStatic.gateway) - 1);
				}
				pObject = json_object_object_get(pJsonObject, "dns2");
				if (is_ip_object_valid(pObject, buffer, sizeof(buffer))) {
					debug(LOG_INFO, "Get dns2 object success!");
					strncpy(unionConfig.configStatic.dns2, buffer, sizeof(unionConfig.configStatic.gateway) - 1);
				}
				if (0x111 == nConfigValid) {
					debug(LOG_INFO, "Start to set wan connect with static...");
					update_network_connect_type(nNetType, &unionConfig);
					nRet = 0;
				}
			break;
			case TYPE_NET_PPPOE:
				nConfigValid = 0x0;
				memset(&unionConfig, 0, sizeof(unionConfig));
				pObject = json_object_object_get(pJsonObject, "user");
				if (is_string_object_valid(pObject, buffer, sizeof(buffer))) {
					debug(LOG_INFO, "Get user object success!");
					strncpy(unionConfig.configPPPoE.user, strip_whitespace(buffer), sizeof(unionConfig.configPPPoE.user) - 1);
					nConfigValid |= 0x01;
				}
	
				pObject = json_object_object_get(pJsonObject, "passwd");
				if (is_string_object_valid(pObject, buffer, sizeof(buffer))) {
					debug(LOG_INFO, "Get passwd object success!");
					strncpy(unionConfig.configPPPoE.passwd, strip_whitespace(buffer), sizeof(unionConfig.configPPPoE.passwd) - 1);
					nConfigValid |= 0x10;
				}
				if (0x11 == nConfigValid) {
					debug(LOG_INFO, "Start to set wan connect with pppoe...");
					update_network_connect_type(nNetType, &unionConfig);
					nRet = 0;
				}
			break;
			default:
			break;
		}
	}
	debug(LOG_ERR, "Get netType object success!");
	return nRet;
}

static int set_user_wifi(struct json_object *pJsonObject)
{
	char bufferSSID[256] = {0};
	char bufferPasswd[256] = {0};
	struct json_object *pObjectSSID = json_object_object_get(pJsonObject, "ssid");
	struct json_object *pObjectPasswd = json_object_object_get(pJsonObject, "passwd");
	
	if (is_string_object_valid(pObjectSSID, bufferSSID, sizeof(bufferSSID))
			&& is_string_object_valid(pObjectPasswd, bufferPasswd, sizeof(bufferPasswd))) {
		debug(LOG_INFO, "Get ssid and passwd object success!");
		update_user_wifi_config(strip_whitespace(bufferSSID), strip_whitespace(bufferPasswd));
		return 0;
	}
	debug(LOG_ERR, "Get ssid or passwd object failed!");
	return -1;
}

static int set_guest_wifi(struct json_object *pJsonObject)
{
	char bufferSuffix[256] = {0};
	struct json_object *pObject = json_object_object_get(pJsonObject, "suffix");

	if (is_string_object_valid(pObject, bufferSuffix, sizeof(bufferSuffix))) {
		debug(LOG_INFO, "Get suffix object success!");
		update_guest_wifi_suffix(strip_whitespace(bufferSuffix));
		return 0;
	}
	debug(LOG_ERR, "Get suffix object failed!");
	return -1;
}

static int handle_router_set_request(struct json_object *pJsonObject, ResponseType_t type)
{
	int nRet = -1;
	switch (type) {
		case TYPE_SHARE_SET:
			nRet = set_network_share_state(pJsonObject);
		break;
		case TYPE_CONNECT_SET:
			nRet = set_network_connect_type(pJsonObject);
		break;
		case TYPE_USER_WIFI_SET:
			nRet = set_user_wifi(pJsonObject);
		break;
		case TYPE_GUEST_WIFI_SET:
			nRet = set_guest_wifi(pJsonObject);
		break;
		default:
		break;
	}
	return nRet;
}

static int set_socket_reuseable(int fd)
{
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(&opt)) < 0) {        
        debug(LOG_ERR, "set_socket_reuseable failed: %s!",strerror(errno));
        return -1;
    }
    return 0;
}


static void init_guest_wifi_state()
{
	int nTimeClosed = 0;
	long nTimeStamp = 0;
	char *pMinutesClosed = nvram_get("Minutes_guest_close");
	g_nMaxCloseTime = nvram_get_int("MaxTime_guest_close");

	if ((g_nMaxCloseTime < 0) || (g_nMaxCloseTime > 24 * 60)) {
		debug(LOG_ERR, "Get max time that guest wifi can be closed is invalid, set it to 0!");
		g_nMaxCloseTime = 0;
	}
	if (pMinutesClosed) {
		if (2 == sscanf(pMinutesClosed, "%d:%ld", &nTimeClosed, &nTimeStamp)) {
			g_nCloseTimeUsed = nTimeClosed;
			g_nTimeStampToday = nTimeStamp;
			if (g_nCloseTimeUsed <= 0) {
				g_nCloseTimeUsed = 0;
			}
			if ((time(NULL) - nTimeStamp) / 86400 >= 1) {
				g_nCloseTimeUsed = 0;
			}
		}
	} else {
		debug(LOG_ERR, "Cannot get Minutes_guest_close from falsh!");
		g_nCloseTimeUsed = 0;
	}
	
	if (g_nTimeStampToday <= 0) {
		g_nTimeStampToday = time(NULL);
	}
	g_nGuestSwitchState = OPEN_SHARE;

	refresh_guest_wifi_close_time(1);
	/* 然后在与JAVA交互的线程中(该线程每分钟运行一次)，根据当前是否关闭状态而去刷新剩余可关闭时长 */
	
}

static int init_listen_socket(int *pFdUDP, int *pFdTCP)
{
	struct sockaddr_in addrUDPServer;
	struct sockaddr_in addrTCPServer;

	/* UDP socket bind */
	*pFdUDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == *pFdUDP) {
		debug(LOG_ERR, "create udp socket failed: %s", strerror(errno));
		return -1;
	}

	set_socket_reuseable(*pFdUDP);

	memset(&addrUDPServer, 0, sizeof(addrUDPServer));
	addrUDPServer.sin_family = AF_INET;
	addrUDPServer.sin_port = htons(APP_CONTROL_PORT);
	addrUDPServer.sin_addr.s_addr = htonl(INADDR_ANY);

	if (-1 == bind(*pFdUDP, (struct sockaddr *)&addrUDPServer, sizeof(addrUDPServer))) {
		debug(LOG_ERR, "Bind udp socket failed: %s", strerror(errno));
		close_fd(pFdUDP);
		return -1;
	}

	/* TCP socket bind */
	*pFdTCP = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == *pFdTCP) {
		debug(LOG_ERR, "create tcp socket failed: %s", strerror(errno));
		close_fd(pFdUDP);
		return -1;
	}

	set_socket_reuseable(*pFdTCP);

	memset(&addrTCPServer, 0, sizeof(addrTCPServer));
	addrTCPServer.sin_family = AF_INET;
	addrTCPServer.sin_port = htons(APP_CONTROL_PORT);
	addrTCPServer.sin_addr.s_addr = htonl(INADDR_ANY);
	if (-1 == bind(*pFdTCP, (struct sockaddr *)&addrTCPServer, sizeof(addrTCPServer))) {
		debug(LOG_ERR, "Bind tcp socket failed: %s", strerror(errno));
		close_fd(pFdUDP);
		close_fd(pFdTCP);
		return -1;
	}

	if (-1 == listen(*pFdTCP, 10)) {
		debug(LOG_ERR, "Listen tcp socket failed: %s", strerror(errno));
		close_fd(pFdUDP);
		close_fd(pFdTCP);
		return -1;
	}

	return 0;
}

static int makeResponseKey(char *buffer)
{
	if (0 == g_timeStampKey) {
		g_timeStampKey = time(NULL);
	}
	if (0 == get_key_encrypt(g_timeStampKey, buffer)) {
		debug(LOG_INFO, "Make encrypt for timestamp success!");
		return 0;
	} else {
		debug(LOG_ERR, "Make encrypt for timestamp failed!");
		return -1;
	}
}


static struct json_object *make_response_json_of_router_ip()
{
	char buffer[33] = {0};
	struct json_object *pJsonObject = NULL, *pObjectIP = NULL, *pObjectResult = NULL, *pObjectKey = NULL;
	if (0 != makeResponseKey(buffer)) {
		goto Err;
	}
	pJsonObject = json_object_new_object();
	if (pJsonObject) {
		pObjectIP = json_object_new_string(g_ipRouter);
		pObjectResult = json_object_new_int(TYPE_ROUTER_IP);
		pObjectKey = json_object_new_string(buffer);
		if (pObjectIP && pObjectResult && pObjectKey) {
			json_object_object_add(pJsonObject, "result", pObjectResult);
			json_object_object_add(pJsonObject, "ip", pObjectIP);
			json_object_object_add(pJsonObject, "key", pObjectKey);
			debug(LOG_INFO, "Make response json of router ip success!");
			goto End;
		}
	}

Err:
	debug(LOG_ERR, "Make response json of router ip failed!");
	free_json_object_safe(pJsonObject);
	free_json_object_safe(pObjectIP);
	free_json_object_safe(pObjectResult);
	free_json_object_safe(pObjectKey);
End:
	return pJsonObject;
}

static struct json_object *make_response_json_of_share_info()
{
	char buffer[33] = {0};
	struct json_object* pJsonObject = NULL, *pObjectResult = NULL, *pObjectLeftTime = NULL, *pObjectSwitchState = NULL, *pObjectKey = NULL;
	if (0 != makeResponseKey(buffer)) {
		goto Err;
	}
	pJsonObject = json_object_new_object();
	if (pJsonObject) {
		pObjectResult = json_object_new_int(TYPE_SHARE_INFO);
		pObjectLeftTime = json_object_new_int(g_nMaxCloseTime - g_nCloseTimeUsed);
		pObjectSwitchState = json_object_new_string((g_nGuestSwitchState == CLOSE_SHARE)? "close": "open");
		pObjectKey = json_object_new_string(buffer);
		if (pObjectResult && pObjectLeftTime && pObjectSwitchState && pObjectKey) {
			json_object_object_add(pJsonObject, "result", pObjectResult);
			json_object_object_add(pJsonObject, "leftTime", pObjectLeftTime);
			json_object_object_add(pJsonObject, "switches", pObjectSwitchState);
			json_object_object_add(pJsonObject, "key", pObjectKey);
			debug(LOG_INFO, "Make response json of share info success!");
			goto End;
		}
	}

Err:
	debug(LOG_ERR, "Make response json of share info failed!");
	free_json_object_safe(pJsonObject);
	free_json_object_safe(pObjectResult);
	free_json_object_safe(pObjectLeftTime);
	free_json_object_safe(pObjectSwitchState);
	free_json_object_safe(pObjectKey);
End:
	return pJsonObject;

}

static struct json_object *make_response_json_of_connect_info()
{
	char buffer[33] = {0};
	NetType_t nConnectType = -1;
	struct json_object *pJsonObject = NULL, *pObjectResult = NULL, *pObjectKey = NULL, *pObjectConnectType = NULL;
	struct json_object *pObjectIP = NULL, *pObjectMask = NULL, *pObjectGateway = NULL, *pObjectDns1 = NULL, *pObjectDns2 = NULL;
	struct json_object *pObjectPPPoEUser = NULL, *pObjectPPPoEPasswd = NULL;
	char *pConnectType = nvram_get("wan_proto");
	if (pConnectType) {
		if (strcmp(pConnectType, "dhcp")) {
			nConnectType = TYPE_NET_AUTO;
		} else if (strcmp(pConnectType, "static")) {
			nConnectType = TYPE_NET_STATIC;
		} else if (strcmp(pConnectType, "pppoe")) {
			nConnectType = TYPE_NET_PPPOE;
		}
	} else {
		debug(LOG_ERR, "Get wan connect proto failed!");
		goto Err;
	}
	if (0 != makeResponseKey(buffer)) {
		goto Err;
	}
	pJsonObject = json_object_new_object();
	if (pJsonObject) {
		char *pIP = NULL, *pMask = NULL, *pGateway = NULL, *pDns1 = NULL, *pDns2 = NULL, *pUser = NULL, *pPasswd = NULL;
		char bufferPasswd[512] = {0};

		pObjectResult = json_object_new_int(TYPE_CONNECT_INFO);
		pObjectKey = json_object_new_string(buffer);
		pObjectConnectType = json_object_new_int(nConnectType);
		if (pObjectResult && pObjectKey && pObjectConnectType) {
			json_object_object_add(pJsonObject, "result", pObjectResult);
			json_object_object_add(pJsonObject, "key", pObjectKey);
			json_object_object_add(pJsonObject, "netType", pObjectConnectType);
		} else {
			goto Err;
		}

		switch (nConnectType) {
			case TYPE_NET_AUTO:
				debug(LOG_INFO, "Make response json of user wifi info success!");
				goto End;
			break;
			case TYPE_NET_STATIC:
				pIP = nvram_get("wan_ipaddr");
				pMask = nvram_get("wan_netmask");
				pGateway = nvram_get("wan_gateway");
				pDns1 = nvram_get("wan_dns1_x");
				pDns2 = nvram_get("wan_dns2_x");
				if (pIP && pMask && pGateway && pDns1 && pDns2) {
					pObjectIP = json_object_new_string(pIP);
					pObjectMask = json_object_new_string(pMask);
					pObjectGateway = json_object_new_string(pGateway);
					pObjectDns1 = json_object_new_string(pDns1);
					pObjectDns2 = json_object_new_string(pDns2);
					if (pObjectIP && pObjectMask && pObjectGateway && pObjectDns1 && pObjectDns2) {
						json_object_object_add(pJsonObject, "ip", pObjectIP);
						json_object_object_add(pJsonObject, "mask", pObjectMask);
						json_object_object_add(pJsonObject, "gateway", pObjectGateway);
						json_object_object_add(pJsonObject, "dns1", pObjectDns1);
						json_object_object_add(pJsonObject, "dns2", pObjectDns2);
						debug(LOG_INFO, "Make response json of user wifi info success!");
						goto End;
					} else {
						debug(LOG_ERR, "Json object new string failed!");
						goto Err;
					}
				} else {
					debug(LOG_ERR, "Get static network config failed!");
					goto Err;
				}
			break;
			case TYPE_NET_PPPOE:
				pUser = nvram_get("wan_pppoe_username");
				pPasswd = nvram_get("wan_pppoe_passwd");
				if (pUser && pPasswd) {
					if (0 != des_encryption(pPasswd, KEY_DES, bufferPasswd)) {
						debug(LOG_ERR, "Make encrypt for pppoe passwd failed!");
						goto Err;
					}
					pObjectPPPoEUser = json_object_new_string(pUser);
					pObjectPPPoEPasswd = json_object_new_string(bufferPasswd);
					if (pObjectPPPoEUser && pObjectPPPoEPasswd) {
						json_object_object_add(pJsonObject, "user", pObjectPPPoEUser);
						json_object_object_add(pJsonObject, "passwd", pObjectPPPoEPasswd);
						debug(LOG_INFO, "Make response json of user wifi info success!");
						goto End;
					} else {
						debug(LOG_ERR, "Json object new string failed!");
						goto Err;
					}
				} else {
					debug(LOG_ERR, "Get pppoe network config failed!");
					goto Err;
				}
			break;
			default:
			break;
		}
	}

Err:
	debug(LOG_ERR, "Make response json of user wifi info failed!");
	free_json_object_safe(pJsonObject);
	free_json_object_safe(pObjectResult);
	free_json_object_safe(pObjectConnectType);
	free_json_object_safe(pObjectIP);
	free_json_object_safe(pObjectMask);
	free_json_object_safe(pObjectGateway);
	free_json_object_safe(pObjectDns1);
	free_json_object_safe(pObjectDns2);
	free_json_object_safe(pObjectPPPoEUser);
	free_json_object_safe(pObjectPPPoEPasswd);
End:
	return pJsonObject;

}

static struct json_object *make_response_json_of_user_wifi_info()
{
	char buffer[33] = {0};
	char bufferPasswd[512] = {0};
	char *pSSID = NULL, *pPasswd = NULL;
	struct json_object *pJsonObject = NULL, *pObjectResult = NULL, *pObjectSSID = NULL, *pObjectPasswd = NULL, *pObjectKey = NULL;
	pSSID = nvram_get("rt_ssid");
	pPasswd = nvram_get("rt_wpa_psk");
	if (0 != makeResponseKey(buffer) || !pSSID || !pPasswd) {
		goto Err;
	}
	if (0 != des_encryption(pPasswd, KEY_DES, bufferPasswd)) {
		debug(LOG_ERR, "Make encrypt for wifi passwd failed!");
		goto Err;
	}
	pJsonObject = json_object_new_object();
	if (pJsonObject) {
		pObjectResult = json_object_new_int(TYPE_USER_WIFI_INFO);
		pObjectSSID = json_object_new_string(pSSID);
		pObjectPasswd = json_object_new_string(bufferPasswd);
		pObjectKey = json_object_new_string(buffer);
		if (pObjectResult && pObjectSSID && pObjectPasswd && pObjectKey) {
			json_object_object_add(pJsonObject, "result", pObjectResult);
			json_object_object_add(pJsonObject, "ssid", pObjectSSID);
			json_object_object_add(pJsonObject, "passwd", pObjectPasswd);
			json_object_object_add(pJsonObject, "key", pObjectKey);
			debug(LOG_INFO, "Make response json of user wifi info success!");
			goto End;
		}
	}

Err:
	debug(LOG_ERR, "Make response json of user wifi info failed!");
	free_json_object_safe(pJsonObject);
	free_json_object_safe(pObjectResult);
	free_json_object_safe(pObjectSSID);
	free_json_object_safe(pObjectPasswd);
	free_json_object_safe(pObjectKey);
End:
	return pJsonObject;

}

static struct json_object *make_response_json_of_guest_wifi_info()
{
	char buffer[33] = {0};
	char *pSuffixSSID = nvram_get("rt_guest_ssid_suffix");
	char *pPrefixSSID = nvram_get("rt_guest_ssid_prefix");
	struct json_object *pJsonObject = NULL, *pObjectResult = NULL, *pObjectPrefix = NULL, *pObjectSuffix = NULL, *pObjectKey = NULL;
	if (0 != makeResponseKey(buffer) || !pSuffixSSID || !pPrefixSSID) {
		goto Err;
	}
	pJsonObject = json_object_new_object();
	if (pJsonObject) {
		pObjectResult = json_object_new_int(TYPE_GUEST_WIFI_INFO);
		pObjectPrefix = json_object_new_string(pPrefixSSID);
		pObjectSuffix = json_object_new_string(pSuffixSSID);
		pObjectKey = json_object_new_string(buffer);
		if (pObjectResult && pObjectSuffix && pObjectKey) {
			json_object_object_add(pJsonObject, "result", pObjectResult);
			json_object_object_add(pJsonObject, "prefix", pObjectPrefix);
			json_object_object_add(pJsonObject, "suffix", pObjectSuffix);
			json_object_object_add(pJsonObject, "key", pObjectKey);
			debug(LOG_INFO, "Make response json of guest wifi info success!");
			goto End;
		}
	}

Err:
	debug(LOG_ERR, "Make response json of guest wifi info failed!");
	free_json_object_safe(pJsonObject);
	free_json_object_safe(pObjectResult);
	free_json_object_safe(pObjectSuffix);
	free_json_object_safe(pObjectKey);
End:
	return pJsonObject;
}

static struct json_object* make_response_json_of_invalid_param()
{
	char buffer[33] = {0};
	struct json_object *pJsonObject = NULL, *pObjectResult = NULL, *pObjectKey = NULL;
	if (0 != makeResponseKey(buffer)) {
		goto Err;
	}
	pJsonObject = json_object_new_object();
	if (pJsonObject) {
		pObjectResult = json_object_new_int(TYPE_INVALID_PARAM);
		pObjectKey = json_object_new_string(buffer);
		if (pObjectResult && pObjectKey) {
			json_object_object_add(pJsonObject, "result", pObjectResult);
			json_object_object_add(pJsonObject, "key", pObjectKey);
			debug(LOG_INFO, "Make response json of invalid param success!");
			goto End;
		}
	}
Err:
	debug(LOG_ERR, "Make response json of invalid param failed!");
	free_json_object_safe(pJsonObject);
	free_json_object_safe(pObjectResult);
	free_json_object_safe(pObjectKey);
End:
	return pJsonObject;

}

static struct json_object *make_response_json_of_invalid_key()
{
	char buffer[33] = {0};
	struct json_object *pJsonObject = NULL, *pObjectResult = NULL, *pObjectKey = NULL;
	if (0 != makeResponseKey(buffer)) {
		goto Err;
	}
	pJsonObject = json_object_new_object();
	if (pJsonObject) {
		pObjectResult = json_object_new_int(TYPE_INVALID_KEY);
		pObjectKey = json_object_new_string(buffer);
		if (pObjectResult && pObjectKey) {
			json_object_object_add(pJsonObject, "result", pObjectResult);
			json_object_object_add(pJsonObject, "key", pObjectKey);
			debug(LOG_INFO, "Make response json of invalid key success!");
			goto End;
		}
	}
Err:
	debug(LOG_ERR, "Make response json of invalid key failed!");
	free_json_object_safe(pJsonObject);
	free_json_object_safe(pObjectResult);
	free_json_object_safe(pObjectKey);
End:
	return pJsonObject;

}

struct json_object* make_response_json_of_set_success(ResponseType_t type)
{
		char buffer[33] = {0};
		struct json_object *pJsonObject = NULL, *pObjectResult = NULL, *pObjectKey = NULL;
		if (0 != makeResponseKey(buffer)) {
			goto Err;
		}
		pJsonObject = json_object_new_object();
		if (pJsonObject) {
			pObjectResult = json_object_new_int(type);
			pObjectKey = json_object_new_string(buffer);
			if (pObjectResult && pObjectKey) {
				json_object_object_add(pJsonObject, "result", pObjectResult);
				json_object_object_add(pJsonObject, "key", pObjectKey);
				debug(LOG_INFO, "Make response json of set OK success!");
				goto End;
			}
		}
	Err:
		debug(LOG_ERR, "Make response json of set OK failed!");
		free_json_object_safe(pJsonObject);
		free_json_object_safe(pObjectResult);
		free_json_object_safe(pObjectKey);
	End:
		return pJsonObject;

}


static void send_response_data(ResponseType_t type, int fd)
{
	struct json_object *pJsonObject = NULL;
	const char *ptr = NULL;
	switch (type) {
		case TYPE_ROUTER_IP:
			pJsonObject = make_response_json_of_router_ip();
		break;
		case TYPE_SHARE_INFO:
			pJsonObject = make_response_json_of_share_info();
		break;
		case TYPE_CONNECT_INFO:
			pJsonObject = make_response_json_of_connect_info();
		break;
		case TYPE_USER_WIFI_INFO:
			pJsonObject = make_response_json_of_user_wifi_info();
		break;
		case TYPE_GUEST_WIFI_INFO:
			pJsonObject = make_response_json_of_guest_wifi_info();
		break;
		case TYPE_INVALID_PARAM:
			pJsonObject = make_response_json_of_invalid_param();
		break;
		case TYPE_INVALID_KEY:
			pJsonObject = make_response_json_of_invalid_key();
		break;
		case TYPE_SHARE_SET:
		case TYPE_CONNECT_SET:
		case TYPE_USER_WIFI_SET:
		case TYPE_GUEST_WIFI_SET:
			pJsonObject = make_response_json_of_set_success(type);
		break;
		default:
		break;
	}
	if (pJsonObject) {
		int len = 0;
		ptr = json_object_to_json_string(pJsonObject);
		if (ptr) {
			debug(LOG_INFO, "Response message to be send: %s", ptr);
			len = strlen(ptr);
			if (fd == g_fdUDPServer) {
				if (sendto(fd, ptr, len, 0, (struct sockaddr*)&g_addrUdpClient, g_lenUdpClient) > 0) {
					debug(LOG_INFO, "Send udp data success!");
				} else {
					debug(LOG_INFO, "Send udp data failed: %s", strerror(errno));
				}
			} else {
				if (write(fd, ptr, len) > 0) {
					debug(LOG_INFO, "Send response OK!");
				} else {
					debug(LOG_ERR, "Send response failed: %s", strerror(errno));
				}
			}
		} else {
			debug(LOG_ERR, "Json to string failed!");
		}
		json_object_put(pJsonObject);
		pJsonObject = NULL;
	}
}


static void operate_json_data(char *jsonBuffer, int fd)
{
	struct json_object *pJsonObject = json_tokener_parse(jsonBuffer);
	if (!is_error(pJsonObject)) {
		debug(LOG_INFO, "Get json object success!");
		struct json_object *pObject = json_object_object_get(pJsonObject, "type");
		if (!is_error(pObject)) {
			debug(LOG_INFO, "Get type object success!");
			int type = json_object_get_int(pObject);
			if (fd == g_fdUDPServer) {
				/* UDP广播数据 */
				pObject = json_object_object_get(pJsonObject, "mac");
				if (!is_error(pObject)) {
					debug(LOG_INFO, "Get mac object success!");
					const char *pMac = json_object_get_string(pObject);
					if (pMac && strcasestr(pMac, g_macRouter)) {
						send_response_data(TYPE_ROUTER_IP, fd);
					} else {
						debug(LOG_WARNING, "Udp request mac is not my mac, ignore it!");
					}
					goto End;
				}
			} else {
				/* TCP请求数据 */
				pObject = json_object_object_get(pJsonObject, "key");
				if (!is_error(pObject)) {
					debug(LOG_INFO, "Get key object success!");
					const char *pKey = json_object_get_string(pObject);
					if (pKey && (get_key_real(pKey) == (g_timeStampKey + 1))) {
						debug(LOG_INFO, "OK, client key is valid!");
						/* key合法 */
						g_timeStampKey = time(NULL);
						switch (type) {
							case TYPE_SHARE_INFO:
							case TYPE_CONNECT_INFO:
							case TYPE_USER_WIFI_INFO:
							case TYPE_GUEST_WIFI_INFO:
								send_response_data(type, fd);
								goto End;
							break;
							case TYPE_SHARE_SET:
							case TYPE_CONNECT_SET:
							case TYPE_USER_WIFI_SET:
							case TYPE_GUEST_WIFI_SET:
								//if (0) {
								if (0 == handle_router_set_request(pJsonObject, type)) {
									debug(LOG_INFO, "handle router set request success!");
									send_response_data(type, fd);
									goto End;
								} else {
									debug(LOG_ERR, "handle router set request failed!");
								}
							break;
							default:
							break;
						}
					} else {
						if (pKey) {
							/* 返回有key,但是不合法 */
							debug(LOG_ERR, "Key is invalid, server key + 1: %ld, client key: %ld", g_timeStampKey + 1, get_key_real(pKey));
							send_response_data(TYPE_INVALID_KEY, fd);
							goto End;
						}
					}
				}
			}
		}
	}

//Err:
	send_response_data(TYPE_INVALID_PARAM, fd);
End:
	if (!is_error(pJsonObject)) {
		json_object_put(pJsonObject);
		pJsonObject = NULL;
	}
}

static void handle_socket_buffer(char *buffer, int fd)
{
	char *pJson = strchr(buffer, '{');
	if (NULL == pJson) {
		send_response_data(TYPE_INVALID_PARAM, fd);
		return;
	}
	operate_json_data(buffer, fd);
}

static void read_udp_socket_data()
{
	char buffer[1024] = {0};
	memset(&g_addrUdpClient, 0, sizeof(g_addrUdpClient));
	g_lenUdpClient = sizeof(g_addrUdpClient);
	recvfrom(g_fdUDPServer, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&g_addrUdpClient, &g_lenUdpClient);
	debug(LOG_INFO, "Recevied json data from udp socket: %s", buffer);
	handle_socket_buffer(buffer, g_fdUDPServer);
}

static void read_tcp_socket_data(int fd)
{
	char buffer[1024] = {0};
	ssize_t len = read(fd, buffer, sizeof(buffer) - 1);
	debug(LOG_INFO, "Recevied json data from tcp socket: %s", buffer);
	if (0 == len) {
		debug(LOG_INFO, "Client is closed, remove it from epoll and close this socket!");
		if (-1 == epoll_ctl(g_fdEpoll, EPOLL_CTL_DEL, fd, NULL)) {
			debug(LOG_WARNING, "epoll_ctl failed: %s", strerror(errno));
		}
		close(fd);
	} else if (-1 == len) {
		debug(LOG_ERR, "read tcp socket failed: %s", strerror(errno));
		usleep(200000);
	} else {
		handle_socket_buffer(buffer, fd);
	}
}

void thread_app_control(void *arg)
{
	g_nExitEpoll = False;
	/* 创建epoll，括号里面的参数代表监听的设备数。目前该函数的参数在使用时被忽略 */
	g_fdEpoll = epoll_create(2);
	/* epoll事件数组，主要用于epoll_wait参数，代表每次epoll_wait返回时最多可处理的消息
	  (也可设置成1，这样同时有多个事件的话，会反复进行epoll_wait的返回)。 */
	struct epoll_event events[MAX_EVENTS];
	int nRet = -1;
	
	if (-1 == g_fdEpoll) {
		debug(LOG_ERR, "epoll create failed: %s", strerror(errno));
		return;
	}

	init_guest_wifi_state();

	if (-1 == init_listen_socket(&g_fdUDPServer, &g_fdTCPServer)) {
		debug(LOG_ERR, "Init listen socket failed!");
		goto Err;
	}

	/* 创建一个epoll_event，用于epoll_ctl。一个就够了，可以重复使用。 
	 * event的类型可以有：EPOLLIN/EPOLLOUT/EPOLLRDHUP/EPOLLPRI/EPOLLERR/EPOLLHUP/EPOLLET/EPOLLONESHOT，最常用的，
	 * 基本上够用的是EPOLLIN和EPOLLOUT
	 */
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = g_fdUDPServer;
	if (-1 == epoll_ctl(g_fdEpoll, EPOLL_CTL_ADD, g_fdUDPServer, &event)) {
		debug(LOG_ERR, "epoll_ctl failed: %s", strerror(errno));
		goto Err;
	}

	/* 这里对epoll_event进行了复用 */
	event.data.fd = g_fdTCPServer;
	if (-1 == epoll_ctl(g_fdEpoll, EPOLL_CTL_ADD, g_fdTCPServer, &event)) {
		debug(LOG_ERR, "epoll_ctl failed: %s", strerror(errno));
		goto Err;
	}

	{
		char *ptr = get_iface_mac(WIFI_INTERFACE);
		if (ptr) {
			strncpy(g_macRouter, ptr, sizeof(g_macRouter) - 1);
			free(ptr);
			ptr = NULL;
			debug(LOG_INFO, "Get wifi mac: %s", g_macRouter);
		} else {
			debug(LOG_ERR, "Get wifi mac failed!");
			goto Err;
		}

		ptr = get_iface_ip(GATEWAY_INTERFACE);
		if (ptr) {
			strncpy(g_ipRouter, ptr, sizeof(g_ipRouter) - 1);
			free(ptr);
			ptr = NULL;
			debug(LOG_INFO, "Get gateway ip: %s", g_ipRouter);
		} else {
			debug(LOG_ERR, "Get gateway ip failed!");
			goto Err;
		}
	}

	debug(LOG_INFO, "Start to app control socket listen...");
	while (1) {
		int i = 0;
		if (g_nExitEpoll) {
			break;
		}
		/* 多路复用开始，其中MAX_EVENTS与events数组大小相对应 */
		nRet = epoll_wait(g_fdEpoll, events, MAX_EVENTS, TIMEOUT_EPOLL);
		switch (nRet) {
			case -1:
				/* 返回出错 */
				debug(LOG_ERR, "epoll_wait failed: %s", strerror(errno));
				usleep(2000000);
				continue;
			break;
			case 0:
				/* 代表超时 */
				debug(LOG_WARNING, "epoll timeout? Are you sure?");
				usleep(1000000);
				continue;
			break;
			default:
				/* 正数，代表返回的事件个数，下面进行遍历 */
			break;
		}

		for (i = 0; i < nRet; ++i) {
			if ((events[i].data.fd == g_fdUDPServer) 
				 && (events[i].events & EPOLLIN)) {
				 /* 监听的端口之一：udp服务端口 */
				read_udp_socket_data();
			} else if ((events[i].data.fd == g_fdTCPServer) 
				 && (events[i].events & EPOLLIN)) {
				 /* 监听的端口之一：tcp服务端口 */
				struct sockaddr_in addrClient;
				socklen_t len = sizeof(addrClient);
				int fdConn = accept(g_fdTCPServer, (struct sockaddr *)&addrClient, &len);
				if (-1 == fdConn) {
					debug(LOG_ERR, "accept new client to connet failed: %s", strerror(errno));
					continue;
				}
				
				event.data.fd = fdConn;
				if (-1 == epoll_ctl(g_fdEpoll, EPOLL_CTL_ADD, fdConn, &event)) {
					debug(LOG_ERR, "epoll_ctl failed: %s", strerror(errno));
					continue;
				}
			} else if (events[i].events & EPOLLIN) {
				/* 动态添加的tcp客户端端口。具体的业务在并发的时候可以放在线程中进行处理。这里只
				 * 是单线程处理，也是实际应用需要。所以要使用多线程的话，需要根据实际应用而定。
				 */
				read_tcp_socket_data(events[i].data.fd);
			}
		}
	}

Err:
	close_app_control_socket_all();
}

void refresh_guest_wifi_close_time(char bInit)
{
	char buffer[20] = {0};
	if ((g_nTimeStampToday - time(NULL)) / 86400 >= 1) {
		g_nTimeStampToday = time(NULL);
		g_nCloseTimeUsed = 0;
	} else {
		if (!bInit) {
			/* 初始化的时候不需要计时 */
			++g_nCloseTimeUsed;
		}
	}
	if (g_nCloseTimeUsed > g_nMaxCloseTime) {
		g_nCloseTimeUsed = 0;
		debug(LOG_INFO, "Guest wifi is closed too long, open it.");
		update_network_share_state(OPEN_SHARE);
	}
	snprintf(buffer, sizeof(buffer), "%d:%ld", g_nCloseTimeUsed, time(NULL));
	nvram_set("Minutes_guest_close", buffer);
	nvram_commit();
}

void close_app_control_socket_all()
{
	close_fd(&g_fdEpoll);
	close_fd(&g_fdUDPServer);
	close_fd(&g_fdTCPServer);
	g_nExitEpoll = True;
}


