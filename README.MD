
##### Tip

简单，实用，跨平台的socket封装。

写了个简单的HTTP POST和GET操作DEMO，未作异常处理。


##### [socket_wrapper.hpp](https://github.com/microsoft/LightGBM/blob/master/src/network/socket_wrapper.hpp)

socket_wrapper 提取自[LightGBM ](https://github.com/microsoft/LightGBM)

##### LightGBM

LightGBM 是一个梯度 boosting 框架, 使用基于学习算法的决策树. 它是分布式的, 高效的。


##### Demo

```
int main(int agrc,char* agrv[])
{
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
```