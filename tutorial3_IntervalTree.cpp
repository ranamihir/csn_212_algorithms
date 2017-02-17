#include <bits/stdc++.h>
using namespace std;

// Define RED and BLACK characteristics
#define RED		1
#define BLACK	2

// Interval Structure
struct Interval{
    int low;
	int high;
};

// Interval Tree Node Structure
struct node {
	Interval interval;
	struct node *left, *right, *p;
	int max;
	int color;
};

typedef struct node *NODEPTR;
struct node NIL;
NODEPTR NILPTR = &NIL;


// Prototypes of functions required
NODEPTR maximum(NODEPTR);
NODEPTR minimum(NODEPTR);
NODEPTR search(NODEPTR, Interval);
void leftRotate(NODEPTR *, NODEPTR);
void rightRotate(NODEPTR *, NODEPTR);
void rbInsertFixup(NODEPTR *, NODEPTR);
void rbInsert(NODEPTR *, Interval);
void rbTransplant(NODEPTR *, NODEPTR, NODEPTR);
void rbDeleteFixup(NODEPTR *, NODEPTR);
void rbDelete(NODEPTR *, Interval);
bool checkOverlap(Interval, Interval);
Interval overlapSearch(NODEPTR, Interval);


// Driver function
int main() {
	NIL.left = NIL.right = NIL.p = NILPTR;
	NIL.color = BLACK;
	NODEPTR tree = NILPTR;
	
	Interval intervals[] = {{15, 20}, {10, 30}, {17, 19}, {5, 20}, {12, 15}, {30, 40}};
    
    int n = sizeof(intervals)/sizeof(intervals[0]), i;
    
    for (i = 0; i < n; i++) {
         rbInsert(&tree, intervals[i]);
         cout << "Interval inserted: {" << intervals[i].low << ", " << intervals[i].high << "}" << endl;
    }
 
  
    Interval I = {14, 16};
 
    cout << "\nSearching for interval: {" << I.low << ", " << I.high << "}" << endl;
    Interval result = overlapSearch(tree, I);
    if (result.low == INT_MIN)
        cout << "No Overlapping Interval." << endl;
    else
        cout << "Interval overlaps with {" << result.low << ", " << result.high << "}." << endl;
        
        
    I = {21, 23};
 
    cout << "\nSearching for interval: {" << I.low << ", " << I.high << "}" << endl;
    result = overlapSearch(tree, I);
    if (result.low == INT_MIN)
        cout << "No Overlapping Interval." << endl;
    else
        cout << "Interval overlaps with {" << result.low << ", " << result.high << "}." << endl;
    
	return 0;
}

// Function for finding maximum in a tree
NODEPTR maximum(NODEPTR root) {
	while(root->right != NILPTR)
		root = root->right;
	return root;
}

// Function for finding minimum in a tree
NODEPTR minimum(NODEPTR root) {
	while(root->left != NILPTR)
		root = root->left;
	return root;
}

// Function for searching for a node in a tree
NODEPTR search(NODEPTR root, Interval I) {
	if (root == NILPTR || (root->interval.low == I.low && root->interval.high == I.high))
		return root;
	if (I.low < root->interval.low)
		return search(root->left, I);
	else
		return search(root->right, I);
}

// Function for left-rotating about a pivot node x
void leftRotate(NODEPTR *root, NODEPTR x) {
	NODEPTR y = x->right;
	x->right = y->left;
	if (y->left != NILPTR)
		y->left->p = x;
	y->p = x->p;
	if (x->p == NILPTR)
		*root = y;
	else if (x->p->left == x)
		x->p->left = y;
	else
		x->p->right = y;
	y->left = x;
	x->p = y;
	
	x->max = max(x->left->max , max(x->right->max , x->interval.high));
    y->max = max(x->max , max(y->right->max , y->interval.high));
}

// Function for right-rotating about a pivot node y
void rightRotate(NODEPTR *root, NODEPTR y) {
	NODEPTR x = y->left;
	y->left = x->right;
	if (x->right != NILPTR)
		x->right->p = y;
	x->p = y->p;
	if (y->p == NILPTR)
		*root = x;
	else if (y->p->left == y)
		y->p->left = x;
	else
		y->p->right = x;
	x->right = y;
	y->p = x;
	
	y->max = max(y->left->max , max(y->right->max , y->interval.high));
  	x->max = max(x->left->max , max(y->max , x->interval.high));
}

