#pragma once
#include "irecoverycore.h"
#include "iexplore.h"
#include "public.h"

#include <string>
#include <vector>
struct VolumeInfo
{
	std::string name_;
	std::string lable_;
	int64_t size_;
};

class CDeviceObj 
{
public:
	CDeviceObj() ;

	~CDeviceObj() ;


public:
	void SetRecoverCore(IRecoveryCore* recover_core);

	void SetDelegate(INotifyDelegate* delegate);

	int EnumeRateDevice();

	void PrintDeviceInfo();

	std::string GetDiveiceInfo(int device_index);

	std::string GetDiveiceName(int device_index);

private:
	void AnalyseDevice();

private:
	IRecoveryCore* recover_core_;
	INotifyDelegate *recover_delegate_;

private:
	std::string deviceInfo_;
	std::vector<VolumeInfo* >device_vct_;
	std::vector<std::string >device_obj_vct_;
};
