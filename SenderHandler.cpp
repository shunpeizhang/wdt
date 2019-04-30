#include "SenderHandler.h"
#include "WdtTransferRequest.h"

#include <vector>
#include <stdlib.h>


using namespace std;


extern int wdt_main(int argc, char *argv[], const vector<facebook::wdt::WdtFileInfo>& fileList);


int sendHandle(int iStartPort, int numPorts, char *destIp, char *rootDir, WDTFileHandleInfo *fileList, int iFileListLen)
{
	//char *argv[] = {"./wdt", "-directory", "/data/work/wdt/src", "-transfer_id", "1", "-start_port", "22888", "-num_ports", "8", "-log_dir", "./", "-destination", "172.16.11.17"};
	char *argv[] = {"./wdt", "-directory", "/data/work/wdt/src", "-destination", "172.16.11.17", "-transfer_id", "1", "-log_dir", "/data/work/wdt/log/dest"};
	int argc = 9;

	//char *argv[] = {"./wdt", "-directory", "/home/zsp/work/wdt_rsync/rsync/tmp/dest", "-transfer_id", "1", "-start_port", "22888", "-num_ports", "8", "-log_dir", "./", "-destination", "172.16.11.17"};
	//int argc = 13;

	//char *argv[] = {"./wdt", "-destination", "172.16.11.17", "-directory", "/home/zsp/work/wdt_rsync/rsync/tmp/src", "-transfer_id", "1", "-start_port", "22888", "-num_ports", "8", "-log_dir", "./"};
	//int argc = 13;


	vector<facebook::wdt::WdtFileInfo> destFileList;
	for(int iPos = 0; iFileListLen > iPos; ++iPos)
	{
		facebook::wdt::WdtFileInfo info(std::string(fileList[iPos].fileName), fileList[iPos].fileSize, false);

		destFileList.push_back(info);

		printf("sendHandle %s|end\n", fileList[iPos].fileName);
		
	}
	

	return wdt_main(argc, argv, destFileList);
}









