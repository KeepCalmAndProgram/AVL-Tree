#include <iostream>
#include <fstream>
#include <cstring>
#include <stack> 
#include <chrono>
#include <cstdlib>
#include <stack>
#include <ctime>

using namespace std;

struct Node
{
	int key;
	Node* right_son;
	Node* left_son;
	int balance;

	Node(int key)
	{
		this->key = key;
		this->left_son = NULL;
		this->right_son = NULL;
		this->balance = 0;
	}
};

class AVL
{
public:
	AVL() {
		root = NULL;
	}
	stack <Node*> findNode(int);

	Node* searchForNode(int);
	int addNode(int);
	void addMultipleNodes(int);

	int getSize() {
		return getSize(root);
	}

	void inorder() {
		int count = inorder(root); cout << "\nIlosc wezlow: " << count << endl;
	}
	void preorder() {
		int count = preorder(root); cout << "\nIlosc wezlow: " << count << endl;
	}
	void postorder() {
		int count = postorder(root); cout << "\nIlosc wezlow: " << count << endl;
	}

	int deleteNode(int);

private:
	Node* root;

	int inorder(Node*);
	int preorder(Node*);
	int postorder(Node*);

	int height(Node*);
	stack <Node*> findMin(Node*);

	int getSize(Node*);

	void rotateRight(Node*, Node*, Node*);
	void rotateLeft(Node*, Node*, Node*);
};

unsigned int dataDraw()
{
	unsigned int x = rand();
	x = x << 16;
	x = (x + rand()) % 100000;

	return x;
}

Node* AVL::searchForNode(int key)
{
	Node* copyRoot = root;

	if (root == NULL)
	{
		return NULL;
	}

	while (copyRoot)
	{
		if (copyRoot->key == key)
		{
			return copyRoot;
		}
		if (copyRoot->key > key) {
			copyRoot = copyRoot->left_son;
		}
		else {
			copyRoot = copyRoot->right_son;
		}
	}

	return NULL;
}

