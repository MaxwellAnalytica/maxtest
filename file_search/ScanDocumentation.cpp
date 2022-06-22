#include "ScanDocumentation.h"
#include <memory>

using namespace  std;
Documentation* Documentation::GetInstance()
{
	static shared_ptr<Documentation> item = make_shared<Documentation>();
	return item.get();
}

Documentation::Documentation()
{
	 file_name_ = "./config/debug.json";
	 json_in_file_ =nullptr;
	 json_out_file_ = nullptr;
	 file_whitelist_map_.clear();
}

bool Documentation::NeedRecordFile(const BaseInfo* bi)
{
	if (json_in_file_ ==nullptr ||confjson_.is_null())
	{
		set_json_path(file_name_);
	}
	std::string filename = (char*)bi->Name;
	transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
	auto itor = file_whitelist_map_.find(filename);
	return file_whitelist_map_.find(filename) != file_whitelist_map_.end();
}

void Documentation::RecordFileInfo(const BaseInfo* bi, const int64_t file_position_on_disk)
{
	if (json_out_file_ ==nullptr)
	{
		json_out_file_ = new fstream(file_outname_, fstream::out );
	}
	if (!json_out_file_->is_open())
		return;
	std::string file_name = (char*)bi->Name;
	nlohmann::json jsonObject;
	if(bi->Runlist!=nullptr)
		jsonObject["RecordOffset"] = bi->Runlist->Offset;
	jsonObject["AccessTime"] = bi->AccessTime;
	jsonObject["Attribute"] = bi->Attribute;
	jsonObject["Category"] = bi->Category;
	jsonObject["Did"] = bi->Did;
	jsonObject["FileSystem"] = bi->FileSystem;
	jsonObject["Flag"] = bi->Flag;
	jsonObject["Name"] = file_name;
	jsonObject["ScanType"] = bi->ScanType;
	jsonObject["Size"] = bi->Size;
	jsonObject["Flag"] = bi->Flag;

	
	auto item = bi->Runlist;
	int count = 0;
	nlohmann::json runlist_arry = nlohmann::json::array();
	while (item) 
	{
		
		nlohmann::json runlist;
		runlist["runlist_start"] = item->Start;
		runlist["runlist_Offset"] = item->Offset;
		runlist["runlist_Number"] = item->Number;
		item = item->Next;
		runlist_arry.push_back(runlist);

	}
	jsonObject["file_runlist_arry"] = runlist_arry;
	file_arry_.push_back(jsonObject);

}

void Documentation::SaveToDisk()
{
	if (!file_arry_.size())
		return;
	root_node_["file_arry"] = file_arry_;
	std::string content=root_node_.dump(4);
	*json_out_file_ << content << std::endl;
	json_out_file_->flush();
	json_out_file_->close();
}

void Documentation::set_json_path(const std::string& filename)
{
	file_name_ = file_name_;

	json_in_file_ =new fstream(filename);


	if (!json_in_file_->is_open())
		return;
	*json_in_file_ >> confjson_;

	if (confjson_.is_null())
		return;
	ReadWhiteList();
}

void Documentation::ReadWhiteList()
{
	auto scannerArray = confjson_.at("whitelist").get<nlohmann::json::array_t>();
	for (auto& scanner : scannerArray)
	{
		//file_whitelist_map_.insert(scanner);
		file_whitelist_map_.insert(scanner.get<std::string>());
	}
}
