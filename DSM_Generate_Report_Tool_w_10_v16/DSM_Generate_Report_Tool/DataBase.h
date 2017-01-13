#pragma once

#include "Types.h"

extern int readConfigRes;

class CDataBase
{
public:
	CDataBase();
	~CDataBase();
						
private:
	char m_lpPath[MAX_PATH];

public:
	CDataBase(char *pFilePath,const eDataType &eType);

protected:  
	std::string m_sDelimiter;
	std::string m_sComment;
	typedef std::map<std::string, std::string>::iterator mapIter;
	typedef std::map<std::string, std::string>::const_iterator mapConstIter;

public:
	static std::map<std::string, std::string> m_sContents;

protected:
	template<class T> static std::string T2String(const T& t);  
	template<class T> static T String2T(const std::string& s);

public:
	static void Trim(std::string& sStr); 

public:
	CDataBase(std::string sFileName,const eDataType &eType,std::string sDelimiter = "=",std::string sComment = "#"); 
	 
public:
	template<class T> T Read(const std::string& sKeyIn) const; 
	template<class T> T Read(const std::string& sKeyIn,const T& sValueIn) const; 
	template<class T> void Add(const std::string& sKeyIn,const T& sValueIn);
	void Remove(const std::string& sKeyIn);
	int Save(std::string sFileName);
	std::string GetDelimiter() const { return m_sDelimiter; }  
	std::string GetComment() const { return m_sComment; }  
	std::string SetDelimiter( const std::string& sDelimiter )  
	{ std::string sOld = m_sDelimiter;  m_sDelimiter = sDelimiter;  return sOld; }    
	std::string SetComment( const std::string& sComment )  
	{ std::string sOld = m_sComment;  m_sComment =  sComment;  return sOld; }  

public:
	friend std::ostream& operator<<(std::ostream& tagOs,const CDataBase& tagData);  
	friend std::istream& operator>>(std::istream& tagIs,CDataBase& tagData);  

public:  
	struct tagDataBaseException
	{  
		std::string sFileName;  
		tagDataBaseException(const std::string& sFileName_ = std::string())  
			: sFileName(sFileName_) 
		{
			readConfigRes = FAILURE;
		} 
	};  

};

template<class T>  
std::string CDataBase::T2String(const T& t)
{  
	std::ostringstream tagOs;  
	tagOs << t;  
	return tagOs.str();
}  

template<class T>  
T CDataBase::String2T(const std::string& s)  
{  
	T t;  
	std::istringstream tagIs(s);  
	tagIs >> t;  
	return t; 
}  

template<>  
inline std::string CDataBase::String2T<std::string>(const std::string& s)  
{  
	return s;  
}  

template<>  
inline bool CDataBase::String2T<bool>(const std::string& s)  
{   
	bool b = true;  
	std::string sup = s;  
	for(std::string::iterator p = sup.begin(); p != sup.end(); ++p)  
		*p = toupper(*p);  
	if(sup==std::string("FALSE") || sup==std::string("F") ||  
		sup==std::string("NO") || sup==std::string("N") ||  
		sup==std::string("0") || sup==std::string("NONE"))  
		b = false;  
	return b;  
}  

template<class T>  
T CDataBase::Read(const std::string& sKeyIn) const  
{  
	mapConstIter p = m_sContents.find(sKeyIn);  
	if(p == m_sContents.end()) 
		return 0;  
	return String2T<T>(p->second);  
} 

template<class T>  
T CDataBase::Read(const std::string& sKeyIn,const T& sValueIn) const  
{ 
	mapConstIter p = m_sContents.find(sKeyIn);  
	if(p == m_sContents.end()) 
		return sValueIn;  
	return String2T<T>(p->second);  
} 

template<class T>  
void CDataBase::Add(const std::string& sKeyIn,const T& sValueIn)  
{  
	std::string sValue = T2String(sValueIn);  
	std::string sKey = sKeyIn;  
	Trim(sKey);  
	Trim(sValue);  
	m_sContents[sKey] = sValue;
	return;  
}

