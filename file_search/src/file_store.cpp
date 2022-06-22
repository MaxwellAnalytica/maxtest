#include "file_store.h"
#include "datatype.h"
#include "../ScanDocumentation.h"
CDataManager* CDataManager::m_data_manager = nullptr;
CDataManager* CDataManager::GetDataManagerObj()
{
	if (m_data_manager == nullptr)
		m_data_manager = new CDataManager();
	return m_data_manager;
}

CDataManager::CDataManager()
{

}

CDataManager::~CDataManager()
{
	delete m_data_manager;
	m_data_manager = nullptr;
}

void CDataManager::PushFileInfo(void* file_handle)
{
	
	const BaseInfo* bi = (BaseInfo*)file_handle;
	if (bi->Category != 1)
		return;
	if (Documentation::GetInstance()->NeedRecordFile(bi))
		Documentation::GetInstance()->RecordFileInfo(bi, 0);
	if(m_file_info_vct.size()%1000 ==0)
		std::cout << "file_count is" << m_file_info_vct.size()<<std::endl;

	 m_file_info_vct .push_back(file_handle);
}

std::vector<void* > CDataManager::GetFileInfoVct()
{
	return m_file_info_vct;
}
