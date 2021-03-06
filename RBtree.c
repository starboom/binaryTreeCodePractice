#include <stdio.h>
/*
性质1. 节点是红色或黑色。
性质2. 根是黑色。
性质3. 所有叶子都是黑色（叶子是NIL节点）。
性质4. 每个红色节点的两个子节点都是黑色。(从每个叶子到根的所有路径上不能有两个连续的红色节点)
性质5. 从任一节点到其每个叶子的所有简单路径都包含相同数目的黑色节点。
*/
typedef usigned long rbtree_key_t;
typedef long		 rbtree_key_int_t;
typedef usigned char color_t;
typedef void(*rbtree_insert_pt)(rbtree_node_t *root,rbtree_node_t *node,
	rbtree_node_t *sentinel);
typedef struct rbtree_node_t{
	rbtree_key_t key;
	rbtree_node_t *left;
	rbtree_node_t *right;
	rbtree_node_t *parent;
	color_t	color; //1 红 0 黑  
}rbtree_node_t;

typedef struct rbtree_t{
	rbtree_node_t *root;
	rbtree_node_t *sentinel; //哨兵
	rbtree_insert_pt insert;
}rbtree_t;

/*void rbtree_left_rotate(rbtree_node_t* node_x){
	rbtree_node_t *node_y;
	rbtree_node_t **root = 
}*/
/*
哨兵节点必须是黑色的不同于一般二叉查找树，
红黑树一般采用一个哨兵结点代表NIL，
这对算法的使用提供了很多方便，
具体编写时可以体会的到。哨兵设置为黑色，
它是根的父结点，也是所有的叶子结点。
而它的其他域可以设置为任意值。
我用关键字把它和普通的结点进行区分。
*/
/*rbtree的属性定义宏函数*/
#define rbtree_sentinel_init(node) rbt_black(node)//哨兵必须是黑色节点 

#define rbt_red(node)	((node) -> color = 1)//设置节点为红
#define rbt_black(node) ((node) -> color = 0)//设置节点为黑
#define rbt_is_red(node)	((node) -> color)//判断节点是否为红
#define rbt_is_black(node)	(!rbt_is_red(node))//判断节点是否为黑
#define rbt_copy_color(n1,n2) (n1 -> color = n2 -> color)//复制节点颜色


#define rbtree_init(tree,s,i)			\
	rbtree_sentinel_init(s);			\
	(tree) -> root = s;					\
	(tree) -> sentinel = s;				\
	(tree) -> insert = i;				\//初始化rbtree
