//---------------------------------------------------------------------------
#pragma hdrstop

#include "Messages.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
const int Message::MAXTextLength=150;
unsigned int MessageSe::Count=0;
//---------------------------------------------------------------------------
Message::Message(){
//this->Time=std::chrono::high_resolution_clock::now();
this->Time=std::time(NULL);
}
//---------------------------------------------------------------------------
Message::Message(std::string newText, std::string newSenderName):Message(){
this->SenderName=newSenderName;
this->SetText(newText);
}
//---------------------------------------------------------------------------
Message::Message(std::string newText, std::string newSenderName, unsigned int newID):Message(newText,newSenderName){
this->ID=newID;
}
//---------------------------------------------------------------------------
Message::Message(std::string newText, std::string newSenderName, unsigned int newID,std::time_t newTime){
this->Time=newTime;
}
//---------------------------------------------------------------------------
void Message::SetText(std::string newText){
if(newText.size() > 0){
	newText.erase(std::remove_if(newText.begin(), newText.end(), Message::CheckSingleLetter), newText.end());
	if(newText.size() > 0){
		if(newText.size() > Message::MAXTextLength){
			newText=newText.substr(0,MAXTextLength);
		}
		this->Text=newText;
	}
}
}
//---------------------------------------------------------------------------
std::string Message::GetText()const{
return this->Text;
}
//---------------------------------------------------------------------------
std::time_t Message::GetTime()const{
return this->Time;
}
//---------------------------------------------------------------------------
std::string Message::GetSenderName()const{
return this->SenderName;
}
//---------------------------------------------------------------------------
int Message::GetID()const{
return this->ID;
}
//---------------------------------------------------------------------------
bool Message::CheckSingleLetter(unsigned short Key){
return !( ((Key >= 32) && (Key <=126)) && ((Key != 91) && (Key !=93)) );
}
//---------------------------------------------------------------------------
std::string Message::GetParsedSubString(const std::string &Str,std::string Key){
std::size_t pos1,pos2;
pos1=Str.find(Key+"[");
if(pos1!=std::string::npos){
	pos1+=2;
	pos2=Str.find("]",pos1);
	if(pos2!=std::string::npos){
		return Str.substr(pos1,pos2-pos1);
	}
}
return "";
}
//---------------------------------------------------------------------------
bool Message::operator<(const Message& Other)const{
if(this->Time == Other.Time){
	return this->ID < Other.ID;
}
return this->Time < Other.Time;
}
//---------------------------------------------------------------------------
bool Message::operator==(const Message& Other)const{
return (this->Time == Other.Time) && (this->Text == Other.Text) && (this->SenderName == Other.SenderName);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
MessageSe::MessageSe():MessageSe("","@Anonym"){
}
//---------------------------------------------------------------------------
MessageSe::MessageSe(std::string newText, std::string newSenderName):Message(newText,newSenderName){
MessageSe::Count++;
this->ID=MessageSe::Count;
}
//---------------------------------------------------------------------------
