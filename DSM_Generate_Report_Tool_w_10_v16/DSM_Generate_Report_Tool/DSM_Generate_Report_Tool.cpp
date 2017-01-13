
#include "stdafx.h"

using namespace CPlusPlusLogging;

int value = 0;
int readConfigRes = SUCCESS;

int getFileList(const string & sFilePath, vector<string>& vecFilePath)
{
	int nPos = 0;
	string sDir = "";
	string sDirWith = "";
	string sExe = "";
	string sFileName = "";
	HANDLE hFileHandle = NULL;
	WIN32_FIND_DATA tagFind;

	vecFilePath.clear();
	if (_access(sFilePath.data(), 0) == -1)
		return FAILURE;
	nPos = sFilePath.find_last_of('\\');
	sDir = sFilePath.substr(0, nPos);
	nPos = sFilePath.find_last_of('.');
	sExe = sFilePath.substr(nPos + 1);
	sDirWith = sDir + "\\*." + sExe;

	hFileHandle = FindFirstFile(sDirWith.c_str(), &tagFind);
	if (!hFileHandle)
		return FAILURE;
	do
	{
		sFileName = sDir + "\\" + tagFind.cFileName;
		vecFilePath.push_back(sFileName);
	} while (FindNextFile(hFileHandle, &tagFind));
	FindClose(hFileHandle);
	hFileHandle = NULL;

	return SUCCESS;
}


int GetLineCount(FILE* pFile, int &nCount)
{
	int i = 0;
	static char pLine[CSV_MAX_LINE_SIZE] = { 0 };

	nCount = 0;
	if (!pFile)
		return FAILURE;
	fseek(pFile, 0, SEEK_SET);
	int nFlag = 0;
	do {
		nFlag = fgetc(pFile);
		if (nFlag == '\n')
			i++;
	} while (nFlag != EOF);
	fseek(pFile, 0, SEEK_SET);

	nCount = i;
	return SUCCESS;
}


tagLogInfo CalcLogInfo(const string &LogPath, BOOL &bFailure)
{
	FILE* pFile = NULL;
	int nCount = 0;
	int nImageNum = 0;
	int nCol = 0;
	int nDataFrameID = 0;
	int feature_extraction_peak_id = 0;
	int detection_stage1_peak_id = 0;
	int detection_stage2_peak_id = 0;
	int regression_peak_id = 0;
	float fData[4] = { 0.0 };
	float feature_extraction_sum = 0.0;
	float detection_stage1_sum = 0.0;
	float detection_stage2_sum = 0.0;
	float regression_sum = 0.0;
	float feature_extraction_peak = 0.0;
	float detection_stage1_peak = 0.0;
	float detection_stage2_peak = 0.0;
	float regression_peak = 0.0;

	static char pLine[CSV_MAX_LINE_SIZE] = { 0 };
	tagLogInfo stLogInfo;
	memset(&stLogInfo, 0, sizeof(tagLogInfo));

	if (_access(LogPath.data(), 0) == -1)
	{
		bFailure = TRUE;
		return stLogInfo;
	}

	if (fopen_s(&pFile, LogPath.data(), "r") != 0)
	{
		bFailure = TRUE;
		return stLogInfo;
	}

	GetLineCount(pFile, nCount);
	nImageNum = nCount - 1;
	fgets(pLine, CSV_MAX_LINE_SIZE, pFile);
	for (int i = 0; i < nImageNum && i < CSV_IMAGE_MAX_NUM; i++)
	{
		if (fgets(pLine, CSV_MAX_LINE_SIZE, pFile))
		{
			nCol = sscanf_s(pLine, "%d,%f,%f,%f,%f", &nDataFrameID, &fData[0], &fData[1], &fData[2],&fData[3]);

			if (fData[0] > feature_extraction_peak)
			{
				feature_extraction_peak = fData[0];
				feature_extraction_peak_id = i;
			}
			if (fData[1] > detection_stage1_peak)
			{
				detection_stage1_peak = fData[1];
				detection_stage1_peak_id = i;
			}
			if (fData[2] > detection_stage2_peak)
			{
				detection_stage2_peak = fData[2];
				detection_stage2_peak_id = i;
			}
			if (fData[3] > regression_peak)
			{
				regression_peak = fData[3];
				regression_peak_id = i;
			}
			feature_extraction_sum += fData[0];
			detection_stage1_sum += fData[1];
			detection_stage2_sum += fData[2];
			regression_sum += fData[3];
		}
	}
	
	int nPos = 0;
	string strFileName = "";
	string strVerName = "";
	nPos = LogPath.find_last_of('\\');
	strFileName = LogPath.substr(nPos + 1);
	nPos = strFileName.find_last_of('.');
	strFileName = strFileName.substr(0, nPos);
	nPos = strFileName.find_last_of('_');
	strVerName = strFileName.substr(nPos + 1);

	strcpy(stLogInfo.filename, strFileName.c_str());
	strcpy(stLogInfo.ver, strVerName.c_str());
	stLogInfo.feature_extraction_ave = feature_extraction_sum / nImageNum;
	stLogInfo.detection_stage1_ave = detection_stage1_sum / nImageNum;
	stLogInfo.detection_stage2_ave = detection_stage2_sum / nImageNum;
	stLogInfo.regression_ave = regression_sum / nImageNum;
	stLogInfo.feature_extraction_peak = feature_extraction_peak;
	stLogInfo.detection_stage1_peak = detection_stage1_peak;
	stLogInfo.detection_stage2_peak = detection_stage2_peak;
	stLogInfo.regression_peak = regression_peak;
	stLogInfo.frameid_feature = feature_extraction_peak_id;
	stLogInfo.frameid_stage1 = detection_stage1_peak_id;
	stLogInfo.frameid_stage2 = detection_stage2_peak_id;
	stLogInfo.frameid_regression = regression_peak_id;

	return stLogInfo;
}

