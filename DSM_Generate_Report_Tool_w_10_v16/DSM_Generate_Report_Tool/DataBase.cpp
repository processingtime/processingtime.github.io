#include "stdafx.h"
#include "DataBase.h"


map<string, string> CDataBase::m_sContents;

CDataBase::CDataBase()
{

}

CDataBase::CDataBase(char *pFilePath,const eDataType &eType)
{
	int nLenth = 0;
	memset(m_lpPath,0,MAX_PATH);
	if(eType==CONFIG)
	{
		if(!pFilePath)
		{
			memset(m_lpPath,0,MAX_PATH);
		}
		else
		{
			nLenth = strlen(pFilePath);
			if(nLenth>MAX_PATH-1)
				memset(m_lpPath,0,MAX_PATH);
			else
				strcpy(m_lpPath,pFilePath);
		}
	}
	
}

CDataBase::CDataBase(string sFileName,const eDataType &eType,string sDelimiter,string sComment)
	: m_sDelimiter(sDelimiter), m_sComment(sComment)
{  
	if(eType==EVALUATION)
	{
		m_sContents.clear();
		std::ifstream tagRead(sFileName.c_str());
		if (!tagRead)
		{
			tagDataBaseException(sFileName);
		}
		else
		{
			tagRead >> (*this);
		}
	}	
}  

CDataBase::~CDataBase()
{

}


void CDataBase::Trim(std::string& sStr)  
{  
	static const char sWhiteSpace[] = " \n\t\v\r\f";  
	sStr.erase(0,sStr.find_first_not_of(sWhiteSpace));  
	sStr.erase(sStr.find_last_not_of(sWhiteSpace)+1U);
}

void CDataBase::Remove(const string& sKeyIn)  
{ 
	m_sContents.erase(m_sContents.find(sKeyIn));
	return;
} 

int CDataBase::Save(std::string sFileName)
{
	std::ofstream tagOut(sFileName.c_str());
	tagOut << (*this);
	return SUCCESS;
}

std::ostream& operator<<(std::ostream& tagOs,const CDataBase& tagData)  
{  
	for(CDataBase::mapConstIter p = tagData.m_sContents.begin();
		p != tagData.m_sContents.end();  
		++p)  
	{  
		tagOs << p->first << " " << tagData.m_sDelimiter << " ";
		tagOs << p->second << std::endl;
	}  
	return tagOs;
} 

std::istream& operator>>(std::istream& tagIs,CDataBase& tagData)  
{  
	typedef string::size_type tagPos;
	const string& sDelim  = tagData.m_sDelimiter;  
	const string& sComm   = tagData.m_sComment; 
	const tagPos tagSkip = sDelim.length(); 

	string sNextLine = "";
	while(tagIs || sNextLine.length()>0)  
	{  
		string sLine;  
		if(sNextLine.length()>0)  
		{  
			sLine = sNextLine; 
			sNextLine = "";  
		}  
		else  
		{  
			std::getline(tagIs,sLine);  
		}  
		sLine = sLine.substr(0,sLine.find(sComm));  
		tagPos sDelimtagPos = sLine.find(sDelim);  
		if(sDelimtagPos<string::npos)
		{  
			string sKey = sLine.substr(0,sDelimtagPos);  
			sLine.replace(0,sDelimtagPos+tagSkip,""); 
			bool bTerminate = false;  
			while(!bTerminate && tagIs)  
			{  
				std::getline(tagIs,sNextLine);  
				bTerminate = true;  
				string sCopy = sNextLine;  
				CDataBase::Trim(sCopy);  
				if(sCopy == "") 
					continue;  
				sNextLine = sNextLine.substr(0,sNextLine.find(sComm));  
				if(sNextLine.find(sDelim)!=string::npos)  
					continue;  
				sCopy = sNextLine;  
				CDataBase::Trim(sCopy);  
				if(sCopy != "") 
					sLine += "\n";  
				sLine += sNextLine;  
				bTerminate = false;  
			}  
			CDataBase::Trim(sKey);  
			CDataBase::Trim(sLine);  
			tagData.m_sContents[sKey] = sLine;
		}  
	}  

	return tagIs;  
}

 