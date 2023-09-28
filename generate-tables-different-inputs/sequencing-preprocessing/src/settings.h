#pragma once

//以下为基本设定
#define INPUT_FILE_PATH  "../../V300020106_L03_read_2.fq" //输入文件名
#define OUTPUT_QN_FILE_PATH  "../output4/preprocess-QN-output" //输出文件名
#define OUTPUT_CQ_FILE_PATH  "../output4/preprocess-CQ-output-"


//以下为项目环境基本设定
#define PROJECT_NAME SEQ_PRE
#define DLL_EXPORT __declspec(dllexport)

#define READ_BUFFER_SZIE 200


//以下为代码调试设置和工程设置
#define TASK_START 10000001
#define TASK_END 20000000

#define DEBUG_MODE 0
#define DEBUG if(DEBUG_MODE)
#define TEST_NUM 10000
#ifdef __WIN32__
    #define debugger if(DEBUG_MODE)system("pause")
#elif __linux__
    #define debugger if(DEBUG_MODE){puts("\n*****SYSTEM PAUSE*****");getchar();}
#endif
#define LONG long long

//以下为公共库
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <mutex>
#include <iostream>
