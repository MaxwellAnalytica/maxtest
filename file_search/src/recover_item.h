#pragma once
#include "device_store.h"

class CRecoverObj
{
public:
	CRecoverObj();
	~CRecoverObj();
public:
	void SetDiviceInfo(CDeviceObj* deivce_info);
	void SetCoreObj(IRecoveryCore* core_obj);
	void Recover();

private:
	CDeviceObj* m_device_info;
	IRecoveryCore* m_core_obj;

};
