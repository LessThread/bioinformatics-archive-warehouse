#include "IO.h"
namespace PROJECT_NAME::MOD_NAME
{
    void tryOutput(int start,int end)
    {
        PROJECT_NAME::G_TASK_START = start;
        PROJECT_NAME::G_TASK_END = end;
        readFile(INPUT_FILE_PATH);
    }

    void readFile(std::string FilePath)
    {

        if (std::string Default = INPUT_FILE_PATH ;FilePath.compare(Default))
            EXCEPTION::printWarnning("File name not specified, default will be used.");
        
        std::fstream ReadFP;
        ReadFP.open(FilePath,std::ios::in);

        if(!ReadFP.is_open())
        {
            EXCEPTION::printError("File cannot be found.");
            std::cout<<FilePath<<"\n";
            ReadFP.close();
            exit(101);
        }
        
        LONG index = 1;//给序列重新编码
        LONG ClassIndex = 0; //分类的编号

        char buffer_id[READ_BUFFER_SZIE] = {0};
        char buffer_seq[READ_BUFFER_SZIE] = {0};
        char buffer_weight[READ_BUFFER_SZIE] = {0};
        char buffer_plus[READ_BUFFER_SZIE] = {0};

        std::vector<std::thread>threads;
        //优化可用线程池
        while(!ReadFP.eof())
        {
            ReadFP.getline(buffer_id,READ_BUFFER_SZIE);
            ReadFP.getline(buffer_seq,READ_BUFFER_SZIE);
            ReadFP.getline(buffer_plus,READ_BUFFER_SZIE);
            ReadFP.getline(buffer_weight,READ_BUFFER_SZIE);
            
            if(index == 185173402)
            {
                break;
            }

            //生成结构体
            PROJECT_NAME::SQE::sequencing item = PROJECT_NAME::SQE::sequencing(index++,buffer_seq,buffer_weight);
            
            //生成识别码,设置哈希对应分类
            item.generateHash();
            if(auto key = HashMap.find(item.hash);key == HashMap.end())
            {
                HashMap.insert(std::pair<std::string,LONG>(item.hash,ClassIndex));
                item.ClassIndex = ClassIndex;
                std::vector<::PROJECT_NAME::SQE::sequencing> temp;
                PROJECT_NAME::SeqVector.push_back(temp);
                ClassIndex++;
            }
            else
            {
                item.ClassIndex = key->second;
            }

            DEBUG{std::cout<<item.hash<<"\n";}

            
            //可能是指定版本问题，服务器无法适当处理
            //异步move序列单位，节省内存

            //多线程在服务器上运行貌似有问题



            (PROJECT_NAME::SeqVector[item.ClassIndex]).push_back(std::move(item));
           


            if(index%100000 == 0)
            {
                printf("%d\n",index);
            }
        }
        ReadFP.close();
        
        
    }

    void outputFile(std::string QNFilePath,std::string CQFilePath)
    {
        if (std::string Default = OUTPUT_QN_FILE_PATH ;QNFilePath.compare(Default))
            EXCEPTION::printWarnning("File name not specified, default will be used.");
        
        std::fstream QNWriteFP;
        
        QNWriteFP.open(QNFilePath,std::ios::out | std::ios::trunc);

        if(!QNWriteFP.is_open())
        {
            EXCEPTION::printError("QNFile output failed.");
            QNWriteFP.close();
            exit(102);
        }

        LONG index = 0;
        for(auto iter1 : SeqVector)
        {
            std::fstream CQWriteFP;
            CQWriteFP.open(CQFilePath+std::to_string(index) ,std::ios::out | std::ios::trunc);
            if(!CQWriteFP.is_open())
            {
                EXCEPTION::printError("CQFile output failed.");
                printf("index:%d\n",index);
                CQWriteFP.close();
                //QNWriteFP.close();
                continue;
            }
            
            for(auto iter2 : iter1)
            {
                DEBUG{std::cout<<iter2.ClassIndex << "-" << iter2.index <<"\n" << iter2.seq << "\n";}
                CQWriteFP << iter2.ClassIndex << "-" << iter2.index <<"\n" << iter2.seq << "\n";
                QNWriteFP << iter2.index << "\n" <<iter2.weight<<"\n";
            }

            CQWriteFP.close();

            index++;
        }

        QNWriteFP.close();
    }
}