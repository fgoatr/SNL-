#pragma once
#include"total.h"
#include"TreeMoke.h"
#include"treewrite.h"

TreeNode* STEP2()
{
	TreeNode* head = program();
	printTree(head, 0, false, OUT2a);
	return head;
}