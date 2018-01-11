void 
arp_get_ip(const char *mac, char *ip, const int lenIP)
{
    FILE *pFile = NULL;
    char strBuffer[256] = {0};
    char strCMD[256] = {0};
    char *pTemp = NULL;
	if (!mac || !ip) {
        debug(LOG_ERR, "Invalid input mac or ip.");
        return;
    }
    snprintf(strCMD, sizeof(strCMD), "cat /proc/net/arp | grep %s | cut -f 1 -d ' '", mac);
    pFile = popen(strCMD, "r");
    if (NULL == pFile) {
        debug(LOG_ERR, "Run popen failed!");
        return;
    }
    memset(strBuffer, 0, sizeof(strBuffer));
    if (fgets(strBuffer, sizeof(strBuffer), pFile) > 0) {
        if (pTemp = strchr(strBuffer, '\n')) { /* 去除可能存在的换行符 */
            pTemp[0] = '\0';
        }
        strncpy(ip, strBuffer, strlen(strBuffer) < lenIP? strlen(strBuffer): lenIP);
        ip[lenIP] = '\0';
    } else {
        debug(LOG_ERR, "Cann't find ip from mac[%s].", strCMD);
    }
    pclose(pFile);
}