#include<cstdio>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<chrono>
#include<ctime>
using namespace std;
using namespace chrono;


const int Total = 5;
const int n[5] = { 20,40,60,80,100 };

enum NodeColor
{
	RED,
	BLACK,
};

typedef struct _tmpRedBlackTreeNode {
	struct _tmpRedBlackTreeNode *p;
	struct _tmpRedBlackTreeNode *left;
	struct _tmpRedBlackTreeNode *right;

	NodeColor color;
	int key;
}RedBlackTreeNode;

class RedBlackTree {
public:
	RedBlackTreeNode *root;
	RedBlackTreeNode *nil;
};

string int_to_string(int a) {
	string res;
	stringstream ss;
	ss << a;
	ss >> res;
	return res;
}


void left_rotate(RedBlackTree &T, RedBlackTreeNode *x) {
	RedBlackTreeNode *y;
	y = x->right;
	
	x->right = y->left;			// put y.left on x.right
	if (y->left != T.nil) {
		y->left->p = x;
	}
	
	y->p = x->p;				// link x.p to y
	if (x->p == T.nil) {
		T.root = y;
	}
	else if (x == x->p->left) {
		x->p->left = y;
	}
	else {
		x->p->right = y;
	}
	
	y->left = x;				// put x on y.left
	x->p = y;
}

void right_rotate(RedBlackTree &T, RedBlackTreeNode *x) {
	RedBlackTreeNode *y;
	y = x->left;
	
	x->left = y->right;			// put y.right on x.left
	if (y->right != T.nil) {
		y->right->p = x;
	}
	
	y->p = x->p;				// link x.p to y
	if (x->p == T.nil) {
		T.root = y;
	}
	else if (x == x->p->left) {
		x->p->left = y;
	}
	else {
		x->p->right = y;
	}
	
	y->right = x;				// put x on y.right
	x->p = y;
}

void RB_insert_fixup(RedBlackTree &T, RedBlackTreeNode *z) {
	RedBlackTreeNode *y;
	while (z->p->color == RED) {
		if (z->p == z->p->p->left) {
			y = z->p->p->right;
			if (y->color == RED) {			// case 1
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if (z == z->p->right) {		// case 2
					z = z->p;
					left_rotate(T, z);
				}
				z->p->color = BLACK;		// case 3
				z->p->p->color = RED;
				right_rotate(T, z->p->p);
			}
		}	

		else {
			y = z->p->p->left;
			if (y->color == RED) {			// case 1
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if (z == z->p->left) {		// case 2
					z = z->p;
					right_rotate(T, z);
				}
				z->p->color = BLACK;		// case 3
				z->p->p->color = RED;
				left_rotate(T, z->p->p);
			}
		}
	}
	T.root->color = BLACK;
}