int AVL::deleteNode(int key)
{
	if (!root)
	{
		return -1;
	}

	stack <Node*> droga = findNode(key);
	if (!droga.top())
	{
		return -1;
	}

	Node* cel = droga.top();
	droga.pop();

	if (!(cel->left_son) && !(cel->right_son))
	{
		if (cel == root) {
			root = NULL;
		}
		else {

			if (droga.top()->left_son == cel) {
				droga.top()->left_son = NULL;
			}
			else {
				droga.top()->right_son = NULL;
			}
			delete cel;
		}
	}
	else if ((!(cel->left_son) && (cel->right_son)) || ((cel->left_son) && !(cel->right_son)))
	{
		if (cel == root) {
			root = cel->left_son ? cel->left_son : cel->right_son;
		}
		else {

			if (droga.top()->left_son == cel)
			{
				droga.top()->left_son = cel->left_son ? cel->left_son : cel->right_son;
			}
			else {
				droga.top()->right_son = cel->left_son ? cel->left_son : cel->right_son;
			}
			delete cel;
		}
	}
	else
	{
		stack <Node*> wayToElement = findMin(cel->right_son);
		Node* next_node = wayToElement.top();

		wayToElement.pop();

		if (cel == root)
		{
			root = next_node;
		}
		else {
			if (droga.top()->left_son == cel) {
				droga.top()->left_son = next_node;
			}
			else {
				droga.top()->right_son = next_node;
			}
		}


		if (cel->right_son == next_node)
		{
			next_node->left_son = cel->left_son;
		}
		else
		{
			wayToElement.top()->left_son = next_node->right_son ? next_node->right_son : NULL;
			next_node->left_son = cel->left_son;
			next_node->right_son = cel->right_son;
		}
		delete cel;

		droga.push(next_node);
		stack <Node*> tmp_wayToElement;

		while (!wayToElement.empty())
		{
			tmp_wayToElement.push(wayToElement.top());
			wayToElement.pop();
		}
		while (!tmp_wayToElement.empty())
		{
			droga.push(tmp_wayToElement.top());
			tmp_wayToElement.pop();
		}
	}

	while (!droga.empty()) {
		Node* tmp = droga.top();
		tmp->balance = height(tmp->right_son) - height(tmp->left_son);

		if (tmp->balance == 1 || tmp->balance == -1) {
			return 0;
		}

		if (tmp->balance == 2) {
			if (tmp->right_son->balance == 1) {
				droga.pop();
				rotateLeft(droga.empty() ? NULL : droga.top(), tmp, tmp->right_son);

			}
			else if (tmp->right_son->balance == 0) {

				droga.pop();
				rotateLeft(droga.empty() ? NULL : droga.top(), tmp, tmp->right_son);

				return 0;
			}
			else if (tmp->right_son->balance == -1) {

				rotateRight(tmp, tmp->right_son, tmp->right_son->left_son);
				droga.pop();
				rotateLeft(droga.empty() ? NULL : droga.top(), tmp, tmp->right_son);
			}
		}
		else if (tmp->balance == -2) {
			if (tmp->left_son->balance == -1) {

				rotateLeft(tmp, tmp->left_son, tmp->left_son->right_son);
				droga.pop();
				rotateRight(droga.empty() ? NULL : droga.top(), tmp, tmp->left_son);

			}
			else if (tmp->left_son->balance == 1) {

				droga.pop();
				rotateRight(droga.empty() ? NULL : droga.top(), tmp, tmp->left_son);
				return 0;

			}
			else if (tmp->left_son->balance == 0) {

				droga.pop();
				rotateRight(droga.empty() ? NULL : droga.top(), tmp, tmp->left_son);

			}
		}
		droga.pop();
	}
	return 0;
}
int AVL::addNode(int key)
{
	if (!root) {
		root = new Node(key);
		return 0;
	}

	stack <Node*> droga = findNode(key);

	if (droga.top()) {
		return -1;
	}
	droga.pop();

	if (key < droga.top()->key) {
		droga.top()->left_son = new Node(key);
	}
	else {
		droga.top()->right_son = new Node(key);
	}
	while (!droga.empty()) {
		Node* tmp = droga.top();
		tmp->balance = height(tmp->right_son) - height(tmp->left_son);

		if (tmp->balance == 0) {
			return 0;
		}

		if (tmp->balance == -2) {
			if ((tmp->left_son)->balance == -1) {
				droga.pop();
				rotateRight(droga.empty() ? NULL : droga.top(), tmp, tmp->left_son);
			}
			else {
				rotateLeft(tmp, tmp->left_son, tmp->left_son->right_son);
				droga.pop();
				rotateRight(droga.empty() ? NULL : droga.top(), tmp, tmp->left_son);
			}
			return 0;
		}
		if (tmp->balance == 2) {
			if ((tmp->right_son)->balance == 1) {
				droga.pop();
				rotateLeft(droga.empty() ? NULL : droga.top(), tmp, tmp->right_son);
			}
			else
			{
				rotateRight(tmp, tmp->right_son, tmp->right_son->left_son);

				droga.pop();
				rotateLeft(droga.empty() ? NULL : droga.top(), tmp, tmp->right_son);
			}
			return 0;
		}
		droga.pop();
	}
	return 0;
}

void AVL::addMultipleNodes(int ilosc)
{
	srand(time(0));

	for (int i = 0; i < ilosc; i++) {
		addNode((rand() % -19999 + 10000));
		cout << "key zgenerowany!" << endl;
	}
}

int AVL::height(Node* copyRoot)
{
	if (!copyRoot) {
		return 0;
	}
	int left = height(copyRoot->left_son) + 1;
	int right = height(copyRoot->right_son) + 1;

	return left > right ? left : right;
}

int AVL::getSize(Node* copyRoot)
{
	int count = 0;

	if (copyRoot) {
		count += getSize(copyRoot->left_son);
		count += getSize(copyRoot->right_son);
		count++;
	}
	return count;
}

stack <Node*> AVL::findNode(int key)
{
	stack <Node*> droga;
	Node* copyRoot = root;

	if (!root) {
		return droga;
	}
	while (copyRoot)
	{
		droga.push(copyRoot);
		if (copyRoot->key == key) {
			return droga;
		}
		if (key < copyRoot->key) {
			copyRoot = copyRoot->left_son;
		}
		else {
			copyRoot = copyRoot->right_son;
		}
	}
	droga.push(NULL);
	return droga;
}

