#include <atomic>
#include <iterator> 
#include <map>
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <list>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#include <set>
#include <io.h>
#include <windows.h>
#include <iomanip>
#include <psapi.h>
#include <cmath>

#include <memory>
#include <functional>
#include <utility>
#include <condition_variable>
#include <queue>
#include <future>

#pragma comment(lib,"psapi.lib")

#define M (4)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)
#define readlength 100

using namespace std;

typedef struct BPlusNode* BPlusTree, * Position;
typedef struct BPlusNode Node;
typedef struct keyType KeyType;

static std::mutex mymutex;
static ifstream filein;
static BPlusTree T;

struct keyType
{
    int id;
    char xulie[1000];
};

struct BPlusNode {
    int KeyNum;
    KeyType Key[M + 1];
    BPlusTree Children[M + 1];
    BPlusTree Next;
};

using namespace std;

/* ��ʼ�� */
extern BPlusTree Initialize();
/* ���� */
extern BPlusTree Insert(BPlusTree T, KeyType Key);
/* ɾ�� */
extern BPlusTree Remove(BPlusTree T, KeyType Key);
/* ���� */
extern BPlusTree Destroy(BPlusTree T);
/* �����ڵ� */
extern void Travel(BPlusTree T);
/* ������Ҷ�ڵ������ */
extern void TravelData(BPlusTree T);

static KeyType Unavailable = { 0 , NULL };

/* ���ɽڵ㲢��ʼ�� */
static BPlusTree MallocNewNode() {
    BPlusTree NewNode;
    int i;
    NewNode = (BPlusTree)malloc(sizeof(struct BPlusNode));
    if (NewNode == NULL)
        exit(EXIT_FAILURE);


    i = 0;
    while (i < M + 1) {
        NewNode->Key[i] = Unavailable;
        NewNode->Children[i] = NULL;
        i++;
    }
    NewNode->Next = NULL;
    NewNode->KeyNum = 0;

    return NewNode;
}

/* ��ʼ�� */
extern BPlusTree Initialize() {

    BPlusTree T;
    if (M < (3)) {
        printf("M��С����3��");
        exit(EXIT_FAILURE);
    }
    /* ����� */
    T = MallocNewNode();
    return T;
}

static Position FindMostLeft(Position P) {
    Position Tmp;
    Tmp = P;
    while (Tmp != NULL && Tmp->Children[0] != NULL) {
        Tmp = Tmp->Children[0];
    }
    return Tmp;
}

static Position FindMostRight(Position P) {
    Position Tmp;

    Tmp = P;

    while (Tmp != NULL && Tmp->Children[Tmp->KeyNum - 1] != NULL) {
        Tmp = Tmp->Children[Tmp->KeyNum - 1];
    }
    return Tmp;
}

/* Ѱ��һ���ֵܽڵ㣬��洢�Ĺؼ���δ�������򷵻�NULL */
static Position FindSibling(Position Parent, int i) {
    Position Sibling;
    int Limit;

    Limit = M;

    Sibling = NULL;
    if (i == 0) {
        if (Parent->Children[1]->KeyNum < Limit)
            Sibling = Parent->Children[1];
    }
    else if (Parent->Children[i - 1]->KeyNum < Limit)
        Sibling = Parent->Children[i - 1];
    else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum < Limit) {
        Sibling = Parent->Children[i + 1];
    }

    return Sibling;
}

/* �����ֵܽڵ㣬��ؼ���������M/2 ;û�з���NULL*/
static Position FindSiblingKeyNum_M_2(Position Parent, int i, int* j) {
    int Limit;
    Position Sibling;
    Sibling = NULL;

    Limit = LIMIT_M_2;

    if (i == 0) {
        if (Parent->Children[1]->KeyNum > Limit) {
            Sibling = Parent->Children[1];
            *j = 1;
        }
    }
    else {
        if (Parent->Children[i - 1]->KeyNum > Limit) {
            Sibling = Parent->Children[i - 1];
            *j = i - 1;
        }
        else if (i + 1 < Parent->KeyNum && Parent->Children[i + 1]->KeyNum > Limit) {
            Sibling = Parent->Children[i + 1];
            *j = i + 1;
        }
    }
    return Sibling;
}

