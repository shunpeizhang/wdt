#pragma once





extern "C"{


	int recvHandle(int iStartPort, int numPorts, char *rootDir, int uid, int gid);




}






/*
class ReceiverHandler
{
public:
	ReceiverHandler(int iStartPort, int numPorts, const std::string& rootDir);
	~ReceiverHandler();


	//wdt相关环境初始化完成
	bool init();

	//可以正常接收文件数据
	bool start();

	//对于正在处理的强制停止
	bool stop();

	//等待处理完成
	bool finish();

	//清理状态信息
	void clear();

private:
	int startPort_;
	int numPorts_;
	std::string rootDir_;
	std::string appName_;
};
*/






