#pragma once

//��Ŀ�����趨
#define PROJECT_NAME NEXT_GENERATION_SEQUENCING
#define VERSION "0.1.0"

//��������
#ifdef _WIN32
	#define debugger system("pause")
#else
	#define debugger cin.get()
#endif // _WIN32

//B+����ѡ����
/*B+���ļ�·��*/#define BPLUES_INIT_FILE_NAME "D:\\zhiju1.txt"



//ID��ϴ��ѡ����
#define ID_WASH_INIT_CHARACTERISTIC_FILE_NAME "D:\\task\\EM_res\\EM_res.txt"
#define ID_WASH_INIT_MATCH_FILE_NAME "D:\\task\\result30\\result30_example.txt"
#define ID_WASH_OUTPUT_MATCH_FILE_NAME "D:\\task\\result\\file1.txt"
#define ID_WASH_OUTPUT_SEQUENCE_FILE_NAME "D:\\task\\result\\file2.txt"