#include "./sequencing.h"

namespace PROJECT_NAME::MOD_NAME
{
    sequencing::sequencing(char index[],char seq[])
    {
        auto temp = std::string(index);
        this->index = std::stoll(temp);
        temp.shrink_to_fit();

        temp = std::string(seq);
        this->seq = temp;
        temp.shrink_to_fit();
    }

    sequencing::sequencing(LONG index,char seq[],char weight[])
    {
        this->index = index;
        
        auto temp = std::string(seq);
        this->seq = temp;
        temp.shrink_to_fit();

        temp = std::string(weight);
        this->weight = temp;
        temp.shrink_to_fit();
    }

    sequencing::sequencing(char index[],char seq[],char weight[])
    {
        auto temp = std::string(index);
        this->index = std::stoll(temp);
        temp.shrink_to_fit();

        temp = std::string(seq);
        this->seq = temp;
        temp.shrink_to_fit();

        temp = std::string(weight);
        this->weight = temp;
        temp.shrink_to_fit();
    }

    sequencing::sequencing(LONG index,std::string seq,std::string weight)
    {
        this->index = index;
        this->seq = seq;
        this->weight = weight;
    }
    
    sequencing::~sequencing()
    {
        this->seq.shrink_to_fit();
        this->weight.shrink_to_fit();
        this->hash.shrink_to_fit();
    }

    void sequencing::setHash(std::string hash)
    {
        this->hash = hash;
        this->seq = this->seq.substr(0,100);
        DEBUG{std::cout<<"seq:"<<this->seq<<"\n";}
    }

    void sequencing::generateHash()
    {
        std::string Seq = this->seq;
        std::string Hash = "";

try
{
        Hash += Seq.substr(100,10);
        Hash += "#";
        Hash += Seq.substr(117,10);
        Hash += "#";
        Hash += Seq.substr(133,10);
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    std::cout <<"Index:"<< this->index<<"\n";
    std::cout<<"seq"<<this->seq<<"\n";

}

       
        
        this->setHash(Hash);
        Seq.shrink_to_fit();
        Hash.shrink_to_fit();
    }
}