#pragma once
#include "./settings.h"
#include <string>
#include <vector>
#include <algorithm>

#undef _MOD_NAME_
#define _MOD_NAME_ SEQUENCE_UNIT

#define PAIR std::pair<int,float>

namespace _PROJECT_NAME_ 
{
    namespace _MOD_NAME_
    {
        class SequenceUnit
        {
        private:
        public:
            std::string* sequence;//碱基主序列
            
            //头尾变化率特征值
            std::pair<int,float>*positive_30_rate_array_container = new std::pair<int,float>;
            std::pair<int,float>*positive_60_rate_array_container = new std::pair<int,float>;
            std::pair<int,float>*positive_90_rate_array_container = new std::pair<int,float>;

            std::pair<int,float>*reverse_30_rate_array_container = new std::pair<int,float>;
            std::pair<int,float>*reverse_60_rate_array_container = new std::pair<int,float>;
            std::pair<int,float>*reverse_90_rate_array_container = new std::pair<int,float>;

            //全碱基特征值（此段可直接调用lib函数，不一定要重写，留存重写备用）
            std::vector<float> positive_value_container;
            std::vector<float> reverse_value_container;

            std::string* temp_positive_str;
            std::string* temp_reverse_str;

            void generationChangeRate(int order,int number);//生成变化率
            void generateCharacteristicValue(int order);//生成特征值
            SequenceUnit();
            SequenceUnit(std::string* content);//常规构造函数
            ~SequenceUnit();

        };
    }
}

