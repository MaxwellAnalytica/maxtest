#pragma once
#include <iostream>
#include <set>
#include <fstream>
#include "json.hpp"
#include "datatype.h"
class Documentation
{
public:
	static Documentation* GetInstance();
	Documentation() ;
	~Documentation()=default;

	bool NeedRecordFile(const BaseInfo* bi);
	void RecordFileInfo(const BaseInfo* bi,const int64_t file_position_on_disk);
	void SaveToDisk();
public:
	void set_json_path(const std::string &filename);

private:
	void ReadWhiteList();
private:
	std::string file_name_ = "./config/debug.json";
	std::string file_outname_ = "./config/recoverfailed.json";
	std::fstream *json_in_file_;
	std::fstream* json_out_file_;
	nlohmann::json root_node_;
	nlohmann::json file_arry_ = nlohmann::json::array();
	nlohmann::json confjson_;
	std::set<std::string > file_whitelist_map_;
};
