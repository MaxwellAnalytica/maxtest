// testdemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "irecoverycore.h"
#include "device_store.h"
#include "notify_item.h"
#include "search_file.h"
#include "recover_item.h"
#include "ScanDocumentation.h"

#include < io.h> 
#include <iostream>

int main()
{
	IRecoveryCore* recovercover_obj = nullptr;
	if (CreateRecoveryCore(&recovercover_obj) < 0) 
	{
		std::cout << "can not createcore" << std::endl;
		return -1;
	}
	CRecoverCoreDelegate* delegate = new CRecoverCoreDelegate;
	recovercover_obj->SetDelegate(delegate);
	recovercover_obj->Init();

	std::string strConfigPath = "./config/debug.json";
	frjson confjson;

	std::ifstream json_in_file(strConfigPath);
	if (!json_in_file.is_open())
		return -1;
	json_in_file >> confjson;

	if (confjson.is_null())
		return -1;

	std::string scan_path = confjson.at("scan_path").get<std::string>();
	CScanner* scanner = new CScanner;
	if (scan_path == "")
	{
		CDeviceObj* device_obj = new CDeviceObj;
		device_obj->SetDelegate(delegate);
		device_obj->SetRecoverCore(recovercover_obj);
		if (device_obj->EnumeRateDevice() < 0)
		{
			std::cout << "can not analyse deivce info" << std::endl;
			return -1;
		}
		scanner->SetDeviceinfo(device_obj);
	}
	else 
	{
		FILE* file = fopen(scan_path.data(), "r");
		uint64_t file_size = 0;
		if (file)
		{
			fseek(file,0, SEEK_END);
			file_size = _ftelli64(file);
			fclose(file);
		}
		nlohmann::json device_infoObj;
		
		
		nlohmann::json infoObj;
		infoObj["DeviceCategory"] = 3;      //  镜像类型，必需是: DC_Image = 3
		infoObj["BytesPerSector"] = 512;    // 目前暂定512
		infoObj["Label"] = scan_path.substr( scan_path.find_last_of("/")+1);
		infoObj["Path"] = scan_path.substr(0, scan_path.find_last_of("/"));;    // 镜像全路径
		infoObj["Size"] = file_size;
		
		device_infoObj["device"] = infoObj;
		device_infoObj["type"] = 13;
		scanner->SetScanPath(device_infoObj.dump());

	}
		
		
	
	

	
	scanner->SetDelegate(delegate);
	scanner->SetRecoverCore(recovercover_obj);
	scanner->Explore();

		
	Documentation::GetInstance()->SaveToDisk();
	CRecoverObj* recover = new CRecoverObj();
	recover->SetCoreObj(recovercover_obj);
	recover->Recover();
	return 0;
}