/* ��Ҫ��X����Key��ʱ��i��X��Parent��λ�ã�j��KeyҪ�����λ��
   ��Ҫ��Parent����X�ڵ��ʱ��i��Ҫ�����λ�ã�Key��j��ֵû����
 */
static Position InsertElement(int isKey, Position Parent, Position X, KeyType Key, int i, int j) {

    int k;
    if (isKey) {
        /* ����key */
        k = X->KeyNum - 1;
        while (k >= j) {
            X->Key[k + 1] = X->Key[k]; k--;
        }

        X->Key[j] = Key;

        if (Parent != NULL)
            Parent->Key[i] = X->Key[0];

        X->KeyNum++;

    }
    else {
        /* ����ڵ� */

        /* ����Ҷ�ڵ�������� */
        if (X->Children[0] == NULL) {
            if (i > 0)
                Parent->Children[i - 1]->Next = X;
            X->Next = Parent->Children[i];
        }

        k = Parent->KeyNum - 1;
        while (k >= i) {
            Parent->Children[k + 1] = Parent->Children[k];
            Parent->Key[k + 1] = Parent->Key[k];
            k--;
        }
        Parent->Key[i] = X->Key[0];
        Parent->Children[i] = X;

        Parent->KeyNum++;

    }
    return X;
}


static Position RemoveElement(int isKey, Position Parent, Position X, int i, int j) {

    int k, Limit;

    if (isKey) {
        Limit = X->KeyNum;
        /* ɾ��key */
        k = j + 1;
        while (k < Limit) {
            X->Key[k - 1] = X->Key[k]; k++;
        }

        X->Key[X->KeyNum - 1] = Unavailable;

        Parent->Key[i] = X->Key[0];

        X->KeyNum--;
    }
    else {
        /* ɾ���ڵ� */

        /* �޸���Ҷ�ڵ������ */
        if (X->Children[0] == NULL && i > 0) {
            Parent->Children[i - 1]->Next = Parent->Children[i + 1];
        }
        Limit = Parent->KeyNum;
        k = i + 1;
        while (k < Limit) {
            Parent->Children[k - 1] = Parent->Children[k];
            Parent->Key[k - 1] = Parent->Key[k];
            k++;
        }

        Parent->Children[Parent->KeyNum - 1] = NULL;
        Parent->Key[Parent->KeyNum - 1] = Unavailable;

        Parent->KeyNum--;

    }
    return X;
}

/* Src��Dst���������ڵĽڵ㣬i��Src��Parent�е�λ�ã�
 ��Src��Ԫ���ƶ���Dst�� ,n���ƶ�Ԫ�صĸ���*/
static Position MoveElement(Position Src, Position Dst, Position Parent, int i, int n) {
    KeyType TmpKey;
    Position Child;
    int j, SrcInFront;

    SrcInFront = 0;

    if (Src->Key[0].id < Dst->Key[0].id)
        SrcInFront = 1;

    j = 0;
    /* �ڵ�Src��Dstǰ�� */
    if (SrcInFront) {
        if (Src->Children[0] != NULL) {
            while (j < n) {
                Child = Src->Children[Src->KeyNum - 1];
                RemoveElement(0, Src, Child, Src->KeyNum - 1, INT_MIN);
                InsertElement(0, Dst, Child, Unavailable, 0, INT_MIN);
                j++;
            }
        }
        else {
            while (j < n) {
                TmpKey = Src->Key[Src->KeyNum - 1];
                RemoveElement(1, Parent, Src, i, Src->KeyNum - 1);
                InsertElement(1, Parent, Dst, TmpKey, i + 1, 0);
                j++;
            }

        }

        Parent->Key[i + 1] = Dst->Key[0];
        /* ����Ҷ�ڵ��������� */
        if (Src->KeyNum > 0)
            FindMostRight(Src)->Next = FindMostLeft(Dst);

    }
    else {
        if (Src->Children[0] != NULL) {
            while (j < n) {
                Child = Src->Children[0];
                RemoveElement(0, Src, Child, 0, INT_MIN);
                InsertElement(0, Dst, Child, Unavailable, Dst->KeyNum, INT_MIN);
                j++;
            }

        }
        else {
            while (j < n) {
                TmpKey = Src->Key[0];
                RemoveElement(1, Parent, Src, i, 0);
                InsertElement(1, Parent, Dst, TmpKey, i - 1, Dst->KeyNum);
                j++;
            }

        }

        Parent->Key[i] = Src->Key[0];
        if (Src->KeyNum > 0)
            FindMostRight(Dst)->Next = FindMostLeft(Src);

    }

    return Parent;
}