vector<tagLogInfo> handleLogInfo(vector<string>& vecFilePath, vector<string>& vecLogVer)
{
	vector<tagLogInfo> vecLogInfo;
	vecLogInfo.clear();

	tagLogInfo stLogInfo;
	memset(&stLogInfo, 0, sizeof(tagLogInfo));
	string LogPath = "";
	BOOL bFailure = FALSE;
	BOOL bIsOpenLogFailed = FALSE;
	for (vector<string>::iterator iterLogVer = vecLogVer.begin(); iterLogVer != vecLogVer.end(); iterLogVer++)
	{
		for (vector<string>::iterator iterFilePath = vecFilePath.begin(); iterFilePath != vecFilePath.end(); iterFilePath++)
		{
			LogPath = *iterFilePath;

			int nPosTmp = 0;
			string strFileNameTmp = "";
			string strVerNameTmp = "";
			nPosTmp = LogPath.find_last_of('\\');
			strFileNameTmp = LogPath.substr(nPosTmp + 1);
			nPosTmp = strFileNameTmp.find_last_of('.');
			strFileNameTmp = strFileNameTmp.substr(0, nPosTmp);
			nPosTmp = strFileNameTmp.find_last_of('_');
			strVerNameTmp = strFileNameTmp.substr(nPosTmp + 1);

			if (strVerNameTmp == *iterLogVer)
			{
				stLogInfo = CalcLogInfo(LogPath, bFailure);
				if (bFailure)
				{
					bIsOpenLogFailed = TRUE;
					bFailure = FALSE;
					string strError = "";
					strError = LogPath + "  Read Failed!";
					LOG_ERROR(strError.data());
					continue;
				}
				vecLogInfo.push_back(stLogInfo);
			}
		}
	}

	if (bIsOpenLogFailed)
	{
		value += OPEN_LOG_FAILED;
	}
	return vecLogInfo;
}


