//---------------------------------------------------------------------------

#ifndef ModelsH
#define ModelsH

#include <set>
#include <mutex>
//---------------------------------------------------------------------------
template <typename T>
class Model{
private:
	bool IsOn=0;
	std::set<T> Data;
	std::mutex Mutex;
public:
Model(){}
//----------------------------
void SetIsOn(bool newIsOn){
std::lock_guard<std::mutex> lock(this->Mutex);
this->IsOn=newIsOn;
this->Data.clear();
}
//----------------------------
void AddData(T newData){
std::lock_guard<std::mutex> lock(this->Mutex);
if(this->IsOn){
	this->Data.insert(newData);
}
}
//----------------------------
void DeleteAllData(){
std::lock_guard<std::mutex> lock(this->Mutex);
if(this->IsOn){
	this->Data.clear();
}
}
//----------------------------
T FindAndGetOneData(T example){
std::lock_guard<std::mutex> lock(this->Mutex);
if(this->IsOn){
	auto it=this->Data.find(example);
	return it==this->Data.end() ? T() : *it;
}
return T();
}
//----------------------------
std::set<T> GetAllData(){
std::lock_guard<std::mutex> lock(this->Mutex);
if(this->IsOn){
	return this->Data;
}
return std::set<T>();
}
//----------------------------
std::set<T> GetFewOfLastData(int Amount){
std::lock_guard<std::mutex> lock(this->Mutex);
if(this->IsOn){
	if(Amount<=this->Data.size()){
		return this->Data;
	}else{
		auto it=this->Data.rbegin();
		std::advance(it,Amount);
		return std::set<T>(this->Data.rbegin(),it);
	}
}
return std::set<T>();
}
//----------------------------
int GetCountOfData(){
std::lock_guard<std::mutex> lock(this->Mutex);
return this->Data.size();
}
//----------------------------
void DeleteOne(T Other){
std::lock_guard<std::mutex> lock(this->Mutex);
if(this->IsOn){
	Data.erase(Other);
}
}
//----------------------------
};
//---------------------------------------------------------------------------
#endif
