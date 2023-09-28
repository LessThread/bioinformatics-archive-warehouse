/*#include "cuda_runtime.h"
#include "device_launch_parameters.h"*/
#include <iostream>
#include <io.h>
#include <string>
#include <set>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
# include <sstream>
#include <vector>
#include <chrono>
#include <math.h>
#include<mutex>
#pragma comment(lib, "pthreadVC2.lib")
#define THREAD_NUM 11
using namespace std;
mutex mute;

typedef struct {
    int start;
    int end;
    std::string file;
}scope;

int count;


int bianhao=1;



std::ifstream& seek_to_line(std::ifstream& in, int line)
//���򿪵��ļ�in����λ��line�С�
{
    int i;
    char buf[1024];
    in.seekg(0, std::ios::beg);  //��λ���ļ���ʼ��
    for (i = 0; i < line; i++)
    {
        in.getline(buf, sizeof(buf));//��ȡ�С�
    }
    return in;
}

string reverse1(string a,int n)
{
string b;
for(int i=n-1;i>=0;i--)
    b+=a[i];
return b;
}

int count1=1;

void* readData(void* str)
{
    scope* scope1 = (scope*)str;
    int start = scope1->start;
    int end = scope1->end;
    std::string file = scope1->file;
    std::ifstream input(file);
    std::string b1, b2, b3, b4;
    int x = start;
    seek_to_line(input, start * 4);
    //int max = 0;


    while (x >= start && x <= end && getline(input, b1) && getline(input, b2) && getline(input, b3) && getline(input, b4))
    {
        x = x + 1;
        std::string b , c ;
        for (int i = 1; i < b1.length(); i++)
        {
            b = b + b1[i];
        }
        const char* a1;

        if (input.eof()) break;
        c = b + "\n" + b2 ;
        a1 = c.c_str();
        int xx = strlen(a1);
        string a2;
        int p;

        for(p=0;p<xx;p++)
        {
            if(a1[p]=='A'||a1[p]=='T'||a1[p]=='G'||a1[p]=='C')
            break;
        }
        for(;p<xx;p++)
            a2+=a1[p];

        int x1 = a2.length();
        string a3=reverse1(a2,x1);
        int x2=a3.length();

        double $match_value[20],$match_value1[20];
        for(int q=1;q<=20;q++)
        {

                if(q*30<x1)
                {
                    double sumx=0,zhixinx=0;
                    for(int w=0;w<q*30;w++)
                    {
                        if(a2[w]=='A')
                            sumx+=16384;
                        else if(a2[w]=='T')
                            sumx+=32768;
                        else if(a2[w]=='C')
                            sumx-=16384;
                        else if(a2[w]=='G')
                            sumx-=32768;
                    }
                    zhixinx=sumx/(q*30);
                    $match_value[q-1]=zhixinx;
                }

                
                else if(q*30>x1&&(q-1)*30>x1)
                {
                    $match_value[q-1]=0;
                }

                else
                {
                    double sumx=0,zhixinx=0;
                    for(int w=0;w<x1;w++)
                    {
                        if(a2[w]=='A')
                        sumx+=16384;
                        else if(a2[w]=='T')
                        sumx+=32768;
                            else if(a2[w]=='C')
                        sumx+=-16384;
                                else if(a2[w]=='G')
                        sumx+=-32768;
                    }
                    zhixinx=sumx/x1;
                    $match_value[q-1]=zhixinx;
                }
        }

        for(int qw=1;qw<=20;qw++)
        {
            if(qw*30<x2)
            {
                double sumx=0,zhixinx=0;
                for(int w=0;w<qw*30;w++)
                {
                    if(a3[w]=='A')
                    sumx+=16384;
                    else if(a3[w]=='T')
                    sumx+=32768;
                    else if(a3[w]=='C')
                    sumx-=16384;
                    else if(a3[w]=='G')
                    sumx-=32768;
                }
                zhixinx=sumx/(qw*30);
                $match_value1[qw-1]=zhixinx;
            }

            else if(qw*30>x2&&(qw-1)*30>x2)
            {
                $match_value1[qw-1]=0;
            }

            else
            {
                double sumx=0,zhixinx=0;
                for(int w=0;w<x2;w++)
                {
                if(a3[w]=='A')
                sumx+=16384;
                else if(a3[w]=='T')
                sumx+=32768;
                    else if(a3[w]=='C')
                sumx+=-16384;
                    else if(a3[w]=='G')
                sumx+=-32768;
                }
                zhixinx=sumx/x2;
                $match_value1[qw-1]=zhixinx;
            }
        }

        mute.lock();
        ofstream ofs;
        ofs.open("D:\\zhiju2.txt",ios::app);
        ofs<<bianhao<<endl;
        ofs<<a2<<endl;
        bianhao++;

        for(int r=0;r<19;r++)
        {
        ofs<<$match_value[r]<<',';
        }
        ofs<<$match_value[19]<<endl;
        for(int r=0;r<19;r++)
        {
        ofs<<$match_value1[r]<<',';
        }
        ofs$match_value1[19]<<endl;

        ofs<<endl;
        ofs.close();

        mute.unlock();
    }
    return NULL;
}


