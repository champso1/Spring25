#include "Utils.hpp"
#include "Ntuple.hpp"
#include "Logger.hpp"

#include <fstream>



template <typename T>
void Ntuple<T>::addField(const std::string& name) {
	this->fields.emplace_back(name);
	this->num_fields++;
}

template <typename T>
void Ntuple<T>::addFields(const std::vector<std::string>& fields) {
	for (const std::string& field : fields) this->fields.emplace_back(field);
	this->num_fields += fields.size();
}


template <typename T>
void Ntuple<T>::fill(const std::vector<T>& entry) {
	this->data.emplace_back(entry);
}



template <typename T>
void Ntuple<T>::save(const std::string& file_name) {
	
}
