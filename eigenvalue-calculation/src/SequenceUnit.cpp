#include "../include/SequenceUnit.h"

namespace _PROJECT_NAME_ 
{
    namespace _MOD_NAME_
    {
        SequenceUnit::SequenceUnit()//空构造函数
        {
            
        }

        SequenceUnit::SequenceUnit(std::string* content)//常规构造函数
        {
            this->sequence = content;
        }
        
        SequenceUnit::~SequenceUnit()
        {
            
        }

        //此函数是用于计算两个碱基的变化率，返回伸缩变换后的int值
        int getCharacteristicPoint(char first,char second)
        {
            int first_value=0;
            int second_value=0;
            switch (first)
            {
            case 'A':
                first_value = _A_VALUE_;
                break;
            case 'T':
                first_value = _T_VALUE_;
                break;
            case 'C':
                first_value = _C_VALUE_;
                break;
            case 'G':
                first_value = _G_VALUE_;
                break;
            default:
                break;
            }

            switch (second)
            {
            case 'A':
                second_value = _A_VALUE_;
                break;
            case 'T':
                second_value = _T_VALUE_;
                break;
            case 'C':
                second_value = _C_VALUE_;
                break;
            case 'G':
                second_value = _G_VALUE_;
                break;
            default:
                break;
            }

            return (second_value - first_value);
        }

        void SequenceUnit::generationChangeRate(int order,int number)
        {
            std::string str = *(this->sequence);
            if(order == _REVERSE_ORDER_ )
                {std::reverse(str.begin(),str.end());}//如果是反向指令则反转字符串
            int len = number;
            if(len>str.length())
                {len = str.length();}
            //printf("Length: %d ,",len);
            std::pair<int,float>* content = nullptr;

            //判断将数据存放在哪一个容器中
            if(order == _POSITIVE_ORDER_)
            {
                switch (number)
                {
                case 30:
                    content = positive_30_rate_array_container;
                    break;
                case 60:
                    content = positive_60_rate_array_container;
                    break;
                case 90:
                    content = positive_90_rate_array_container;
                    break;
                default:
                    throw("generationChangeRate owns illegal parameter");
                    break;
                }
            }
            else
            {
                switch (number)
                {
                case 30:
                    content = reverse_30_rate_array_container;
                    break;
                case 60:
                    content = reverse_60_rate_array_container;
                    break;
                case 90:
                    content = reverse_90_rate_array_container;
                    break;
                default:
                    throw("generationChangeRate owns illegal parameter");
                    break;
                }
            }
            //此段待确认以修改
            //目前情况：简单来说就是变化特征只考虑前90bp，共有三个值
            //不需要单独保留，实时计算即可

            std::vector<PAIR> point_pair_container;//点的临时计算容器
            //生成该序列的特征二位点数组
            for(int i=0;i<len-1;i++)
            {
                PAIR arr;
                arr.first = i;
                arr.second = getCharacteristicPoint(str[i],str[i+1]);
                printf("%c,%c: %f\n",str[i],str[i+1],arr.second);
                point_pair_container.push_back(arr);
                //printf("first second: %c,%c  value:%f",str[i],str[i+1],arr.second/_MULTIPLICATION_FACTOR_);debugger;
            }

            printf("%d\n",number);
            long long res = 0;

            for(auto iter = point_pair_container.begin();iter!=point_pair_container.end();iter++)
            {
                res+=(*iter).second;
            }

            printf("res: %f\n" ,res);
            res*=4861;
            //res/=(point_pair_container.size()+1);
            printf("res2: %f\n" ,res);
            content->second = res/(point_pair_container.size()+1);
            std::vector<PAIR>().swap(point_pair_container);//释放临时容器（vector应该会导致内存泄露）
            
        }
        
        void SequenceUnit::generateCharacteristicValue(int order)
        {
            std::string str = *(this->sequence);//先拷贝字符串
            std::vector<float>* temp_vector = &positive_value_container;
            if(order == _REVERSE_ORDER_ )//如果是反向指令则反转字符串
                {
                    std::reverse(str.begin(),str.end());
                    temp_vector = &reverse_value_container;
                }
            int StrLen = str.length();

            for(int SeqLen = 30;SeqLen<=390;SeqLen+=30)
            {
                float TempValue = 0;

                if(SeqLen>StrLen)
                {
                    if(SeqLen-30 < StrLen)
                    {
                        for(int i=0;i<StrLen;i++)
                        {
                            switch (str[i])
                            {
                                case 'A': TempValue+=_A_CHARACTERISTIC_;break;
                                case 'G': TempValue+=_G_CHARACTERISTIC_;break;
                                case 'C': TempValue+=_C_CHARACTERISTIC_;break;
                                case 'T': TempValue+=_T_CHARACTERISTIC_;break;
                                default: throw("Err in text");break;
                            }
                        }
                        TempValue/=StrLen;
                    }
                }
                else
                {
                    int DivisorFactor = SeqLen;
                    for(int i = 0;;)
                    {
                        switch (str[i])
                        {
                            case 'A': TempValue+=_A_CHARACTERISTIC_;break;
                            case 'G': TempValue+=_G_CHARACTERISTIC_;break;
                            case 'C': TempValue+=_C_CHARACTERISTIC_;break;
                            case 'T': TempValue+=_T_CHARACTERISTIC_;break;
                            default: throw("Err in text");break;
                        }
                        if(i==SeqLen-1){break;}

                        if(i<StrLen-1){i++;}
                        else{DivisorFactor = i+1;break;}
                    }
                    TempValue /= DivisorFactor;
                }

                temp_vector->push_back(TempValue);
            }
            
        }
    }
}