static BPlusTree SplitNode(Position Parent, Position X, int i) {
    int j, k, Limit;
    Position NewNode;

    NewNode = MallocNewNode();

    k = 0;
    j = X->KeyNum / 2;
    Limit = X->KeyNum;
    while (j < Limit) {
        if (X->Children[0] != NULL) {
            NewNode->Children[k] = X->Children[j];
            X->Children[j] = NULL;
        }
        NewNode->Key[k] = X->Key[j];
        X->Key[j] = Unavailable;
        NewNode->KeyNum++; X->KeyNum--;
        j++; k++;
    }

    if (Parent != NULL)
        InsertElement(0, Parent, NewNode, Unavailable, i + 1, INT_MIN);
    else {
        /* �����X�Ǹ�����ô�����µĸ������� */
        Parent = MallocNewNode();
        InsertElement(0, Parent, X, Unavailable, 0, INT_MIN);
        InsertElement(0, Parent, NewNode, Unavailable, 1, INT_MIN);

        return Parent;
    }

    return X;
}

/* �ϲ��ڵ�,X����M/2�ؼ��֣�S�д��ڻ����M/2���ؼ���*/
static Position MergeNode(Position Parent, Position X, Position S, int i) {
    int Limit;

    /* S�Ĺؼ�����Ŀ����M/2 */
    if (S->KeyNum > LIMIT_M_2) {
        /* ��S���ƶ�һ��Ԫ�ص�X�� */
        MoveElement(S, X, Parent, i, 1);
    }
    else {
        /* ��Xȫ��Ԫ���ƶ���S�У�����Xɾ�� */
        Limit = X->KeyNum;
        MoveElement(X, S, Parent, i, Limit);
        RemoveElement(0, Parent, X, i, INT_MIN);

        free(X);
        X = NULL;
    }
    return Parent;
}

static BPlusTree RecursiveInsert(BPlusTree T, KeyType Key, int i, BPlusTree Parent) {
    int j, Limit;
    Position Sibling;

    /* ���ҷ�֧ */
    j = 0;
    while (j < T->KeyNum && (Key.id > T->Key[j].id))
    {
        /* �ظ�ֵ������ */
        if (Key.id == T->Key[j].id)
            return T;
        j++;
    }
    if (j != 0 && T->Children[0] != NULL) j--;

    /* ��Ҷ */
    if (T->Children[0] == NULL)
        T = InsertElement(1, Parent, T, Key, i, j);
    /* �ڲ��ڵ� */
    else
        T->Children[j] = RecursiveInsert(T->Children[j], Key, j, T);

    /* �����ڵ� */

    Limit = M;

    if (T->KeyNum > Limit) {
        /* �� */
        if (Parent == NULL) {
            /* ���ѽڵ� */
            T = SplitNode(Parent, T, i);
        }
        else {
            Sibling = FindSibling(Parent, i);
            if (Sibling != NULL) {
                /* ��T��һ��Ԫ�أ�Key����Child���ƶ���Sibing�� */
                MoveElement(T, Sibling, Parent, i, 1);
            }
            else {
                /* ���ѽڵ� */
                T = SplitNode(Parent, T, i);
            }
        }

    }
    if (Parent != NULL)
        Parent->Key[i] = T->Key[0];
    return T;
}

/* ���� */
extern BPlusTree Insert(BPlusTree T, KeyType Key) {
    return RecursiveInsert(T, Key, 0, NULL);
}

static void RecursiveTravel(BPlusTree T, int Level) {
    int i;
    if (T != NULL) {
        printf("  ");
        printf("[Level:%d]-->", Level);
        printf("(");
        i = 0;
        while (i < T->KeyNum)
            std::cout << T->Key[i++].id << " ";
        printf(")");
        Level++;
        i = 0;
        while (i <= T->KeyNum) {
            RecursiveTravel(T->Children[i], Level);
            i++;
        }
    }
}

/* ���� */
extern void Travel(BPlusTree T) {
    RecursiveTravel(T, 0);
    printf("\n");
}

