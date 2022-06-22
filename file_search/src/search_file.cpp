#include "search_file.h"

CScanner::CScanner()
{
	device_info_string_ = "";
	m_device_info = nullptr;
	recover_core_ = nullptr;
	recover_delegate_ = nullptr;
}

CScanner::~CScanner()
{

}

void CScanner::SetRecoverCore(IRecoveryCore* recover_core)
{	
	recover_core_ = recover_core;
}

void CScanner::SetDelegate(INotifyDelegate* delegate)
{
	recover_delegate_ = delegate;
}

void CScanner::SetDeviceinfo(CDeviceObj* device_info)
{
	m_device_info = device_info;
}

void CScanner::SetScanPath(std::string dmg_path)
{
	device_info_string_ = dmg_path;
}

int CScanner::Explore()
{
	if (recover_core_ == nullptr || recover_delegate_ == nullptr )
	{
		std::cout << " recover core or delegate is null";
		return -1;
	}
	
	if (m_device_info) 
	{
		m_device_info->PrintDeviceInfo();
		while (true)
		{
			std::cout << "input scan  device index";
			int device_index = 0;
			std::cin >> device_index;
			frjson scan_params_;
			scan_params_["type"] = 1;

			frjson deviceObject;
			std::string deviceInfo = m_device_info->GetDiveiceInfo(device_index);
			if (deviceInfo.empty())
			{
				std::cout << "can not get deivce info,select again";
				continue;;
			}
			deviceObject = frjson::parse(deviceInfo);
			scan_params_["device"] = deviceObject;
			device_info_string_ = scan_params_.dump();
			break;
		}
	}
	

	return recover_core_->Scan(device_info_string_.data(), device_info_string_.length());

}
