#include "../include/IO.h"

namespace _PROJECT_NAME_
{
    extern std::vector<std::string*> content_char_list;
    extern std::vector<std::string*> line_1;
    extern std::vector<std::string*> line_2;
    extern std::vector<SEQUENCE_UNIT::SequenceUnit*> sequence_vector;

    namespace _MOD_NAME_
    {
        void readFile(std::string in_file_name)
        {
            unsigned count = 1;
            std::fstream fp;
            fp.open(in_file_name,std::ios::in);

            if(!fp.is_open())
            {
                printf("ERR: File open error!");
                exit(1);
            }

            char length_1[_LINE1_BUF_SIZE_];//@length = ...
            char content_2[_LINE2_BUF_SIZE_];//ACGGGT 只有这行是有效数据
            char blank_3[_LINE3_BUF_SIZE_];//+
            char value_4[_LINE4_BUF_SIZE_];//lllllllll

            while(fp.getline(length_1,(sizeof(char)*_LINE1_BUF_SIZE_)))
            {
                fp.getline(content_2,(sizeof(char)*_LINE2_BUF_SIZE_));
                fp.getline(blank_3,(sizeof(char)*_LINE3_BUF_SIZE_));
                fp.getline(value_4,(sizeof(char)*_LINE4_BUF_SIZE_));

                std::string* _content = new std::string(content_2);
                content_char_list.push_back(_content);
                //只将序列放入内存中
                if(count++ == 10)break;
            }
            fp.close();
        }

        void writeFile(std::string out_file_name)
        {
            std::fstream fp;
            fp.open(out_file_name,std::ios::out);
            if(!fp.is_open())
            {
                printf("ERR: File open error!");
                exit(1);
            }

            unsigned index = 1;
            for(auto iter : sequence_vector)
            {
                fp<<index++<<"\n";
                fp<<*(iter->sequence)<<"\n";
                for(auto iter_pos : iter->positive_value_container)
                {
                    fp<<iter_pos<<",";
                }
                fp<<"\n";
                fp<<(iter)->positive_30_rate_array_container->second<<" ";
                fp<<(iter)->positive_60_rate_array_container->second<<" ";
                fp<<(iter)->positive_90_rate_array_container->second<<"\n";
                for(auto iter_rev : iter->reverse_value_container)
                {
                    fp<<iter_rev<<",";
                }
                fp<<"\n";
                fp<<iter->reverse_30_rate_array_container->second<<" ";
                fp<<iter->reverse_60_rate_array_container->second<<" ";
                fp<<iter->reverse_90_rate_array_container->second<<"\n";

                fp<<"----\n";

            }
        }

        //此函数用于从旧代码中直接调用以生成变化率
        void readFileFormOld(std::string FILE_NAME)
        {
            std::fstream fp;
            fp.open(FILE_NAME,std::ios::in);

            if(!fp.is_open())
            {
                printf("ERR: File open error!");
                exit(1);
            }

            char ID_1[_LINE1_BUF_SIZE_];
            char content_2[_LINE2_BUF_SIZE_];
            char PV_3[_LINE3_BUF_SIZE_];
            char RV_4[_LINE4_BUF_SIZE_];
            char blank[20];

            int count = 0;

            while(fp.getline(ID_1,(sizeof(char)*_LINE1_BUF_SIZE_)))
            {
                fp.getline(content_2,(sizeof(char)*_LINE2_BUF_SIZE_));
                fp.getline(PV_3,(sizeof(char)*_LINE3_BUF_SIZE_));
                fp.getline(RV_4,(sizeof(char)*_LINE4_BUF_SIZE_));
                fp.getline(blank,20);

                std::string* _content = new std::string(content_2);
                std::string* $line_1 = new std::string(PV_3);
                std::string* $line_2 = new std::string(RV_4);

                content_char_list.push_back(_content);
                line_1.push_back($line_1);
                line_2.push_back($line_2);
                
                // if(count == 20)
                //     {break;printf("back\n");}
                // else
                //     {count++;}
            }

            fp.close();
        }

        void writeFileToOld(std::string FILE_NAME)
        {
            ::std::fstream fp;
            fp.open(FILE_NAME,std::ios::out);
            if(!fp.is_open())
            {
                printf("ERR: Write open error!");
                exit(1);
            }

            unsigned index = 1;

            for(auto item : ::_PROJECT_NAME_::sequence_vector)
            {
                fp<<index++<<"\n";
                fp<<*(item->sequence)<<"\n";

                fp<<*(item->temp_positive_str)<<"\n";
                fp<<(item)->positive_30_rate_array_container->second<<" ";
                fp<<(item)->positive_60_rate_array_container->second<<" ";
                fp<<(item)->positive_90_rate_array_container->second<<"\n";

                fp<<*(item->temp_reverse_str)<<"\n";
                fp<<item->reverse_30_rate_array_container->second<<" ";
                fp<<item->reverse_60_rate_array_container->second<<" ";
                fp<<item->reverse_90_rate_array_container->second<<"\n";

                fp<<"----\n";
            }
        }
    }
}