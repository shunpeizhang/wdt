#pragma once




struct WDTFileHandleInfo
{
	char fileName[1024];
	long long fileSize;
};


extern "C"{


	int sendHandle(int iStartPort, int numPorts, char *destIp, char *rootDir, WDTFileHandleInfo *fileList, int iFileListLen);
	



}










/*
class SenderHandler
{
public:
	SenderHandler(int iStartPort, int numPorts, const std::string& rootDir);
	~SenderHandler();


	//wdt相关环境初始化完成
	bool init();

	//添加需要处理的文件
	bool addNeedHandleFiles(const std::vector<std::string>& needSendFiles);

	//开启处理
	bool start();

	//对于正在处理的强制停止
	bool stop();

	//等待处理完成
	bool finish();
	

};
*/






