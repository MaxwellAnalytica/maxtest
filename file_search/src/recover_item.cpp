#include "recover_item.h"
#include "file_store.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#define  STD_BUF_SIZE 2*1024*1024
CRecoverObj::CRecoverObj()
{
	m_device_info = nullptr;
}

CRecoverObj::~CRecoverObj()
{

}

void CRecoverObj::SetDiviceInfo(CDeviceObj* deivce_info)
{
	m_device_info = deivce_info;
}

void CRecoverObj::SetCoreObj(IRecoveryCore* core_obj)
{
	m_core_obj = core_obj;
}

void CRecoverObj::Recover()
{
	
	std::string save_path = "";
	int64_t g_BytesPerSector = 512;

	std::string strConfigPath = "./config/debug.json";
	frjson confjson;

	std::ifstream json_in_file(strConfigPath);
	if (!json_in_file.is_open())
		return;
	json_in_file >> confjson;

	if (confjson.is_null())
		return;

	save_path = confjson.at("save_path").get<std::string>();
	//
/*	std::cout << "input scan  device index";
	int device_index = 0;
	std::cin >> device_index;
	frjson scan_params_;
	scan_params_["type"] = 1;

	frjson deviceObject;
	std::string deviceInfo = m_device_info->GetDiveiceInfo(device_index);
	frjson deviceJson = frjson::parse(deviceInfo);
	g_BytesPerSector = deviceJson["BytesPerSector"];;
	divice_name = m_device_info->GetDiveiceName(device_index);
	if (deviceInfo.empty())
	{
		std::cout << "can not get deivce info,select again";
		continue;;
	}
	break;*/


	
	std::vector<void* > file_vct = CDataManager::GetDataManagerObj()->GetFileInfoVct();
	std::cout << "found file num is: " << file_vct.size() << " enter to recover" << std::endl;
	system("pause");
	char g_pPreviewBuffer[STD_BUF_SIZE] = {0};
	for (auto item : file_vct) 
	{
		const BaseInfo* bi = (BaseInfo*)item;
		std::string filename = save_path +(char*)bi->Name;
		FILE* fp;
		fp = fopen(filename.data(), "wb+");
		if(fp ==nullptr)
			continue;
		Runlist* rl = bi->Runlist;
		int32_t saveAsStatus = 0;
		int64_t fileOffset = 0;
		while (rl != nullptr)
		{
			if (bi->RunlistCategory == 2) // resident data
			{
				int64_t deviceOffset = g_BytesPerSector * rl->Start;
				int64_t bytesOffset = rl->Number;
				int32_t expectBytes = (bi->Size / g_BytesPerSector + 2) * g_BytesPerSector; 
				int64_t bytes = m_core_obj->ReadRawData(g_pPreviewBuffer, deviceOffset, bytesOffset, nullptr);
				if (bytes < 0)
					saveAsStatus = -1;
				fwrite(g_pPreviewBuffer + bytesOffset, bi->Size,1, fp);
				break;
			}

			int64_t deviceOffset = g_BytesPerSector * rl->Start;
			int64_t bulkSize = g_BytesPerSector * rl->Number;
			if (bulkSize - STD_BUF_SIZE > 0)
			{
				int64_t bulkOffset = 0;
				while (bulkOffset < bulkSize)
				{
					int32_t expectBytes = (bulkSize - bulkOffset) > STD_BUF_SIZE ? STD_BUF_SIZE : (bulkSize - bulkOffset);
					int64_t bytes = m_core_obj->ReadRawData(g_pPreviewBuffer, deviceOffset + bulkOffset, expectBytes);
					if (bytes < 0)
						saveAsStatus = -1;
					int32_t validBytes = (bi->Size - fileOffset) > expectBytes ? expectBytes : (bi->Size - fileOffset);
					fwrite(g_pPreviewBuffer,1, validBytes, fp);
					fflush(fp);
					bulkOffset += expectBytes;
					fileOffset += validBytes;
				}
			}
			else if (bulkSize > 0)
			{
				int32_t expectBytes = bulkSize;
				int64_t bytes = m_core_obj->ReadRawData(g_pPreviewBuffer, deviceOffset, expectBytes);
				if (bytes < 0)
					saveAsStatus = -1;
				int32_t validBytes = (bi->Size - fileOffset) > expectBytes ? expectBytes : (bi->Size - fileOffset);
				fwrite(g_pPreviewBuffer, 1, validBytes, fp);
				fflush(fp);
				fileOffset += validBytes;
			}
			rl = rl->Next;
		}
	
		fclose(fp);
	}

	std::cout << "Recover Finished,all count is " << file_vct.size()<< std::endl;
}