char * quickCheck(BPlusTree T, KeyType key)
{
    if (T == NULL)
    {
        char a[] = "No";
        /*std::cout << "No" << std::endl;*/
        return a ;
    }
    int j = 0;

    while (j < T->KeyNum && (key.id >= T->Key[j].id))
    {
        if (key.id == T->Key[j].id)
        {
            return T->Key[j].xulie;
        }
        j++;
    }
    j--;

    if (key.id == T->Key[j].id){
        return T->Key[j].xulie;
    }
    else
    {
        if (T->Children[j] != NULL)
        {
            T = T->Children[j];
            quickCheck(T, key);
        }
    }
}

/* ������Ҷ�ڵ������ */
extern void TravelData(BPlusTree T) {
    Position Tmp;
    int i;
    if (T == NULL)
        return;
    printf("All Data:");
    Tmp = T;
    while (Tmp->Children[0] != NULL)
        Tmp = Tmp->Children[0];
    /* ��һƬ��Ҷ */
    while (Tmp != NULL) {//Tmp != NULL
        i = 0;
        while (i < Tmp->KeyNum)
        {
            std::cout << Tmp->Key[i++].id << std::endl;
        }
        Tmp = Tmp->Next;
    }
}

void line_process(std::string& line, const std::string comment_str = "#")
{
    for (char& c : line)//C++11���ϰ汾���﷨
    {
        //�Ʊ��� tab�����ţ��ֺŶ�������Ч�ķָ�����ͳһת�ɿո�
        //Ϊ�˱�����󣬻س����ͻ��з�ҲתΪ�ո񣨷����޷��������У�
        if (c == '\t' || c == ',' || c == ';' || c == '\r' || c == '\n')
            c = ' ';
    }

    line.erase(0, line.find_first_not_of(" "));//ɾ�����׿ո�
    line.erase(line.find_last_not_of(" ") + 1);//ɾ����ĩ�ո�
}

void readFile(std::string file, BPlusTree& T)
{
    std::ifstream input(file);
    if (input.fail())
    {
        std::cout << "Not exist file !" << std::endl;
        return;
    } else {
        std::string b1, b2;
        std::string x1, x2, x3;

        while (getline(input, b1))
        {

            if (b1.empty()) continue;
            getline(input, b2);
            
            for(int i=0;i<5;i++)
                getline(input,x1);
            // input >> x1;
            // input >> x2;
            // input >> x3;

            //std::cout<<b1<<"\n"<<b2<<std::endl;system("pause");

            KeyType key = Unavailable;
            key.id = std::stoi(b1.c_str());
            strcpy_s(key.xulie, strlen(b2.c_str()) + 1, b2.c_str());
            T = Insert(T, key);
        }
    }
}

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
                // std::cout<< "file: " << s << std::endl;
            }
        }
    } while (_findnext(handle, &data) == 0); //��ȡ��һ���ļ����ļ���
    _findclose(handle);       // �ر��������
}

int CountLines(char *filename)
{
    ifstream ReadFile;
    int n=0;
    string tmp;
    ReadFile.open(filename,ios::in);//ios::in ��ʾ��ֻ���ķ�ʽ��ȡ�ļ�
    if(ReadFile.fail())//�ļ���ʧ��:����0
    {
        return 0;
    }
    else//�ļ�����
    {
        while(getline(ReadFile,tmp,'\n'))
        {
            n++;
        }
        ReadFile.close();
        return n;
    }
}

template <typename T>
class SafeQueue
{
private:
    std::queue<T> m_queue; //利用模板函数构造队列
    std::mutex m_mutex; // 访问互斥信号量
public:
    SafeQueue() {}
    SafeQueue(SafeQueue &&other) {}
    ~SafeQueue() {}
    bool empty() // 返回队列是否为空
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变
        return m_queue.empty();
    }
    int size()
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变
        return m_queue.size();
    }
    // 队列添加元素
    void enqueue(T &t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.emplace(t);
    }
    // 队列取出元素
    bool dequeue(T &t)
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 队列加锁
        if (m_queue.empty())
            return false;
        t = std::move(m_queue.front()); // 取出队首元素，返回队首元素值，并进行右值引用
        m_queue.pop(); // 弹出入队的第一个元素
        return true;
    }
};

