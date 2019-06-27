#define USE_SOCKET
#include "socket_wrapper.h"

using namespace LightGBM;

int ReadBuf(const char* fileFulPath, char** pReadBuf, long *nReadLen)
{
	FILE *pFile = fopen(fileFulPath, "rb");
	if (pFile == NULL)
		return -1;
	
	fseek(pFile, 0, SEEK_END);
	long nFilseSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	// 预留100空间作为越界保护
	char * pFilebuf = (char*)malloc(nFilseSize + 100);
	memset(pFilebuf, 0, nFilseSize + 100);
	nFilseSize = fread(pFilebuf, 1, nFilseSize, pFile);
	fclose(pFile);

	*pReadBuf = pFilebuf;
	*nReadLen = nFilseSize;
}

// https://help.aliyun.com/document_detail/31988.html?spm=a2c4g.11186623.6.1177.517a20bewcyXQP&tdsourcetag=s_pcqq_aiomsg
int PostTest()
{
	TcpSocket::Startup();
	TcpSocket tcpSocke;
	bool bRet = tcpSocke.ConnectUrl("oss-example.oss-cn-hangzhou.aliyuncs.com", 80);
	if (bRet)
	{
		std::string strSend = "POST / HTTP/1.1\r\n\
Host: oss-example.oss-cn-hangzhou.aliyuncs.com\r\n\
Content-Type: multipart/form-data; boundary=9431149156168\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"key\"\r\n\
C:\\Apache24\\bin\\apache.key\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"success_action_status\"\r\n\
200\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"Content-Disposition\"\r\n\
content_disposition\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"x-oss-meta-uuid\"\r\n\
uuid\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"x-oss-meta-tag\"\r\n\
metadata\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"OSSAccessKeyId\"\r\n\
44CF9590006BF252F707\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"policy\"\r\n\
eyJleHBpcmF0aW9uIjoiMjAxMy0xMi0wMVQxMjowMDowMFoiLCJjb25kaXRpb25zIjpbWyJjb250ZW50LWxlbmd0aC1yYW5nZSIsIDAsIDEwNDg1NzYwXSx7ImJ1Y2tldCI6ImFoYWhhIn0sIHsiQSI6ICJhIn0seyJrZXkiOiAiQUJDIn1dfQ==\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"Signature\"\r\n\
kZoYNv66bsmc10+dcGKw5x2PRrk=\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"file\"; filename=\"MyFilename.txt\"\r\n\
Content-Type: text/plain\r\n\
abcdefg\r\n\
--9431149156168\r\n\
Content-Disposition: form-data; name=\"submit\"\r\n\
Upload to OSS\r\n\
--9431149156168--\r\n";

		char * pFilebuf = NULL;
		long nFilseSize = 0;
		ReadBuf("C:\\Apache24\\bin\\apache.key", &pFilebuf, &nFilseSize);
		
		int nLen = strSend.length();
		char szbuf[64] = { 0 };
		sprintf_s(szbuf, "Content-Length: %d\r\n\r\n", nFilseSize);

		strSend.append(szbuf);
		strSend.append(pFilebuf, nFilseSize);
		free(pFilebuf);

		int nSend = tcpSocke.Send(strSend.c_str(), strSend.length());

		// 接收
		char buf[10241] = { 0 };
		int nRecvLen = 10240;
		int nRecv = tcpSocke.Recv(buf, nRecvLen);
		char *pData = strstr(buf, "\r\n\r\n");
		if (pData)
			printf("RecvData:\n%s\n", pData + 4);
		tcpSocke.Close();
	}
	return 0 ;
}


int main(int agrc,char* agrv[])
{
	PostTest();

	TcpSocket::Startup();
	TcpSocket tcpSocke;
	bool bRet = tcpSocke.ConnectUrl("httpbin.org", 80);
	if (bRet)
	{
		// 发送
		char* pSend = "GET /get?City=BeiJing HTTP/1.1\r\n\
Host: httpbin.org\r\n\
Connection: keep - alive\r\n\
User-Agent: Mozilla/5.0(Windows NT 10.0; WOW64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 58.0.3029.110 Safari / 537.36 SE 2.X MetaSr 1.0\r\n\
accept: application/json\r\n\
Accept-Language: zh-CN,zh;q=0.8\r\n\
Accept-Encoding: deflate\r\n\r\n";
//Accept-Encoding: gzip, deflate\r\n\r\n"; // 开启压缩支持,需要解压操作
		int nSend = tcpSocke.Send(pSend, strlen(pSend));
		
		// 接收
		char buf[10241] = { 0 };
		int nLen = 10240;
		int nRecv = tcpSocke.Recv(buf, nLen);
		char *pData = strstr(buf, "\r\n\r\n");
		if (pData)
			printf("HttpGet:\n%s\n", pData + 4);
		tcpSocke.Close();
	}
	return 0;
}