void RB_insert(RedBlackTree &T, RedBlackTreeNode *z) {
	RedBlackTreeNode *x, *y;
	
	y = T.nil;						// y  = x.p
	x = T.root;
	while (x != T.nil) {
		y = x;
		if (z->key < x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	z->p = y;
	if (y == T.nil) {
		T.root = z;
	}
	else if (z->key < y->key) {
		y->left = z;
	}
	else {
		y->right = z;
	}
	z->left = T.nil;
	z->right = T.nil;
	z->color = RED;
	RB_insert_fixup(T, z);
}

void RB_delete_fixup(RedBlackTree &T, RedBlackTreeNode *x) {
	RedBlackTreeNode *w;
	while (x != T.root && x->color == BLACK) {
		if (x == x->p->left) {
			w = x->p->right;
			if (w->color == RED) {											// case 1
				w->color = BLACK;
				x->p->color = RED;
				left_rotate(T, x->p);
				w = x->p->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {		// case 2
				w->color = RED;
				x = x->p;
			}
			else {
				if (w->right->color == BLACK) {								// case 3
					w->left->color = BLACK;
					w->color = RED;
					right_rotate(T, w);
					w = x->p->right;
				}
				w->color = x->p->color;										// case 4
				x->p->color = BLACK;
				w->right->color = BLACK;
				left_rotate(T, x->p);
				x = T.root;
			}
		}

		else {
			w = x->p->left;
			if (w->color == RED) {											// case 1
				w->color = BLACK;
				x->p->color = RED;
				right_rotate(T, x->p);
				w = x->p->left;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {		// case 2
				w->color = RED;
				x = x->p;
			}
			else {
				if (w->left->color == BLACK) {								// case 3
					w->right->color = BLACK;
					w->color = RED;
					left_rotate(T, w);
					w = x->p->left;
				}
				w->color = x->p->color;										// case 4
				x->p->color = BLACK;
				w->left->color = BLACK;
				right_rotate(T, x->p);
				x = T.root;
			}
		}
	}
	x->color = BLACK;
}

RedBlackTreeNode* Successor(RedBlackTree T, RedBlackTreeNode *x) {
	RedBlackTreeNode *y;
	if (x->right != T.nil) {
		x = x->right;					// bug : miss this line 
										// because this function have 1/2 possibility to run,
										// so sometimes can run correctly
		while (x->left != T.nil) {
			x = x->left;
		}
		return x;
	}
	y = x->p;
	while (y != T.nil && x == y->right) {
		x = y;
		y = y->p;
	}
	return y;
}

void RB_delete(RedBlackTree &T, RedBlackTreeNode *z) {
	RedBlackTreeNode *x, *y;
	// y is node to be deleted
	if (z->left == T.nil || z->right == T.nil) {
		y = z;
	}
	else {
		y = Successor(T, z);			// successor only has right child
	}
	// x is node to replaced y
	if (y->left != T.nil) {
		x = y->left;
	}
	else {
		x = y->right;
	}
	x->p = y->p;
	
	if (y->p == T.nil) {
		T.root = x;
	}
	else if (y == y->p->left) {
		y->p->left = x;
	}
	else {
		y->p->right = x;
	}

	if (y != z) {
		// copy y.key to z.key
		z->key = y->key;
	}
	if (y->color == BLACK) {			// replace y with x, so x.color += black
		RB_delete_fixup(T, x);
	}
	delete y;
}

RedBlackTreeNode* RB_search(RedBlackTree T, RedBlackTreeNode *node, int key) {
	if (key == node->key || node == T.nil) {
		return node;
	}
	if (key < node->key) {
		return RB_search(T, node->left, key);
	}
	else {
		return RB_search(T, node->right, key);
	}
}

ofstream Traversal;

void RB_preorder(RedBlackTree T, RedBlackTreeNode *x) {
	if (x == T.nil) {
		return;
	}
	Traversal << x->key << (x->color ? "B" : "R") << endl;
	RB_preorder(T, x->left);
	RB_preorder(T, x->right);
}

void RB_inorder(RedBlackTree T, RedBlackTreeNode *x) {
	if (x == T.nil) {
		return;
	}
	RB_inorder(T, x->left);
	Traversal << x->key << (x->color ? "B" : "R") << endl;
	RB_inorder(T, x->right);
}

void RB_postorder(RedBlackTree T, RedBlackTreeNode *x) {
	if (x == T.nil) {
		return;
	}
	RB_postorder(T, x->left);
	RB_postorder(T, x->right);
	Traversal << x->key << (x->color ? "B" : "R") << endl;
}


string OutPutPath = "../outputA/size";

void RBT_run(RedBlackTree T, int input[], int n) {
	ofstream time1;
	time1.open(OutPutPath + int_to_string(n) + "/time1.txt");
	//OutPutRuntime << "n = " << n << endl;
	cout << "n = " << n << endl;

	/* insert and runtime */
	int64_t runtime = 0;
	auto start = steady_clock::now();
	for (int i = 0; i < n; i++) {
		RedBlackTreeNode *z;
		z = new RedBlackTreeNode();
		z->key = input[i];
		RB_insert(T, z);
		
		if ((i+1) % 10 == 0){
			auto end = steady_clock::now();
			auto duration = duration_cast<nanoseconds>(end - start);
			runtime = duration.count();
			cout << "插入" << i + 1 << "个节点用时:" << runtime << "ns" << endl;
			time1 << "插入" << i + 1 << "个节点用时:" << runtime << "ns" << endl;
		}
	}
	time1.close();

	/* traversal */
	if (T.root == T.nil) {
		cout << "root nil" << endl;
	}
	Traversal.open(OutPutPath + int_to_string(n) + "/preorder.txt");
	RB_preorder(T, T.root);
	Traversal.close();
	Traversal.open(OutPutPath + int_to_string(n) + "/inorder.txt");
	RB_inorder(T, T.root);
	Traversal.close();
	Traversal.open(OutPutPath + int_to_string(n) + "/postorder.txt");
	RB_postorder(T, T.root);
	Traversal.close();

	/* delete and runtime */
	// randomly select n/10 keys 
	srand(unsigned(time(0)));
	int keytodel[11];
	int randindex;
	bool his[101] = { false };			// random index history
	for (int i = 0; i < n / 10; i++) {
		randindex = rand() % n;
		while (his[randindex] == true) {
			randindex = rand() % n;
		}
		keytodel[i] = input[randindex];
		his[randindex] = true;
	}

	/* debug: replay bug */
	/*keytodel[0] = 789;
	keytodel[1] = 18015;
	keytodel[2] = 43301;
	keytodel[3] = 58577;
	keytodel[4] = 50903;
	keytodel[5] = 11378;
	keytodel[6] = 59036;
	keytodel[7] = 9528;*/



	ofstream time2;
	time2.open(OutPutPath + int_to_string(n) + "/time2.txt");
	
	RedBlackTreeNode *nodetodel;
	start = steady_clock::now();
	for (int i = 0; i < n / 10; i++) {
		nodetodel = RB_search(T, T.root, keytodel[i]);
		if (nodetodel == NULL) {
			cout << "node to delete is NULL" << endl;
		}
		else if (nodetodel == T.nil) {
			cout << "node to delete is T.NIL" << endl;
		}
		else {
			RB_delete(T, nodetodel);
		}

		auto end = steady_clock::now();
		auto duration = duration_cast<nanoseconds>(end - start);
		runtime = duration.count();
		cout << "删除" << i + 1 << "个节点用时:" << runtime << "ns" << endl;
		time2 << "删除" << i + 1 << "个节点用时:" << runtime << "ns" << endl;
	}

	/* after delete n/10 nodes */
	ofstream deldata;
	deldata.open(OutPutPath + int_to_string(n) + "/delete_data.txt");
	deldata << "删除的关键字为:" << endl;
	cout << "删除的关键字为:" << endl;
	for (int i = 0; i < n / 10; i++) {
		deldata << keytodel[i] << endl;
		cout << keytodel[i] << endl;
	}
	deldata << endl;
	cout << endl;
	// inorder traversal
	deldata << "删除完成后中序遍历为:" << endl;
	Traversal.open(OutPutPath + int_to_string(n) + "/delete_data.txt", ios::app);
	RB_inorder(T, T.root);
	Traversal.close();
	deldata.close();
}

void RBT_destory(RedBlackTree T, RedBlackTreeNode *x) {
	if (x == T.nil) {
		return;
	}
	RBT_destory(T, x->left);
	RBT_destory(T, x->right);
	delete x;
}

int main()
{
	ifstream inputA;
	int input[101];
	inputA.open("../inputA/inputA.txt");
	for (int i = 0; i < 100; i++) {
		inputA >> input[i];
	}
	inputA.close();

	RedBlackTree T;
	T.root = NULL;
	T.nil = new RedBlackTreeNode();
	T.nil->color = BLACK;
	T.nil->p = NULL;
	T.nil->left = NULL;
	T.nil->right = NULL;

	T.root = T.nil;
	T.root->p = T.nil;
	T.root->left = T.nil;
	T.root->right = T.nil;
	
	for (int i = 0; i < Total; i++) {

		/* debug: replay captured bug */
		//RBT_run(T, input, 80);

		RBT_run(T, input, n[i]);
		
		RBT_destory(T, T.root);
		T.root = T.nil;
		T.root->p = T.nil;
		T.root->left = T.nil;
		T.root->right = T.nil;
	}
	//system("pause");

    return 0;
}
