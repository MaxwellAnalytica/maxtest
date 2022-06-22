#pragma once
#include <iostream>
#include <vector>
#include "datatype.h"

class CDataManager
{
public:
	static CDataManager* GetDataManagerObj();

	CDataManager();

	~CDataManager();

public:
	void PushFileInfo(void* file_handle);

	std::vector<void* > GetFileInfoVct();

private:
	static CDataManager * m_data_manager;
	std::vector<void* > m_file_info_vct;
};
