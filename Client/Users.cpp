#include "Users.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
const int User::MAXNameLength=15;
//---------------------------------------------------------------------------
User::User(std::string newIP, std::string newName):User(newIP){
this->SetName(newName);
}
//---------------------------------------------------------------------------
User::User(std::string newIP){
this->SetIP(newIP);
}
//---------------------------------------------------------------------------
void User::SetIP(std::string newIP){
if(newIP.size() > 0){
	this->IP=newIP;
}
}
//---------------------------------------------------------------------------
void User::SetName(std::string newName){
newName=User::CleanNameString(newName);
if(newName.size() > 0){
	this->Name=newName;
}
}
//---------------------------------------------------------------------------
std::string User::CleanNameString(std::string newName){
if(newName.size() > 0){
	newName.erase(std::remove_if(newName.begin(), newName.end(),[](unsigned short Key)->bool{
		return !( ((Key >= 32) && (Key <=126)) && ((Key != 91) && (Key !=93) && (Key !=64)) );
	}), newName.end());
	if(newName.size() > 0){
		if(newName.size() > User::MAXNameLength){
			newName=newName.substr(0,User::MAXNameLength);
		}
		return newName;
	}
}
return "";
}
//---------------------------------------------------------------------------
std::string User::GetIP()const{
return this->IP;
}
//---------------------------------------------------------------------------
std::string User::GetName()const{
return this->Name;
}
//---------------------------------------------------------------------------
bool User::operator<(const User& Other)const{
return this->IP < Other.IP;
}
//---------------------------------------------------------------------------
bool User::operator==(const User& Other)const{
return this->IP == Other.IP;
}
//---------------------------------------------------------------------------
