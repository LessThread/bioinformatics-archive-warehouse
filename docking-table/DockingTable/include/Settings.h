#pragma once

#define _PROJECT_NAME_ DOCKING_TABLE

/*默认设置*/

//源文件位置
//#define _SOURCE_FILE_NAME_ "../../test/file/source.txt"  
#define _SOURCE_FILE_NAME_ ".\\characteristic-matrix.txt"

//IO单行缓冲区
#define _IO_BUFF_SIZE_ 450

//默认特征矩阵元素个数
//缓冲区大小/30 即 15 个
#define _BP_COUNT_UNIT_ 30

//基础误差阈值
#define _ERROR_THRESHOLD_ 32767

//输出的默认文件名前缀
#define _OUTPUT_FILE_NAME_ "G:\\Test\\characteristic-matrix"
//#undef _OUTPUT_FILE_NAME_
//#define _OUTPUT_FILE_NAME_ "F:\\CodeWork\\ClionDNA\\DockingTableRelease\\x64\\Debug\\output\\result"