// Function for fixing red-black properties in a tree after insertion
void rbInsertFixup(NODEPTR *root, NODEPTR z) {
	while(z->p->color == RED) {
		if (z->p == z->p->p->left) {
			NODEPTR y = z->p->p->right;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if (z == z->p->right) {
					z = z->p;
					leftRotate(root,z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				rightRotate(root,z->p->p);
			}
		}
		else {
			NODEPTR y = z->p->p->left;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if (z == z->p->left) {
					z = z->p;
					rightRotate(root,z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				leftRotate(root,z->p->p);
			}
		}
	}
	(*root)->color = BLACK;
}

// Function for inserting an interval I in a tree
void rbInsert(NODEPTR *root, Interval I) {
	NODEPTR z = (NODEPTR) malloc(sizeof(struct node));
	z->interval = I;
	NODEPTR y = NILPTR;
	NODEPTR x = *root;
	while(x != NILPTR) {
		y = x;
		if (z->interval.low < x->interval.high)
			x = x->left;
		else
			x = x->right;
	}
	z->p = y;
	if (y == NILPTR)
		*root = z;
	else if (z->interval.low < y->interval.low)
		y->left = z;
	else
		y->right = z;
	z->left = NILPTR;
	z->right = NILPTR;
	z->color = RED;
	
	rbInsertFixup(root,z);
}

// Function for replacing node u with node v
void rbTransplant(NODEPTR *root, NODEPTR u, NODEPTR v) {
	if (u->p == NILPTR)
		*root = v;
	else if (u == u->p->left)
		u->p->left = v;
	else
		u->p->right = v;
	v->p = u->p;
}

// Function for fixing red-black properties in a tree after deletion
void rbDeleteFixup(NODEPTR *root, NODEPTR x) {
	while(x != *root && x->color == BLACK) {
		if (x == x->p->left) {
			NODEPTR w = x->p->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				leftRotate(root,x->p);
				w = x->p->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			}
			else {
			 	if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					rightRotate(root,w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				leftRotate(root,x->p);
				x = *root;
			}
		}
		else {
			NODEPTR w = x->p->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				rightRotate(root,x->p);
				w = x->p->left;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			}
			else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					leftRotate(root,w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				rightRotate(root,x->p);
				x = *root;
			}
		}
	}
	x->color = BLACK;
}

// Function for deleting an interval I in a tree
void rbDelete(NODEPTR *root, Interval I) {
	NODEPTR z = search(*root, I);
	if (z == NILPTR) {
		printf("Node to be deleted not found\n");
		return;
	}
	NODEPTR y = z;
	int yoc = y->color;
	NODEPTR x;
	if (z->left == NILPTR) {
		x = z->right;
		rbTransplant(root,z,z->right);
	}
	else if (z->right == NILPTR) {
		x = z->left;
		rbTransplant(root,z,z->left);
	}
	else {
		y = minimum(z->right);
		yoc = y->color;
		x = y->right;
		if (y->p == z)
			x->p = y;
		else {
			rbTransplant(root,y,y->right);
			y->right = z->right;
			y->right->p = y;
		}
		rbTransplant(root,z,y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}
	if (yoc == BLACK)
		rbDeleteFixup(root,x);
}


// Function for checking overlap of two given intervals I1 and I2
bool checkOverlap(Interval I1, Interval I2) {
    if (I1.low <= I2.high && I2.low <= I1.high)
        return true;
    return false;
}

// Function for searching a given interval I in a given Interval Tree
Interval overlapSearch(NODEPTR root, Interval I) {
    // Base Case: tree is empty
    if (root == NULL)
    	return {INT_MIN, INT_MIN};
 
    // If given interval overlaps with root
    if (checkOverlap(root->interval, I))
        return root->interval;
 
    /* If left child of root is present and max of left child is greater than
       or equal to given interval, then I may overlap with an interval in left subtree */
    if (root->left != NULL && root->left->max >= I.low)
        return overlapSearch(root->left, I);
 
    // Otherwise interval can only overlap with right subtree
    return overlapSearch(root->right, I);
}