int AVL::preorder(Node* copyRoot)
{
	int count = 0;
	if (copyRoot) {
		cout << copyRoot->key << "  " << copyRoot->balance << "    ";
		count++;
		count += preorder(copyRoot->left_son);
		count += preorder(copyRoot->right_son);
	}
	return count;
}

int AVL::inorder(Node* copyRoot)
{
	int count = 0;
	if (copyRoot) {
		count += inorder(copyRoot->left_son);
		cout << copyRoot->key << "  " << copyRoot->balance << "    ";
		count++;
		count += inorder(copyRoot->right_son);
	}
	return count;
}

int AVL::postorder(Node* copyRoot)
{
	int count = 0;
	if (copyRoot) {
		count += postorder(copyRoot->left_son);
		count += postorder(copyRoot->right_son);
		cout << copyRoot->key << "  " << copyRoot->balance << "    ";
		count++;
	}
	return count;
}

void AVL::rotateRight(Node* dziadek, Node* ojciec, Node* potomek)
{
	if (dziadek) {
		if (dziadek->right_son == ojciec) {
			dziadek->right_son = potomek;
		}
		else {
			dziadek->left_son = potomek;
		}
	}
	else {
		root = potomek;
	}

	Node* temp = potomek->right_son;

	potomek->right_son = ojciec;
	ojciec->left_son = temp;

	if (dziadek) {
		dziadek->balance = height(dziadek->right_son) - height(dziadek->left_son);
		ojciec->balance = height(ojciec->right_son) - height(ojciec->left_son);
		potomek->balance = height(potomek->right_son) - height(potomek->left_son);
	}
}

void AVL::rotateLeft(Node* dziadek, Node* ojciec, Node* potomek)
{
	if (dziadek) {
		if (dziadek->right_son == ojciec) {
			dziadek->right_son = potomek;
		}
		else {
			dziadek->left_son = potomek;
		}
	}
	else
	{
		root = potomek;
	}
	Node* temp = potomek->left_son;
	potomek->left_son = ojciec;
	ojciec->right_son = temp;

	if (dziadek) {
		dziadek->balance = height(dziadek->right_son) - height(dziadek->left_son);
		ojciec->balance = height(ojciec->right_son) - height(ojciec->left_son);
		potomek->balance = height(potomek->right_son) - height(potomek->left_son);
	}
}

stack <Node*> AVL::findMin(Node* copyRoot)
{
	stack <Node*> wayToElement;
	wayToElement.push(copyRoot);

	while (copyRoot->left_son) {
		copyRoot = copyRoot->left_son;
		wayToElement.push(copyRoot);
	}

	return wayToElement;
}
int main(int argc, char* argv[])
{
	if (argc != 3) {
		return 1;
	}

	int n = atoi(argv[1]);
	bool randomly = atoi(argv[2]);

	AVL obj;

	srand(555);

	if (randomly) {
		auto t_start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < n; ++i)
		{
			obj.addNode(dataDraw());
		}

		auto t_end = std::chrono::high_resolution_clock::now();
	}
	else {
		int N;
		N = n / 2;

		auto t_start = std::chrono::high_resolution_clock::now();

		for (int i = 1; i <= N; ++i)
		{
			obj.addNode(dataDraw());
			obj.addNode(i);
		}
		auto t_end = std::chrono::high_resolution_clock::now();
	}
	obj.getSize();

	auto t_start = std::chrono::high_resolution_clock::now();

	int found = 0;

	for (int i = 0; i < n; ++i) {
		if (obj.searchForNode(dataDraw())) found++;
	}
	auto t_end = std::chrono::high_resolution_clock::now();

	t_start = std::chrono::high_resolution_clock::now();

	int delet = 0;

	for (int i = 0; i < n; ++i) {
		if (obj.deleteNode(dataDraw()) == 0)
			++delet;
	}
	t_end = std::chrono::high_resolution_clock::now();

	cout << obj.getSize() << endl;

	getchar();
	getchar();

	return 0;
}