vector<string> analysisLogVer(string &sLogVer)
{
	vector<string> vecLogVer;
	vecLogVer.clear();
	int nPos = 0;
	string sVer = "";
	
	while (nPos != -1)
	{
		nPos = sLogVer.find_first_of('_');
		sVer = sLogVer.substr(0, nPos);
		vecLogVer.push_back(sVer);
		sLogVer = sLogVer.substr(nPos + 1);
	}
	
	return vecLogVer;
}


int createPeakReport(vector<tagLogInfo>& vecLogInfo, vector<string>& vecLogVer)
{
	FILE* pFile = NULL;
	time_t lRawTime = 0;
	time(&lRawTime);
	struct tm* tagTimeInfo;
	tagTimeInfo = localtime(&lRawTime);
	char LogFileName[64];
	sprintf(LogFileName, "processing_time_report_peak_%d%d%d.csv", tagTimeInfo->tm_year + 1900, tagTimeInfo->tm_mon + 1, tagTimeInfo->tm_mday);
	char FilePath[128] = "Output\\";
	strcat(FilePath, LogFileName);
	pFile = fopen(FilePath, "w+t");

	if (pFile)
	{
		fprintf(pFile, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
			"Ver./Sprint",
			"filename", "frameno", "feature_extraction_peak",
			"filename", "frameno", "detection_stage1_peak", 
			"filename", "frameno", "detection_stage2_peak", 
			"filename", "frameno", "regression_peak");
		for (vector<string>::iterator iterVer = vecLogVer.begin(); iterVer != vecLogVer.end(); iterVer++)
		{
			string strVer = "";
			string strFileName_feature = "";
			string strFileName_stage1 = "";
			string strFileName_stage2 = "";
			string strFileName_regression = "";
			int iframeno_feature = 0;
			int iframeno_stage1 = 0;
			int iframeno_stage2 = 0;
			int iframeno_regression = 0;
			float feature_peak = 0.0;
			float stage1_peak = 0.0;
			float stage2_peak = 0.0;
			float regression_peak = 0.0;
			for (vector<tagLogInfo>::iterator iterLogInfo = vecLogInfo.begin(); iterLogInfo != vecLogInfo.end(); iterLogInfo++)
			{
				if (iterLogInfo->ver == *iterVer)
				{
					strVer = *iterVer;

					if (iterLogInfo->feature_extraction_peak > feature_peak)
					{
						strFileName_feature = iterLogInfo->filename;
						iframeno_feature = iterLogInfo->frameid_feature;
						feature_peak = iterLogInfo->feature_extraction_peak;
					}
					if (iterLogInfo->detection_stage1_peak > stage1_peak)
					{
						strFileName_stage1 = iterLogInfo->filename;
						iframeno_stage1 = iterLogInfo->frameid_stage1;
						stage1_peak = iterLogInfo->detection_stage1_peak;
					}
					if (iterLogInfo->detection_stage2_peak > stage2_peak)
					{
						strFileName_stage2 = iterLogInfo->filename;
						iframeno_stage2 = iterLogInfo->frameid_stage2;
						stage2_peak = iterLogInfo->detection_stage2_peak;
					}
					if (iterLogInfo->regression_peak > regression_peak)
					{
						strFileName_regression = iterLogInfo->filename;
						iframeno_regression = iterLogInfo->frameid_regression;
						regression_peak = iterLogInfo->regression_peak;
					}
				}
			}
			fprintf(pFile, "%s,%s,%d,%f,%s,%d,%f,%s,%d,%f,%s,%d,%f\n",
				strVer.data(),
				strFileName_feature.data(), iframeno_feature, feature_peak,
				strFileName_stage1.data(), iframeno_stage1, stage1_peak,
				strFileName_stage2.data(), iframeno_stage2, stage2_peak,
				strFileName_regression.data(), iframeno_regression, regression_peak);
		}
	}
	else
	{
		fclose(pFile);
		pFile = NULL;
		return FAILURE;
	}

	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	return SUCCESS;
}


