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

typedef struct _interval{
	int low;
	int high;
}RB_interval;

typedef struct _tmpRedBlackTreeNode {
	struct _tmpRedBlackTreeNode *p;
	struct _tmpRedBlackTreeNode *left;
	struct _tmpRedBlackTreeNode *right;

	NodeColor color;
	int max;
	RB_interval interval;
}RedBlackTreeNode;

class RedBlackTree {
public:
	RedBlackTreeNode * root;
	RedBlackTreeNode *nil;
};

string int_to_string(int a) {
	string res;
	stringstream ss;
	ss << a;
	ss >> res;
	return res;
}

int max(int a, int b, int c) {
	if (a > b) {
		return a > c ? a : c;
	}
	else {
		return b > c ? b : c;
	}
}
int max(int a, int b) {
	return a > b ? a : b;
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

	// update max
	y->max = x->max;
	x->max = max(
		x->interval.high,
		(x->left != T.nil) ? x->left->max : 0, 
		(x->right != T.nil) ? x->right->max : 0
	);
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

	// update max
	y->max = x->max;
	x->max = max(
		x->interval.high,
		(x->left != T.nil) ? x->left->max : 0,
		(x->right != T.nil) ? x->right->max : 0
	);
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
		
		// update max alone T.root to insert location
		x->max = max(x->max, z->max);

		y = x;
		if (z->interval.low < x->interval.low) {
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
	else if (z->interval.low < y->interval.low) {
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
	
	// replace y with x
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
		// copy y.interval to z.interval
		/*z->interval.low = y->interval.low;
		z->interval.high = y->interval.high;*/
		z->interval = y->interval; 
	}

	// update max alone y.p .. to T.root
	z = y->p;
	while (z != T.nil) {
		z->max = max(
			z->max, 
			(z->left != T.nil) ? z->left->max : 0, 
			(z->right != T.nil) ? z->right->max : 0
		);
		z = z->p;
	}

	// replace y with x, so x.color += black
	if (y->color == BLACK) {			
		RB_delete_fixup(T, x);
	}
	delete y;
}

RedBlackTreeNode* RB_search_exact(RedBlackTree T, RedBlackTreeNode *node, int low) {
	if (low == node->interval.low || node == T.nil) {
		return node;
	}
	if (low < node->interval.low) {
		return RB_search_exact(T, node->left, low);
	}
	else {
		return RB_search_exact(T, node->right, low);
	}
}

ofstream Traversal;

/*void RB_preorder(RedBlackTree T, RedBlackTreeNode *x) {
	if (x == T.nil) {
		return;
	}
	Traversal << x->interval.low << (x->color ? "B" : "R") << endl;
	RB_preorder(T, x->left);
	RB_preorder(T, x->right);
}*/

void RB_inorder(RedBlackTree T, RedBlackTreeNode *x) {
	if (x == T.nil) {
		return;
	}
	RB_inorder(T, x->left);
	Traversal << x->interval.low << ' ' << x->interval.high << ' ' << x->max << ' ' << (x->color ? "B" : "R") << endl;
	RB_inorder(T, x->right);
}

/*void RB_postorder(RedBlackTree T, RedBlackTreeNode *x) {
	if (x == T.nil) {
		return;
	}
	RB_postorder(T, x->left);
	RB_postorder(T, x->right);
	Traversal << x->interval.low << (x->color ? "B" : "R") << endl;
}*/


bool overlap(RB_interval a, RB_interval b) {
	if (a.high < b.low || a.low > b.high) {
		return false;
	}
	else return true;
}

RedBlackTreeNode* IntervalSearch(RedBlackTree T, RB_interval i) {
	RedBlackTreeNode *x;
	x = T.root;
	while (x != T.nil && !overlap(x->interval, i)) {
		if (x->left != T.nil && x->left->max >= i.low) {
			x = x->left;
		}
		else x = x->right;
	}
	return x;
}



string OutPutPath = "../outputB";

void RBT_run(RedBlackTree T, int input[], int n) {
	cout << "n = " << n << endl;

	/* insert and runtime */
	int64_t runtime = 0;
	auto start = steady_clock::now();
	for (int i = 0; i < n; i++) {
		RedBlackTreeNode *z;
		z = new RedBlackTreeNode();
		z->interval.low = input[i * 2];
		z->interval.high = input[i * 2 + 1];
		z->max = z->interval.high;
		RB_insert(T, z);

		if ((i + 1) % 10 == 0) {
			auto end = steady_clock::now();
			auto duration = duration_cast<nanoseconds>(end - start);
			runtime = duration.count();
			cout << "插入" << i + 1 << "个节点用时:" << runtime << "ns" << endl;
		}
	}

	/* traversal */
	if (T.root == T.nil) {
		cout << "tree is empty, root is nil" << endl;
	}
	else {
		/*Traversal.open(OutPutPath + int_to_string(n) + "/preorder.txt");
		RB_preorder(T, T.root);
		Traversal.close();*/

		Traversal.open(OutPutPath + "/inorder.txt");
		RB_inorder(T, T.root);
		Traversal.close();

		/*Traversal.open(OutPutPath + int_to_string(n) + "/postorder.txt");
		RB_postorder(T, T.root);
		Traversal.close();*/
	}


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
		keytodel[i] = input[randindex * 2];		// index of low is index*2
		his[randindex] = true;
	}


	RedBlackTreeNode *nodetodel;
	start = steady_clock::now();
	for (int i = 0; i < n / 10; i++) {
		nodetodel = RB_search_exact(T, T.root, keytodel[i]);
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
	}

	/* after delete n/10 nodes */
	ofstream deldata;
	deldata.open(OutPutPath + "/delete_data.txt");
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
	Traversal.open(OutPutPath + "/delete_data.txt", ios::app);
	RB_inorder(T, T.root);
	Traversal.close();
	deldata.close();


	/* search */
	cout << "搜索:" << endl;
	ofstream OutPutSearch;
	OutPutSearch.open(OutPutPath + "/search.txt");

	RedBlackTreeNode *nodetosearch;
	RB_interval in;
	
	for (int i = 0; i < 2; i++) {
		randindex = rand() % n;
		while (his[randindex] == true) {
			randindex = rand() % n;
		}
		
		in.low = input[randindex * 2];		// index of low is index*2
		in.high = input[randindex * 2 + 1];
		nodetosearch = IntervalSearch(T, in);
		if (nodetosearch == T.nil) {
			cout << "search nil" << endl;
		}
		else {
			cout << nodetosearch->interval.low << ' ' << nodetosearch->interval.high << endl;
		}
		his[randindex] = true;
	}
	// search failed
	// random interval in [26, 29]
	in.low = 26 + rand() % 3;
	in.high = in.low + 1 + rand() % (29 - in.low);
	cout << "search failed:" << in.low << ' ' << in.high << endl;
	nodetosearch = IntervalSearch(T, in);
	if (nodetosearch == T.nil) {
		cout << "search return T.nil" << endl;
	}
	else {
		cout << nodetosearch->interval.low << ' ' << nodetosearch->interval.high << endl;
	}
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
	inputA.open("../inputB/inputB.txt");
	for (int i = 0; i < 60; i++) {
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

	
	RBT_run(T, input, 30);

	RBT_destory(T, T.root);
	
	//system("pause");

	return 0;
}
