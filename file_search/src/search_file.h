#pragma once
#pragma once
#include "irecoverycore.h"
#include "iexplore.h"
#include "public.h"
#include "device_store.h"

#include <string>
#include <vector>


class CScanner
{
public:
	CScanner();

	~CScanner();


public:
	void SetRecoverCore(IRecoveryCore* recover_core);

	void SetDelegate(INotifyDelegate* delegate);

	void SetDeviceinfo(CDeviceObj* device_info);

	void SetScanPath(std::string dmg_path);

	int Explore();


private:
	IRecoveryCore* recover_core_;
	INotifyDelegate* recover_delegate_;
	std::string device_info_string_;
	CDeviceObj* m_device_info;

};
