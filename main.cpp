#include "ReceiverHandler.h"
#include "SenderHandler.h"
#include "WdtTransferRequest.h"

#include <vector>
#include <stdio.h>

using namespace std;


extern int wdt_main(int argc, char *argv[]);


int main(int argc, char *argv[])
{
	//return recvHandle(22888, 8, "/data/work/wdt/dest", 1001, 1001);

	WDTFileHandleInfo fileList[2];
	{
		snprintf(fileList[0].fileName, sizeof(fileList[0].fileName) - 1, "%s", "t1.txt");
		fileList[0].fileSize = 7933;

		snprintf(fileList[1].fileName, sizeof(fileList[1].fileName) - 1, "%s", "t2.txt");
		fileList[1].fileSize = 34235;
	}
	
	return sendHandle(22888, 8, "172.16.11.17", "/data/work/wdt/src", fileList, 2);

	/*printf("argc:%d\n", argc);
	for(int iPos = 0; argc > iPos; ++iPos)
	{
		printf("%s | ", argv[iPos]);
	}
	printf("\n");

	return wdt_main(argc, argv);*/
}



