#include "./include/settings.h"
#include "./include/IO.h"
#include "./include/SequenceUnit.h"
//#include "./main.h"
namespace _PROJECT_NAME_
{
    std::vector<std::string*> content_char_list;
    std::vector<SEQUENCE_UNIT::SequenceUnit*> sequence_vector;

    //下列为针对旧文件的快速生成
    std::vector<std::string*> line_1;
    std::vector<std::string*> line_2;
}


using namespace _PROJECT_NAME_;
int main(int argc,char** argv)
{
    //默认设置
    std::string in_file_name = _IN_FILE_NAME_;
    std::string out_file_name = _OUT_FILE_NAME_;

    //命令行处理
    if (argc == 1)
    {

    }
    else if(argc==3)
    {
        if(argv[1]=="-i")
        {
            in_file_name = argv[2];
        }
        else if(argv[1] == "-o")
        {
            out_file_name = argv[2];
        }
        else
        {
            printf("err");exit(1);
        }
    }
    else if(argc == 5)
    {
        if(argv[1]=="-i")
        {
            in_file_name = argv[2];
        }
        else if(argv[1] == "-o")
        {
            out_file_name = argv[2];
        }
        else
        {
            printf("err");exit(1);
        }

        if(argv[3]=="-i")
        {
            in_file_name = argv[4];
        }
        else if(argv[3] == "-o")
        {
            out_file_name = argv[4];
        }
        else
        {
            printf("err");exit(1);
        }
    }

    std::cout<<"Read: "<<in_file_name<<"\nOutput: "<<out_file_name<<"\n";
    IO::readFile(in_file_name);

    unsigned index = 1;
    //单节点生成
    for(auto iter : content_char_list)
    {
        SEQUENCE_UNIT::SequenceUnit* temp = new SEQUENCE_UNIT::SequenceUnit(iter);

        temp->generateCharacteristicValue(_POSITIVE_ORDER_);
        //std::reverse(temp->sequence->begin(),temp->sequence->end());
        temp->generateCharacteristicValue(_REVERSE_ORDER_);

        temp->generationChangeRate(_POSITIVE_ORDER_,30);
        temp->generationChangeRate(_POSITIVE_ORDER_,60);
        temp->generationChangeRate(_POSITIVE_ORDER_,90);
        temp->generationChangeRate(_REVERSE_ORDER_,30); 
        temp->generationChangeRate(_REVERSE_ORDER_,60);
        temp->generationChangeRate(_REVERSE_ORDER_,90);
        sequence_vector.push_back(temp);

        if((index++ % 10000) == 0){printf("running: %d\n",index);}
        //if(index==9) system("pause");
    }
    IO::writeFile(out_file_name);

    printf("Finished.");
    //debugger;
}

int _main(int argc,char** argv)
{
    //默认设置
    std::string in_file_name = _IN_FILE_NAME_;
    std::string out_file_name = _OUT_FILE_NAME_;

    //命令行处理
    if (argc == 1)
    {

    }
    else if(argc==3)
    {
        if(argv[1]=="-i")
        {
            in_file_name = argv[2];
        }
        else if(argv[1] == "-o")
        {
            out_file_name = argv[2];
        }
        else
        {
            printf("err");exit(1);
        }
    }
    else if(argc == 5)
    {
        if(argv[1]=="-i")
        {
            in_file_name = argv[2];
        }
        else if(argv[1] == "-o")
        {
            out_file_name = argv[2];
        }
        else
        {
            printf("err");exit(1);
        }

        if(argv[3]=="-i")
        {
            in_file_name = argv[4];
        }
        else if(argv[3] == "-o")
        {
            out_file_name = argv[4];
        }
        else
        {
            printf("err");exit(1);
        }
    }

    std::cout<<"Read: "<<in_file_name<<"\nOutput: "<<out_file_name<<"\n";
    IO::readFile(in_file_name);

    unsigned index = 1;
    //单节点生成
    for(auto iter : content_char_list)
    {
        SEQUENCE_UNIT::SequenceUnit* temp = new SEQUENCE_UNIT::SequenceUnit(iter);

        temp->generateCharacteristicValue(_POSITIVE_ORDER_);
        //std::reverse(temp->sequence->begin(),temp->sequence->end());
        temp->generateCharacteristicValue(_REVERSE_ORDER_);

        temp->generationChangeRate(_POSITIVE_ORDER_,30);
        temp->generationChangeRate(_POSITIVE_ORDER_,60);
        temp->generationChangeRate(_POSITIVE_ORDER_,90);
        temp->generationChangeRate(_REVERSE_ORDER_,30); 
        temp->generationChangeRate(_REVERSE_ORDER_,60);
        temp->generationChangeRate(_REVERSE_ORDER_,90);
        sequence_vector.push_back(temp);

        if((index++ % 10000) == 0){printf("running: %d\n",index);}
        //if(index==9) system("pause");
    }
    IO::writeFile(out_file_name);

    printf("Finished.");
    //debugger;
}

//自旧文件生成（已废弃）
int mainFormOld()
{
    
    //IO::readFile();//读取文件
    std::string file_name = "../../old/zhiju2.txt";
    std::string res_file_name = "../../old/res1.txt";

    IO::readFileFormOld(file_name);

    auto iter2 = line_1.begin();
    auto iter3 = line_2.begin();
    for( auto iter : content_char_list )
    {
        SEQUENCE_UNIT::SequenceUnit* temp = new SEQUENCE_UNIT::SequenceUnit(iter);
        temp->temp_positive_str = *iter2;iter2++;
        temp->temp_reverse_str = *iter3;iter3++;
        temp->generationChangeRate(_POSITIVE_ORDER_,30);
        temp->generationChangeRate(_POSITIVE_ORDER_,60);
        temp->generationChangeRate(_POSITIVE_ORDER_,90);
        temp->generationChangeRate(_REVERSE_ORDER_,30); 
        temp->generationChangeRate(_REVERSE_ORDER_,60);
        temp->generationChangeRate(_REVERSE_ORDER_,90);
        sequence_vector.push_back(temp);
        //完成单节点的生成
    }

    IO::writeFileToOld(res_file_name);
    //system("pause");
    return 0;
}