void readFile(std::string file)
{
    std::ifstream input(file);
    if (input.fail())
    {
        std::cout << "Not exist file !" << std::endl;
        return;
    }
    else
    {
        pthread_t th[THREAD_NUM];
        int x = count / 4;
        int len = x / THREAD_NUM;
        scope scope1[THREAD_NUM];
        for (int i = 0; i < THREAD_NUM; i++)
        {
            if (i != THREAD_NUM - 1)
            {
                scope1[i] = {len * i , len * (i + 1) - 1 , file};
            }
            else
            {
                scope1[i] = {len * i , count , file};
            }

        }
        for (int i = 0; i < THREAD_NUM; i++)
        {
            pthread_create(&th[i], NULL, readData, &scope1[i]);
            //pthread_join(th[i], NULL);
        }
        for (int i = 0; i < THREAD_NUM; i++)
        {

            pthread_join(th[i], NULL);
        }

    }
    std::cout << "OK" << std::endl;
}
// ����·��path�£��ļ�����Ϊtype�������ļ� 
void findFiles(const std::string& path, const std::string& type, std::set<std::string>& files)
{
    _finddata_t data;  // �洢�ļ���Ϣ�Ľṹ��
    auto handle = _findfirst((path + "/*.*").c_str(), &data); //��ȡ��1�ļ����ļ���
    if (handle == -1)                                         //�ж��Ƿ���Զ�ȡ�ļ�
    {
        std::cout << "can not read file!";
        return;
    }
    do
    {
        std::string s = data.name;        //�ļ���
        if (data.attrib & _A_SUBDIR) // Ŀ¼
        {
            // if (s != "." && s != "..") //�ų��ļ���.���ļ���..
            //     std::cout<< "dir: " << s << std::endl;
        }
        else // �ļ�
        {
            std::string s1 = "." + type;
            if (s.rfind(s1) == s.size() - s1.size()) // �жϺ�׺�Ƿ�Ϊ.type
            {
                files.insert(path + "\\" + s);
            }
        }
    } while (_findnext(handle, &data) == 0); //��ȡ��һ���ļ����ļ���
    _findclose(handle);       // �ر��������
}
int CountLines(std::string file)
{
    std::ifstream ReadFile;
    int n = 0;
    std::string tmp;
    ReadFile.open(file, std::ios::in);
    if (ReadFile.fail()) return 0;
    else
    {
        while (getline(ReadFile, tmp, '\n')) n++;
        ReadFile.close();
        return n;
    }
}

int main()
{
    char path[1000];
    std::cout << "Enter path :" << " ";
    std::cin.getline(path, 800);
    std::string type = "fq";                                  //�ļ�����
    std::set <std::string> files;                                      //����ҵ����ļ�
    findFiles(path, type, files);
    for (auto file : files)
    {
        count = 0;
        std::cout << file << std::endl;
        count = CountLines(file);
       // std::cout << count << std::endl;
        readFile(file);
    }//����ҵ����ļ�
    return 0;
}
