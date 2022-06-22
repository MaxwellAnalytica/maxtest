#include "device_store.h"

CDeviceObj::CDeviceObj()
{
	recover_core_ = nullptr;
	recover_delegate_ = nullptr;
	deviceInfo_ = "";
}

CDeviceObj::~CDeviceObj()
{

}

void CDeviceObj::SetRecoverCore(IRecoveryCore* recover_core)
{
	recover_core_ = recover_core;
}

void CDeviceObj::SetDelegate(INotifyDelegate* delegate)
{
	recover_delegate_ = delegate;
}

int CDeviceObj::EnumeRateDevice()
{
	if (recover_core_ == nullptr)
		return -1;
	
	char szBuf[16 * 1024] = { 0x00 };
	int32_t ret = recover_core_->Explore(szBuf);
	if (ret <= 0)
		return -1;

	deviceInfo_.assign(szBuf, ret);
	AnalyseDevice();
	return 0;
}

void CDeviceObj::PrintDeviceInfo()
{
	std::cout << "===============Devices Detail=====================" << std ::endl;
	for (int i = 0; i < device_vct_.size(); i++) 
	{
		std::cout << "index :"<<i << "  name:" << device_vct_.at(i)->name_ << " lable:" << device_vct_.at(i)->lable_ << " size: " << device_vct_.at(i)->size_ << std::endl;
	}
}

std::string CDeviceObj::GetDiveiceInfo(int device_index)
{
	if (device_index >= device_obj_vct_.size())
		return "";
	return device_obj_vct_.at(device_index);
}

std::string CDeviceObj::GetDiveiceName(int device_index)
{
	if (device_index >= device_vct_.size())
		return "";
	return device_vct_.at(device_index)->name_;
}

void CDeviceObj::AnalyseDevice()
{
	
	try
	{
		frjson deviceJson = frjson::parse(deviceInfo_);
		if (deviceJson.is_null())
		{
			std::cout << "bad json string" << std::endl;;
			return ;
		}
		auto volumeCount = deviceJson["VolumeCount"];
		if (volumeCount.is_number())
		{
			const int32_t count = volumeCount.get<int32_t>();
			if (count <= 0)
			{
				return ;
			}
		
			int32_t c = 0;
			int32_t dc = 0;
			auto devicesObject = deviceJson["DeviceArray"];
			if (devicesObject.is_array())
			{
				auto diskArray = devicesObject.get<frjson::array_t>();
				for (auto disk : diskArray)
				{
					if (disk.is_object())
					{
						auto volumeObject = disk["VolumeArray"];
						if (volumeObject.is_array())
						{
							auto volumeArray = volumeObject.get<frjson::array_t>();
							for (auto volume : volumeArray)
							{
								if (volume.is_object())
								{
									
									bool logical = volume.at("Logical").get<bool>();
									VolumeInfo* volumeinfo = new VolumeInfo;
									volumeinfo->name_ = volume.at("Name").get<std::string>();
									volumeinfo->lable_ = volume.at("Label").get<std::string>();
									volumeinfo->size_ = volume.at("Size").get<int64_t>();
									device_obj_vct_.push_back(volume.dump());
									device_vct_.push_back(volumeinfo);
								}
							}
						}
					}
				}
			}
		}
	}
	catch(...)
	{
	
	}
}
