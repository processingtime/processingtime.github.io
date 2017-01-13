#pragma once
#include "stdafx.h"
#include <vector>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>  
#include <map>  
#include <iostream>  
#include <fstream>  
#include <sstream> 
using namespace std;

const int EXECUTE_SUCCESS = 0;
const int READ_LOGLIST_FAILED = 1;
const int OPEN_LOG_FAILED = 2;
const int READ_CONFIG_FAILED = 4;
const int CREATE_PEAK_REPORT_FAILED = 8;
const int CREATE_AVE_REPORT_FAILED = 16;
const int CSV_MAX_LINE_SIZE = 256;
const int CSV_IMAGE_MAX_NUM = 10000;

typedef enum LOG_LEVEL
{
	DISABLE_LOG = 1,
	LOG_LEVEL_INFO = 2,
	LOG_LEVEL_BUFFER = 3,
	LOG_LEVEL_TRACE = 4,
	LOG_LEVEL_DEBUG = 5,
	ENABLE_LOG = 6,
}LogLevel;

typedef enum LOG_TYPE
{
	NO_LOG = 1,
	CONSOLE = 2,
	FILE_LOG = 3,
}LogType;

typedef enum _ERROR_TYPE_
{
	SUCCESS,
	FAILURE,
	ERROR_POINTER,
} eErrorType;

typedef enum _DATA_TYPE_
{
	CONFIG,
	EVALUATION,
} eDataType;

typedef struct _LOG_INFO_
{
	char ver[100];                       
	char filename[50];                    
	float feature_extraction_ave;    
	float detection_stage1_ave;     
	float detection_stage2_ave;      
	float regression_ave;              
	float feature_extraction_peak;  
	float detection_stage1_peak;   
	float detection_stage2_peak;   
	float regression_peak;            
	int frameid_feature;                
	int frameid_stage1;                
	int frameid_stage2;                 
	int frameid_regression;          
} tagLogInfo;