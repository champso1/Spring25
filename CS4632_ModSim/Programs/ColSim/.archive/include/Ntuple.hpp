#ifndef __NTUPLE_HPP
#define __NTUPLE_HPP


#include "Utils.hpp"
#include "Logger.hpp"

#include <string>
#include <vector>


template <typename T>
class Ntuple {
private:
	std::string name;
	std::string description;
	std::vector<std::string> fields;
	u32 num_fields;
	
	std::vector<std::vector<T>> data;

public:
	Ntuple(const std::string& name, const std::string& description)
		: name(name), description(description)
	{}
	
	Ntuple(const std::string& name, const std::string& description, const std::string& fields_str)
		: name(name), description(description)
	{
		std::vector<std::string> fields;
		Utils::SplitString(name, ":", fields);
		for (const std::string& field : fields) {
			GLOG_DEBUG("%s", field.c_str());
		}
	}
	
	Ntuple(const std::string& name, const std::string& description, const std::vector<std::string>& fields)
		: name(name), description(description), fields(fields)
	{}
	
	Ntuple(const std::string& file_name) {
		GLOG_INFO("NOT IMPLEMENTED");
	}
	
	~Ntuple() {}

	
	void addField(const std::string& name);
	void addFields(const std::vector<std::string>& names);

	void fill(const std::vector<T>& entry);

	void save(const std::string& file_name);
};




#endif // __NTUPLE_HPP