class ThreadPool
{
private:
    class ThreadWorker // 内置线程工作类
    {
    private:
        int m_id; // 工作id
        ThreadPool *m_pool; // 所属线程池
    public:
        // 构造函数
        ThreadWorker(ThreadPool *pool, const int id) : m_pool(pool), m_id(id)
        {
        }
        // 重载()操作
        void operator()()
        {
            std::function<void()> func; // 定义基础函数类func
            bool dequeued; // 是否正在取出队列中元素
            while (!m_pool->m_shutdown)
            {
                {
                    // 为线程环境加锁，互访问工作线程的休眠和唤醒
                    std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
                    // 如果任务队列为空，阻塞当前线程
                    if (m_pool->m_queue.empty())
                    {
                        m_pool->m_conditional_lock.wait(lock); // 等待条件变量通知，开启线程
                    }
                    // 取出任务队列中的元素
                    dequeued = m_pool->m_queue.dequeue(func);
                }
                // 如果成功取出，执行工作函数
                if (dequeued)
                    func();
            }
        }
    };
    bool m_shutdown; // 线程池是否关闭
    SafeQueue<std::function<void()>> m_queue; // 执行函数安全队列，即任务队列
    std::vector<std::thread> m_threads; // 工作线程队列
    std::mutex m_conditional_mutex; // 线程休眠锁互斥变量
    std::condition_variable m_conditional_lock; // 线程环境锁，可以让线程处于休眠或者唤醒状态
public:
    inline bool isempty(){
        return m_queue.empty();
    }
    // 线程池构造函数
    ThreadPool(const int n_threads = 4)
        : m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false)
    {
    }
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;
    
    void init()
    {
        for (int i = 0; i < m_threads.size(); ++i)
        {
            m_threads.at(i) = std::thread(ThreadWorker(this, i)); // 分配工作线程
        }
    }
    
    void shutdown()
    {
        m_shutdown = true;
        m_conditional_lock.notify_all(); // 通知，唤醒所有工作线程
        for (int i = 0; i < m_threads.size(); ++i)
        {
            if (m_threads.at(i).joinable()) // 判断线程是否在等待
            {
                m_threads.at(i).join(); // 将线程加入到等待队列
            }
        }
    }

    template <typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        // 连接函数和参数定义，特殊函数类型，避免左右值错误
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
        std::function<void()> warpper_func = [task_ptr]()
        {
            (*task_ptr)();
        };
        // 队列通用安全封包函数，并压入安全队列
        m_queue.enqueue(warpper_func);
        // 唤醒一个等待中的线程
        m_conditional_lock.notify_one();
        // 返回先前注册的任务指针
        return task_ptr->get_future();
    }
};

bool match(const std::string &a, std::string &b, int &c)
{
    int min0 = min(a.length(), b.length());

    vector<double> num;

    for (int i = 10; i <= min0; i++) {
        int count = 0;
        for (int j = 0, k = a.length() - i; j < i; j++, k++)
            if (a[k] == b[j])
                count++;
        num.push_back( (double)count / i);
    }

    for (int i = num.size() - 1; i >= 0; i--) {
        if (num[i] >= 0.8) {
            b.erase(0, (size_t)(i + 10));
            if(b.length() == 0)
                return false;
            else {
                c = i + 10;
                return true;
            }
        }    
    }
    return false;
}

bool multiple_match(const std::string &a, const std::string &b)
{
    int min0 = min(a.length(), b.length());
    int i = 0;
    int count = 0;
    for(; i<min0; i++)
        if(a[i] == b[i])
            count++;
    if((double) count/(i+1) >= 0.8)
        return true;
    return false;
}

static void Multiple_Match_element(std::list<std::vector<std::pair<int, std::pair<std::string, int>>>> &element) {
    for(auto it = element.begin(); it != element.end();) {
        auto iter = it;
        iter++;
        for(; iter != element.end();) {
            if(multiple_match(it->front().second.first, iter->front().second.first)) {
                it->emplace_back(iter->front());
                if(it->back().second.first.length() > it->front().second.first.length()) {
                    it->front().second.first = iter->back().second.first;
                }
                it->back().second.first.clear();
                auto iterator = it;
                iterator++;
                bool flag = false;
                for(; iterator != element.end(); iterator++) {
                    if(iterator != iter && multiple_match(iter->front().second.first, iterator->front().second.first) && !multiple_match(it->front().second.first, iterator->front().second.first)){
                        flag = true;
                        break;
                    }
                }
                if(!flag)
                    element.erase(iter++);
                else
                    ++iter;
            } else
                ++iter;
        }
        it++;
    }
}

