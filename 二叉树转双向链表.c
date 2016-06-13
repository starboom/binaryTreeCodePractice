/*题目描述

输入一棵二叉搜索树，
将该二叉搜索树转换成一个排序的双向链表。
要求不能创建任何新的结点，只能调整树中结点指针的指向。
struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
	TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
	}
};*/
class Solution {
public:
	void ConvertNode(TreeNode* pNode,TreeNode **pLastNode){
		if(pNode == NULL) return;
		TreeNode* pCurrent = pNode;
		if(pCurrent -> left != NULL){
			ConvertNode(pCurrent -> left,pLastNode);
		}
		pCurrent -> left = *pLastNode;
		if(*pLastNode != NULL){
			(*pLastNode) -> right = pCurrent;
		}
		*pLastNode = pCurrent;
		if(pCurrent -> right != NULL){
			ConvertNode(pCurrent -> righ,pLastNode);
		}

	}
    TreeNode* Convert(TreeNode* pRootOfTree){
    		TreeNode *pLastNodeInList = NULL;
    		ConvertNode(pRootOfTree,*pLastNodeInList);
    		TreeNode *pHeadOfList = pLastNodeInList;
    		while(pHeadOfList != NULL && pHeadOfList -> left != NULL){
    			pHeadOfList = pHeadOfList -> left;
    		}
    		return pHeadOfList;
    }
};