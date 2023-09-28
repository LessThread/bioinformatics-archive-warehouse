#pragma once

//项目基本设定
#define PROJECT_NAME NEXT_GENERATION_SEQUENCING
#define VERSION "0.1.0"

//调试设置
#ifdef _WIN32
	#define debugger system("pause")
#else
	#define debugger cin.get()
#endif // _WIN32

//B+树可选设置
/*B+树文件路径*/#define BPLUES_INIT_FILE_NAME "D:\\zhiju1.txt"



//ID清洗可选设置
#define ID_WASH_INIT_CHARACTERISTIC_FILE_NAME "D:\\task\\EM_res\\EM_res.txt"
#define ID_WASH_INIT_MATCH_FILE_NAME "D:\\task\\result30\\result30_example.txt"
#define ID_WASH_OUTPUT_MATCH_FILE_NAME "D:\\task\\result\\file1.txt"
#define ID_WASH_OUTPUT_SEQUENCE_FILE_NAME "D:\\task\\result\\file2.txt"