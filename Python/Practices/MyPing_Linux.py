#!/usr/bin/env python3
#coding: utf-8

import os
import sys
import re
import time
import smtplib
from email.mime.text import MIMEText
from email.header import Header

mail_host = 'smtp.126.com'
mail_user = 'yanglw115'
mail_passwd = '20100115ylw'

mail_sender = 'yanglw115@126.com'
mail_receivers = ['yangliwei85@dingtalk.com']
#mail_receivers = ['357969275@qq.com']

ip = '192.168.1.196'
# Linux
cmd_ping = "ping -c 10 {} | grep 'packet loss' | cut -d ',' -f 3 | cut -d '%' -f 1 ".format(ip)

while True:
    result_ping = os.popen(cmd_ping, 'r').read()
    #print(str(result_ping))
    result_ping = int(str(result_ping).strip())
    print('Packet lost %d'%(result_ping))
    if result_ping >= 100:
        currentTime = time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))
        message = str('%s: %s is out of power now, please check!'%(currentTime, ip))
        mail_message = MIMEText(message)
        #mail_message['From'] = Header('yanglw115', 'utf-8')
        mail_message['From'] = 'yanglw115@126.com'
        mail_message['To'] = Header('yangliwei85', 'utf-8')
        mail_message['Subject'] = Header('路由器移动电源运行状况', 'utf-8')
        print(message)
        try:
            smtpObj = smtplib.SMTP()
            smtpObj.connect(mail_host)
            smtpObj.login(mail_user, mail_passwd)
            smtpObj.sendmail(mail_sender, mail_receivers, mail_message.as_string())
            print('邮件发送成功@！')
        except smtplib.SMTPException:
            print('邮件发送失败！')
        sys.exit(1)
    else:
        time.sleep(4)
		

