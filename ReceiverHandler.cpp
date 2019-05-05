#include "ReceiverHandler.h"
#include "WdtTransferRequest.h"


#include <folly/String.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include <wdt/Protocol.h>
#include <vector>



extern int wdt_main(int argc, char *argv[], const std::vector<facebook::wdt::WdtFileInfo>& fileList);



int recvHandle(int iStartPort, int numPorts, char *rootDir, int uid, int gid)
{
	//char *argv[] = {"./wdt", "-directory", "/data/work/wdt/dest", "-transfer_id", "1", "-start_port", "22888", "-num_ports", "8", "-log_dir", "/data/work/wdt/log/src"};
	//int argc = 11;

	//char *argv[] = {"./wdt", "-directory", "/data/work/wdt/dest", "-transfer_id", "1", "-log_dir", "/data/work/wdt/log/src"};
	//int argc = 7;

	char *argv[] = {"./wdt", "-directory", rootDir, "-transfer_id", "1"};
	int argc = 5;

	std::vector<facebook::wdt::WdtFileInfo> destFileList;

	return wdt_main(argc, argv, destFileList);
}








/*

// This can be the fbonly (FbWdt) version (extended initialization, and options)
#ifndef WDTCLASS
#define WDTCLASS Wdt
#endif



using namespace facebook::wdt;

// TODO: move this to some util and/or delete
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::set<T> &v) {
  std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
  return os;
}





ReceiverHandler::ReceiverHandler(int iStartPort, int numPorts, const std::string& rootDir):
	startPort_(iStartPort), numPorts_(numPorts), rootDir_(rootDir)
{
	char szBuf[128];
	snprintf(szBuf, "ReceiverHandler_%d", iStartPort);
	appName_ = szBuf;
}

ReceiverHandler::~ReceiverHandler()
{


}


//wdt相关环境初始化完成
bool ReceiverHandler::init()
{
	// Might be a sub class (fbonly wdtCmdLine.cpp)
	Wdt &wdt = WDTCLASS::initializeWdt(appName_);
	WdtOptions &options = wdt.getWdtOptions();

	ErrorCode retCode = OK;

	// General case : Sender or Receiver
	std::unique_ptr<WdtTransferRequest> reqPtr;
	{
		reqPtr = std::make_unique<WdtTransferRequest>(
			options.start_port, options.num_ports, FLAGS_directory);
		reqPtr->hostName = FLAGS_destination;
		reqPtr->transferId = FLAGS_transfer_id;
		reqPtr->ivChangeInterval = options.iv_change_interval_mb * kMbToB;
		reqPtr->tls = wdt.isTlsEnabled();
	}
	
	WdtTransferRequest &req = *reqPtr;
	req.wdtNamespace = FLAGS_namespace;
	if (!FLAGS_dest_id.empty()) {
		req.destIdentifier = FLAGS_dest_id;
	}
	if (FLAGS_protocol_version > 0) {
		req.protocolVersion = FLAGS_protocol_version;
	}
	if (!FLAGS_hostname.empty()) {
		reqPtr->hostName = FLAGS_hostname;
	}
	if (FLAGS_destination.empty() && connectUrl.empty()) {
		Receiver receiver(req);
		WdtOptions &recOptions = receiver.getWdtOptions();
		if (FLAGS_run_as_daemon) {
		  // Backward compatible with static ports, you can still get dynamic
		  // daemon ports using -start_port 0 like before
		  recOptions.static_ports = true;
		}
		if (!FLAGS_recovery_id.empty()) {
		  // TODO: add a test for this
		  recOptions.enable_download_resumption = true;
		  receiver.setRecoveryId(FLAGS_recovery_id);
		}
		wdt.wdtSetReceiverSocketCreator(receiver);
		WdtTransferRequest augmentedReq = receiver.init();
		retCode = augmentedReq.errorCode;
		if (retCode == FEWER_PORTS) {
		  if (FLAGS_treat_fewer_port_as_error) {
			WLOG(ERROR) << "Receiver could not bind to all the ports";
			return false;
		  }
		  retCode = OK;
		} else if (augmentedReq.errorCode != OK) {
		  WLOG(ERROR) << "Error setting up receiver " << errorCodeToStr(retCode);
		  return false;
		}
		// In the log:
		WLOG(INFO) << "Starting receiver with connection url "
				   << augmentedReq.getLogSafeString();	// The url without secret
		// on stdout: the one with secret:
		std::cout << augmentedReq.genWdtUrlWithSecret() << std::endl;
		std::cout.flush();

		return true;
	} 
	  
	return false;
}

//可以正常接收文件数据
bool ReceiverHandler::start();
{

}

//对于正在处理的强制停止
bool ReceiverHandler::stop()
{

}

//等待处理完成
bool ReceiverHandler::finish()
{

}

//清理状态信息
void ReceiverHandler::clear()
{


}
*/