int createAveReport(vector<tagLogInfo>& vecLogInfo, vector<string>& vecLogVer)
{
	FILE* pFile = NULL;
	time_t lRawTime = 0;
	time(&lRawTime);
	struct tm* tagTimeInfo;
	tagTimeInfo = localtime(&lRawTime);
	char LogFileName[64];
	sprintf(LogFileName, "processing_time_report_average_%d%d%d.csv", tagTimeInfo->tm_year + 1900, tagTimeInfo->tm_mon + 1, tagTimeInfo->tm_mday);
	char FilePath[128] = "Output\\";
	strcat(FilePath, LogFileName);
	pFile = fopen(FilePath, "w+t");

	if (pFile)
	{
		fprintf(pFile, "%s,%s,%s,%s,%s\n", "Ver./Sprint","feature_extraction","detection_stage1","detection_stage2","regression");

		for (vector<string>::iterator iterVer = vecLogVer.begin(); iterVer != vecLogVer.end(); iterVer++)
		{
			string strVer = "";
			int iLogNum = 0;
			float feature_sum = 0.0;
			float stage1_sum = 0.0;
			float stage2_sum = 0.0;
			float regression_sum = 0.0;
			float feature_ave = 0.0;
			float stage1_ave = 0.0;
			float stage2_ave = 0.0;
			float regression_ave = 0.0;
			for (vector<tagLogInfo>::iterator iterLogInfo = vecLogInfo.begin(); iterLogInfo != vecLogInfo.end(); iterLogInfo++)
			{
				if (iterLogInfo->ver == *iterVer)
				{
					iLogNum++;
					strVer = *iterVer;
					feature_sum += iterLogInfo->feature_extraction_ave;
					stage1_sum += iterLogInfo->detection_stage1_ave;
					stage2_sum += iterLogInfo->detection_stage2_ave;
					regression_sum += iterLogInfo->regression_ave;
				}
			}
			feature_ave = feature_sum / iLogNum;
			stage1_ave = stage1_sum / iLogNum;
			stage2_ave = stage2_sum / iLogNum;
			regression_ave = regression_sum / iLogNum;
			fprintf(pFile, "%s,%f,%f,%f,%f\n", strVer.data(), feature_ave, stage1_ave, stage2_ave, regression_ave);
		}
	}
	else
	{
		fclose(pFile);
		pFile = NULL;
		return FAILURE;
	}

	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	return SUCCESS;
}


int main()
{
	const string strConfigPath = "Config.ini";
	string strFilePath = "Input\\processing_timing_1_Ver1.xlsx";
	string strLogVer = "Ver1";
	vector<string> vecFilePath;
	vector<string> vecLogVer;
	vector<tagLogInfo> vecLogInfo;

	CDataBase tagReadData(strConfigPath, EVALUATION, "=", "#");

	if (readConfigRes == FAILURE)
	{
		value += READ_CONFIG_FAILED;
		LOG_ERROR("Read Config File Failed!");
		return value;
	}

	strFilePath = tagReadData.Read<string>("LogPath");
	strLogVer = tagReadData.Read<string>("LogVer");
	
	vecLogVer = analysisLogVer(strLogVer);

	int res_GetList = getFileList(strFilePath, vecFilePath);
	if (res_GetList != 0)
	{
		value += READ_LOGLIST_FAILED;
		LOG_ERROR("Read File List Failed!");
		return value;
	}

	vecLogInfo = handleLogInfo(vecFilePath, vecLogVer);

	CreateDirectory("./Output", NULL);

	int res_CreatePeakReport = createPeakReport(vecLogInfo, vecLogVer);
	if (res_CreatePeakReport == FAILURE)
	{
		LOG_ERROR("Create Peak Report Failed!");
		value += CREATE_PEAK_REPORT_FAILED;
	}

	int res_CreateAveReport = createAveReport(vecLogInfo, vecLogVer);
	if (res_CreateAveReport == FAILURE)
	{
		LOG_ERROR("Create Average Report Failed!");
		value += CREATE_AVE_REPORT_FAILED;
	}

	return value;
}

