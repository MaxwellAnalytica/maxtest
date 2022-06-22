#include "notify_item.h"
#include "file_store.h"

typedef struct _WdProgress
{
	int32_t value;
	int64_t offset;
	int64_t total;
	_WdProgress() {
		value = 0;
		offset = 0;
		total = 0;
	}
}WdProgress;
int32_t CRecoverCoreDelegate::OnNotify(int32_t type, void* ptr /*= nullptr*/, int64_t* size /*= nullptr*/)
{
	
	if (ptr == nullptr)
		return 0;
		
	if (type == NotifyOption::NO_Progress)
	{
		WdProgress* progress = (WdProgress*)ptr;
		std::cout << "scan progress is " << progress->value << "%" << std::endl;
		return 0;
	}

	if (type != NotifyOption::NO_FileInfo)
	{
		return 0;
	}
	CDataManager::GetDataManagerObj()->PushFileInfo(ptr);
	return 0;
}
