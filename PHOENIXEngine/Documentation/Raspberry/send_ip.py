#!\usr\bin\env python
#-*-coding: utf-8-*-
import socket
import time
import smtplib
import urllib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.image import MIMEImage
from email.utils import formataddr

#���������ͨ��
def check_network():
    while True:
        try:
            result=urllib.urlopen('https://www.qq.com/').read()
            print result
            print "Network is Ready!"
            break
        except Exception,e:
            print e
            print "Network is not ready,Sleep 5s..."
            time.sleep(5)
    return True
	
#��ȡ�����ƶ��ӿڵ�ip��ַ
def get_ip_address():
s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    s.connect(("1.1.1.1",80))
    ipaddr=s.getsockname()[0]
    s.close()
    return ipaddr

def sendEmail():
    ret=True
    ipaddr=get_ip_address();
    try:
        msg=MIMEText(ipaddr,'plain','utf-8')
        msg['From']=formataddr(["�û���",'manyxu@foxmail.com'])
        msg['To']=formataddr(["�ռ���",'manyxu@foxmail.com'])
        msg['Subject']="Raspberry IP"
        server=smtplib.SMTP_SSL("smtp.qq.com",465)
        server.login("manyxu@foxmail.com","SMTP����Ȩ��")
        server.sendmail('manyxu@foxmail.com',['manyxu@foxmail.com',],msg.as_string())
        server.quit()
        print('send email suc')
        ret=True
    except:
        ret=False
if  __name__ == '__main__' :
    check_network()
    sendEmail()