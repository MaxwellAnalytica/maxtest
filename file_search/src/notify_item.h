#pragma once
#include "irecoverycore.h"
#include "datatype.h"

class CRecoverCoreDelegate :public INotifyDelegate
{
public:
	virtual int32_t OnNotify(int32_t type, void* ptr = nullptr, int64_t* size = nullptr) override;
private:

};

