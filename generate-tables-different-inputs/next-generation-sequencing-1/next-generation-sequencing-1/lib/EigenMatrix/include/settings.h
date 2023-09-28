#pragma once

#define debugger system("pause")

#define _PROJECT_NAME_ EIGENVALUE_MATRIX_GENERATION
#define _IN_FILE_NAME_ "./V2.1_1.fq"
#define _OUT_FILE_NAME_ "./src/EM_res.txt"

//IO
#define _LINE1_BUF_SIZE_ 20
#define _LINE2_BUF_SIZE_ 350
#define _LINE3_BUF_SIZE_ 300
#define _LINE4_BUF_SIZE_ _LINE2_BUF_SIZE_

//COMPUTING
#define _A_CHARACTERISTIC_ 16384
#define _T_CHARACTERISTIC_ 32768
#define _C_CHARACTERISTIC_ -16384
#define _G_CHARACTERISTIC_ -32768

#define _A_VALUE_ 1
#define _T_VALUE_ 2
#define _C_VALUE_ 4
#define _G_VALUE_ 8

#define _MULTIPLICATION_FACTOR_ 4681
#define _A_TO_T_ _A_VALUE_-_T_VALUE_

#define _POSITIVE_ORDER_ 1
#define _REVERSE_ORDER_ -1
