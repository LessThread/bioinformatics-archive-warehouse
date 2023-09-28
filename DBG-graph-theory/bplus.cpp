#include "bplus.h"



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

char* quickCheck(BPlusTree T, KeyType key)
{

    if (T == NULL)
    {
        char a[] = "No";
        /*std::cout << "No" << std::endl;*/
        return a;
    }
    int j = 0;

    while (j < T->KeyNum && (key.id >= T->Key[j].id))
    {
        if (key.id == T->Key[j].id)
        {
            //std::cout << T->Key[j].xulie << std::endl;
            return T->Key[j].xulie;
        }
        j++;
    }
    j--;

    if (key.id == T->Key[j].id)
        return T->Key[j].xulie;
    //std::cout << T->Key[j].xulie << std::endl;
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
        //�Ʊ�� tab�����ţ��ֺŶ�������Ч�ķָ�����ͳһת�ɿո�
        //Ϊ�˱�����󣬻س����ͻ��з�ҲתΪ�ո񣨷����޷�������У�
        if (c == '\t' || c == ',' || c == ';' || c == '\r' || c == '\n')
            c = ' ';
    }

    line.erase(0, line.find_first_not_of(" "));//ɾ�����׿ո�
    line.erase(line.find_last_not_of(" ") + 1);//ɾ����ĩ�ո�
}

void readFile(std::string file, BPlusTree& T, std::map<std::string, long long>& maps)
{
    unsigned num = 0;
    std::ifstream input(file);
    if (input.fail())
    {
        std::cout << "Not exist file !" << std::endl;
        return;
    }
    else
    {
        long long n = 0;

        std::string b1, b2;
        float x1 = 0, x2 = 0, x3 = 0, x4 = 0;
        while (getline(input, b1))
        {
            line_process(b1);
            if (b1.empty()) continue;
            getline(input, b2);

            /*input >> x1;
            input >> x2;
            input >> x3;
            input >> x4;*/

            KeyType key = Unavailable;
            strcpy_s(key.xulie, strlen(b2.c_str()) + 1, b2.c_str());
            //key.id = std::stoi(b1.c_str());
            key.id = num;
            T = Insert(T, key);

            maps.insert(std::pair<std::string, long long>(b1, num));

            num++;

           /* std::cout
                << "b1 :" << b1 << "\n"
                << "b2 :" << b2 << "\n";
            ////system("pause");*/

            //if (n++ == 400)break;

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



//API.....................

BPlusTree* T_init(std::string p, std::map<std::string, long long>&maps)
{
    static BPlusTree T;
    T = Initialize();
    std::string path = p;
    readFile((std::string)path, T,maps);
    return &T;
    //Ϊɶ��ָ�������⣿ԭ���Ǿ�̬����
}

std::string findSeq(unsigned id, BPlusTree* T)
{
    KeyType X;
    X.id=id;
    std::string ans=quickCheck(*T, X);
    return ans;
}