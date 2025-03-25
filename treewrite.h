#pragma once
#include"total.h"
#include"TreeMoke.h"


void printTree(TreeNode* t, int layer, bool islastson, char out[100])
{
	if (t == NULL)
		return;// �ݹ����

	int last = 31; //��δ����Ŀ�����ҵ���ǰ������һ�����ӵ�index
	while (last >= 0)
	{
		if (t->child[last] != NULL)
			break;
		// �ҵ����һ�����ӽڵ�
		last--;
	}
	// ���û�ж��� last == -1
	// ���� last ��¼���һ�����ӽڵ��index


	for (int i = 0; i < layer; i++) //���ݲ�����ӡ�ո�
	{
		outstr[strline] += "   ";
		//printf("   ");
	}

	if (layer == 0) //������� ���ڵ����ʾ
	{
		outstr[strline] += "   ";
		outstr[strline] += t->name;
		//cout << t->name;
	}
	else //���Ǹ��ڵ��ʱ��
	{
		if (islastson == true) // �����ǰ�ڵ��Ǹ��ڵ�����һ�����ӽڵ�
		{
			outstr[strline] += "�� "; // ����ǰ�����֦����Щ����
			outstr[strline] += t->name;
			//cout << "�� " << t->name;
		}
		else// ���Ǹ��ڵ�����һ�����ӽڵ�
		{
			outstr[strline] += "�� ";
			outstr[strline] += t->name;
			//cout << "�� " << t->name;
		}
	}

	outstr[strline] += "\n"; //����
	strline++;
	//outstr += "\n";
	//cout << endl;

	for (int i = 0; i < 10; i++) //�ݹ�
	{
		if (i != last) // ������Ƕ��ӽڵ� ��ô�����������ͱ��false
			printTree(t->child[i], layer + 1, false, out);
		else
			printTree(t->child[i], layer + 1, true, out);
	}

	if (layer == 0) // �������ݹ���̽����˳�֮ǰ���д�ӡ
	{
		// ���ȶ��ַ��������޸�
		// �����޸ĵ�Ŀ��������֦��������
		// ������δ���Ĺ��ܱȽ�����������
		// �����ע�͵�һЩ�����Լ���һ����Դ�ӡ������ʲôӰ��
		///*
		for (int i = 1; i < strline; i++)
		{
			int j = 0;
			while (outstr[i][j] != '\n')
			{
				if (outstr[i - 1][j] == '\n')
					break;
				if (outstr[i - 1][j] == ch2[0] && outstr[i - 1][j + 1] == ch2[1]) // �������Ҫ ���ӵĻ��������
				{
					; // ��һ����"��"
				}
				else if (outstr[i - 1][j] == ch1[0] && outstr[i - 1][j + 1] == ch1[1] && outstr[i][j] == ' ')
				{
					// ��һ����"��" ��һ���ǿո�
					outstr[i][j] = ch3[0];
					outstr[i][j + 1] = ch3[1];
				}
				else if (outstr[i - 1][j] == ch3[0] && outstr[i][j] == ' ')
				{
					// ��һ����"��" ��һ���ǿո�
					outstr[i][j] = ch3[0];
					outstr[i][j + 1] = ch3[1];
				}
				j++;
			}
		}
		//*/

		// �޸���֮����д�ӡ
		ofstream output(out);
		for (int i = 0; i < strline; i++)
		{
			output << outstr[i];
		}
		output.close();
		for (int i = 0; i < strline; i++)
		{
			cout << outstr[i];
		}
	}
}

