#include "thread.h"
#include "IO.h"

#pragma warning(disable : 4996)

void CharacterExtraction(char * str,unsigned id)
{
	unsigned BraceL_1 = 0;
	unsigned BraceR_1 = 0;
	unsigned BraceL_2 = 0;
	unsigned BraceR_2 = 0;
	unsigned num = 0;
	unsigned count = 0;
	std::string res[_RES_LEN_];

	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '{' && num == 0)
		{
			BraceL_1 = i;
			num++;
		}
		else if (str[i] == '}' && num == 1)
		{
			BraceR_1 = i;
			num++;
		}
		else if (str[i] == '{' && num == 2)
		{
			BraceL_2 = i;
			num++;
		}
		else if (str[i] == '}' && num == 3)
		{
			BraceR_2 = i;

			char* temp = (char*)malloc(sizeof(char) * (BraceR_2 - BraceL_1 + 2));
			if (!temp)
			{
				std::cout << "MMO in CharacterExtraction";
				exit(1);
			}
			strncpy(temp, str + BraceL_1, BraceR_2 - BraceL_1 + 1);
			temp[BraceR_2 - BraceL_1 + 1] = '\0';

			res[count] = getBaseSequence(temp, count);//�˴����첽���ڴ˺����ڲ�free

			num = 2;
			BraceL_1 = BraceL_2;
			BraceR_1 = BraceR_2;
			count++;
		}
	}

	//���϶���Ϊ���õ����������ʽ���ַ����˺�����д��С�ļ���
	writeFile(res,id,count);
}


void ThreadProcessor(char* buf,unsigned id)
{
	//�����õ���ÿ�е��ַ�����
	char* str = (char*)malloc(sizeof(char) * BUF_SIZE);

	if (!str)
	{
		std::cout << "MMO in ThreadProcessor";
		exit(1);
	}

	memcpy(str, buf, BUF_SIZE);
	memset(buf, '\0', BUF_SIZE);

	CharacterExtraction(str,id);
	free(str);


}