/***************宏函数定义结束*******************************/
rbtree_node_t* rbtree_min(rbtree_node_t *node,rbtree_node_t *sentinel){
	while(node -> left != sentinel){
		node = node -> left;
	}
	return node;
}
void rbtree_insert_value(rbtree_node_t *temp,rbtree_node_t *node,rbtree_node_t *sentinel){
	rbtree_node_t **p;
	for(;;){
		p = (node -> key < temp -> key)?&temp -> left : &temp -> right;
		if(*p == sentinel){
			break;
		}
		temp = *p;
	}
	*p = node;
	node -> parent = temp;
	node -> left = sentinel;
	node -> right = sentinel;
	rbt_red(node);
}
void rbtree_insert(volatile rbtree_t *tree,rbtree_node_t *node){
	rbtree_node_t **root,*temp,*sentinel;

	root = (rbtree_node_t **)&tree -> root;
	sentinel = tree -> sentinel;
	if(*root == sentinel){
		node -> parent = NULL;
		node -> left = sentinel;
		node -> right = sentinel;
		rbt_black(node);
		*root = node;
		return;
	}
	tree -> insert(*root,node,sentinel);
	/*re_balance tree*/
	while(node != *root && rbt_is_red(node -> parent)){
		if(node -> parent == node -> parent -> parent -> left){
			temp = node -> parent -> parent -> right;
			if(rbt_is_red(temp)){
				rbt_black(node -> parent);
				rbt_black(temp);
				rbt_red(node -> parent -> parent);
				node = node -> parent -> parent;
			}else{
				if(node == node -> parent -> right){
					node = node -> parent;
					rbtree_left_rotate(root,sentinel,node);
				}
				rbt_black(node -> parent);
				rbt_red(node -> parent -> parent);
				rbtree_right_rotate(root,sentinel,node -> parent -> parent);
			}
		}else{
			temp = node -> parent -> parent -> left;
			if(rbt_is_red(temp)){
				rbt_black(node -> parent);
				rbt_black(temp);
				rbt_red(node -> parent -> parent);
				node = node -> parent -> parent;
			}else{
				if(node == node -> parent -> left){
					node = node -> parent;
					rbtree_right_rotate(root,sentinel,node);
				}
				rbt_black(node -> parent);
				rbt_red(node -> parent -> parent);
				rbtree_left_rotate(root,sentinel,node -> parent -> parent);
			}
		}
	}
	rbt_black(*root);
}
void rbtree_delete(volatile rbtree_t *tree,rbtree_node_t *node){
	color_t red;
	rbtree_node_t **root,*sentinel,*subst,*temp,*w;
	/*a binary tree delete*/
	root = (rbtree_node_t **)&tree -> root;
	sentinel = tree -> sentinel;
	if(node -> left == sentinel){
		temp = node -> right;
		subst = node;
	}
	else if(node -> right == sentinel){
		temp = node -> left;
		subst = node;
	}else{
		subst = rbtree_min(node -> right,sentinel);
		if(subst -> left != sentinel){
			temp = subst -> left;
		}else{
			temp = subst -> right;
		}
	}
	if(subst == *root){
		*root = temp;
		rbt_black(temp);
		node -> left = NULL;
		node -> right = NULL;
		node -> parent = NULL;
		node -> key = 0;

		return;
	}
	red = rbt_is_red(subst);
	if(subst == subst -> parent -> left){
		subst -> parent -> left = temp;
	}else{
		subst -> parent -> right = temp;
	}
	if(subst == node){
		temp -> parent = subst -> parent;
	}else{
		if(subst -> parent == node){
			temp -> parent = subst;
		}else{
			temp -> parent = subst -> parent;
		}
		subst -> left = node -> left;
		subst -> right = node -> right;
		subst -> parent = node -> parent;
		rbt_copy_color(subst,node);
		if(node == *root){
			*root = subst;
		}else{
			if(node == node -> parent -> left){
				node -> parent -> left = subst;
			}else{
				node -> parent -> right = subst;
			}
		}
		if(subst -> left != sentinel){
			subst -> left -> parent = subst;
		}
		if(subst -> right != sentinel){
			subst -> right -> parent = subst;
		}
	}
	node -> left = NULL;
	node -> right = NULL;
	node -> parent = NULL;
	node -> key = NULL;
	if(red){
		return;
	}
	/*a delete fixup*/
	while(temp != *root && rbt_is_black(temp)){
		if(temp == temp -> parent -> left){
			w = temp -> parent -> right;
			if(rbt_is_red(w)){
				rbt_black(w);
				rbt_red(temp -> parent);
				rbtree_left_rotate(root,sentinel,temp -> parent);
				w = temp -> parent -> right;
			}
			if(rbt_is_black(w -> left) && rbt_is_black(w -> right)){
				rbt_red(w);
				temp = temp -> parent;
			}else{
				if(rbt_is_black(w -> right)){
					rbt_black(w -> left);
					rbt_red(w);
					rbtree_right_rotate(root,sentinel,w);
					w = temp -> parent -> right;
				}
				rbt_copy_color(w,temp -> parent);
				rbt_black(temp -> parent);
				rbt_black(w -> right);
				rbtree_left_rotate(root,sentinel,temp -> parent);
				temp = *root;
			}
		}else{
			w = temp -> parent -> left;
			if(rbt_is_red(w)){
				rbt_black(w);
				rbt_red(temp -> parent);
				rbtree_right_rotate(root,sentinel,temp -> parent);
				w = temp -> parent -> left;
			}
			if(rbt_is_black(w -> left) && rbt_is_black(w -> right)){
				rbt_red(w);
				temp = temp -> parent;
			}else{
				if(rbt_is_black(w -> left)){
					rbt_black(w -> right);
					rbt_red(w);
					rbtree_right_rotate(root,sentinel,w);
					w = temp -> parent -> left;
				}
				rbt_copy_color(w,temp -> parent);
				rbt_black(temp -> parent);
				rbt_black(w -> left);
				rbtree_right_rotate(root,sentinel,temp -> parent);
				temp = *root;

			}
		}
	}
	rbt_black(temp);
}



int main(int argc,char const *argv[])
{

	return 0;
}