static void Match_element(std::pair<std::pair<int, std::string>, std::list<std::vector<std::pair<int, std::pair<std::string, int>>>>> &element) {
    KeyType key_temp;
    key_temp.id = element.first.first;
    element.first.second = quickCheck(T, key_temp);
    for(auto iter = element.second.begin(); iter != element.second.end();) {
        key_temp.id = iter->front().first;
        iter->front().second.first = quickCheck(T, key_temp);
        if(!match(element.first.second, iter->front().second.first, iter->front().second.second) || iter->front().first == element.first.first)
            (element.second).erase(iter++);
        else
            ++iter;
        }
}

static void Sort_element(std::list<std::vector<std::pair<int, std::pair<std::string, int>>>> &element) {
    bool flag = false;
    for(auto it = element.begin(); true;) {
        auto iter = it;
        iter++;
        auto iterator = iter;
        if(iter == element.end())
            break;

        if(!flag) {
            for(; iter != element.end(); iter++) 
                if(iter->size() > iterator->size())
                    iterator = iter;
            if(iterator->size() != 1 && it->size() < iterator->size()) 
                swap(*iterator, *it);
            else if(iterator->size() == 1) {
                if(it->size() != 1)
                    it++;
                flag = true;
                continue;
            }
        } else {
            for(; iter != element.end(); iter++)
                if(iter->front().second.second > iterator->front().second.second)
                    iterator = iter;
            if(it->front().second.second < iterator->front().second.second)
                swap(*iterator, *it);
        }
        it++;
    }
}

static decltype(auto) thread_deal()
{
    std::vector<std::pair<std::pair<int, std::string>, std::list<std::vector<std::pair<int, std::pair<std::string, int>>>>>> thread_vector;
    thread_vector.reserve(readlength);
    mymutex.lock();

    for(int i = 0; i < readlength; i++)
    {
        std::pair<std::pair<int, std::string>, std::list<std::vector<std::pair<int, std::pair<std::string, int>>>>> list_element;
        std::string string_temp;
        std::vector<std::pair<int, std::pair<std::string, int>>> vector_pair_temp;
        std::pair<int, std::pair<std::string, int>> pair_temp;
        getline(filein, string_temp, '-');
        list_element.first.first = stoi(string_temp);
        while(filein.peek() != '\n')
        {
            getline(filein,string_temp,',');
            pair_temp.first = stoi(string_temp);
            vector_pair_temp.emplace_back(std::move(pair_temp));
            list_element.second.push_back(std::move(vector_pair_temp));
        }
        filein.get();
        thread_vector.emplace_back(std::move(list_element));
    }
    mymutex.unlock();

    for(auto &element : thread_vector) {
        Match_element(std::ref(element));

        Multiple_Match_element(std::ref(element.second));

        Sort_element(std::ref(element.second));
    }

    return thread_vector;
}

void task() {
    T = Initialize();
    readFile("G:\\Test\\characteristic-matrix.txt",T);
    filein.open("G:\\Test\\characteristic-matrix1.txt", ios::in);
    ThreadPool pool(10);
    pool.init();
    std::vector<decltype(pool.submit(thread_deal))> resultvector;
    std::map<int, decltype(pool.submit(thread_deal).get())> mymap;
    
    for(int i = 0; i < 1000; i += readlength){
        auto result = pool.submit(thread_deal);
        resultvector.push_back(std::move(result));
    }

    for(auto &element : resultvector){
        auto result = element.get();
        mymap.insert(make_pair(result.front().first.first, result));
    }

    puts("start wirte");

    ofstream outfile1("G:\\Test\\file1.txt", ios::out | ios::app);
    ofstream outfile2("G:\\Test\\file2.txt", ios::out | ios::app);
    for(auto &element0 : mymap){
        for(auto &element1 : element0.second){
            for(auto &element2 : element1.second){
                outfile1 << element1.first.first << "-";
                for(auto &element3 : element2){
                    outfile1 << element3.first << ",";
                }
                outfile1 << endl;
                outfile2 << element1.first.second + element2.front().second.first << endl;
            }
        }
    }
    outfile1.close();
    outfile2.close();

    pool.shutdown();
    filein.close();
}

int main() {
    task();
    return 0;
}