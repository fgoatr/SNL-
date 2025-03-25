#pragma once
#include"total.h"
#include"TreeMoke.h"


void printTree(TreeNode* t, int layer, bool islastson, char out[100])
{
	if (t == NULL)
		return;// 递归出口

	int last = 31; //这段代码的目的是找到当前结点最后一个儿子的index
	while (last >= 0)
	{
		if (t->child[last] != NULL)
			break;
		// 找到最后一个儿子节点
		last--;
	}
	// 如果没有儿子 last == -1
	// 否则 last 记录最后一个儿子节点的index


	for (int i = 0; i < layer; i++) //根据层数打印空格
	{
		outstr[strline] += "   ";
		//printf("   ");
	}

	if (layer == 0) //特殊情况 根节点的显示
	{
		outstr[strline] += "   ";
		outstr[strline] += t->name;
		//cout << t->name;
	}
	else //不是根节点的时候
	{
		if (islastson == true) // 如果当前节点是父节点的最后一个儿子节点
		{
			outstr[strline] += "└ "; // 则它前面的树枝会有些区别
			outstr[strline] += t->name;
			//cout << "└ " << t->name;
		}
		else// 不是父节点的最后一个儿子节点
		{
			outstr[strline] += "├ ";
			outstr[strline] += t->name;
			//cout << "├ " << t->name;
		}
	}

	outstr[strline] += "\n"; //换行
	strline++;
	//outstr += "\n";
	//cout << endl;

	for (int i = 0; i < 10; i++) //递归
	{
		if (i != last) // 如果不是儿子节点 那么第三个参数就标成false
			printTree(t->child[i], layer + 1, false, out);
		else
			printTree(t->child[i], layer + 1, true, out);
	}

	if (layer == 0) // 在整个递归过程结束退出之前进行打印
	{
		// 首先对字符串进行修改
		// 这里修改的目的是让树枝能连起来
		// 底下这段代码的功能比较难以描述了
		// 你可以注释掉一些部分自己看一看会对打印的树有什么影响
		///*
		for (int i = 1; i < strline; i++)
		{
			int j = 0;
			while (outstr[i][j] != '\n')
			{
				if (outstr[i - 1][j] == '\n')
					break;
				if (outstr[i - 1][j] == ch2[0] && outstr[i - 1][j + 1] == ch2[1]) // 这个很重要 不加的话会出问题
				{
					; // 上一行是"└"
				}
				else if (outstr[i - 1][j] == ch1[0] && outstr[i - 1][j + 1] == ch1[1] && outstr[i][j] == ' ')
				{
					// 上一行是"├" 这一行是空格
					outstr[i][j] = ch3[0];
					outstr[i][j + 1] = ch3[1];
				}
				else if (outstr[i - 1][j] == ch3[0] && outstr[i][j] == ' ')
				{
					// 上一行是"│" 这一行是空格
					outstr[i][j] = ch3[0];
					outstr[i][j + 1] = ch3[1];
				}
				j++;
			}
		}
		//*/

		// 修改完之后进行打印
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

