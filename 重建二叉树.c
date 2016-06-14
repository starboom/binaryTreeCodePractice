/**
输入某二叉树的前序遍历和中序遍历的结果，
请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    struct TreeNode* reConstructBinaryTree(vector<int> pre,vector<int> in) {
    		if(pre.empty() ==  true || in.empty() == true){
    			return NULL;
    		}
    	//	int length = pre.size();
        vector<int> leftPre,leftIn,rightPre,rightIn;
    	int rootValue = pre[0]; // 确定根节点的值
    	/*******初始化根节点*********/
    	struct TreeNode* root = new TreeNode(pre[0]);
    	/*******初始化结束*************/
    	if(pre.size() == 1){
    		if(in.size() == 1 && pre[0] == in[0]){
    			return root;
    		}
    	//	else
    	//		throw std :: exception("Invalid input");
    	}//判断是否有违法输入,保持代码鲁棒性
	int rootIndex{};
    	//在中序中找到根节点的位置
    	int i;
    	for(i = 0;i < in.size();++i){
    		if(in[i] == rootValue){
    			rootIndex = i;
    			break;
    		}
    	}
    	//找到根节点位置之后.可以确定左子树
    	int leftLength = rootIndex;

    	for(i = 0;i < leftLength;i++){
    		leftPre.push_back(pre[i+1]);
    		leftIn.push_back(in[i]);
    	}
    	for(i = leftLength+1;i < in.size();i++){
    		rightPre.push_back(pre[i]);
    		rightIn.push_back(in[i]);
    	}
    	root -> left = reConstructBinaryTree(leftPre,leftIn); 
    	root -> right = reConstructBinaryTree(rightPre,rightIn);
    

    	return root;


    }
};
