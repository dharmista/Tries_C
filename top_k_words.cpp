#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

struct Trie{
	Trie * character[26];
	int frequency;
	bool leaf;
};

struct node{
	int frequency;
	char word[30];
	node * left;
	node * right;
};

void start(char *, Trie *);

node * create_node(char * word, int frequency){
	node * temp = (node *)malloc(sizeof(node));
	temp->frequency = frequency;
	strcpy(temp->word, word);
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

void place_to_tree(node * root, node * data){
	if (root->frequency >= data->frequency){
		if (root->right == NULL)
			root->right = data;
		else
			place_to_tree(root->right, data);
	}
	else{
		if (root->left == NULL)
			root->left = data;
		else
			place_to_tree(root->left, data);
	}
}

node * add_to_tree(node * root, node * data){
	if (root == NULL)
		return data;
	else
		place_to_tree(root, data);
	return root;
}

Trie * add_to_Trie(Trie * root, char * name){
	int pos = 0;
	Trie * fin = root;
	//Adding till end of the string
	while (name[pos] != 0){
		if (root == NULL){
			//Allocating new branch..
			root = (Trie *)malloc(sizeof(Trie));
			root->leaf = false;
			root->frequency = 0;
			for (int i = 0; i < 26; i++)
				root->character[i] = NULL;
			if (pos == 0)fin = root;
		}
		if (name[pos] != 0){
			if (root->character[name[pos] - 'a'] == NULL){
				root->character[name[pos] - 'a'] = (Trie *)malloc(sizeof(Trie));
				root->character[name[pos] - 'a']->leaf = false;
				root->character[name[pos] - 'a']->frequency = 0;
				for (int i = 0; i < 26; i++)
					root->character[name[pos] - 'a']->character[i] = NULL;
			}
			root = root->character[name[pos] - 'a'];
		}
		pos++;
	}
	root->leaf = true;
	root->frequency++;
	return fin;
}

void print(char cha, Trie * root, int pos, bool isRoot, int * count) {
	static char name[30];
	if (root == NULL)
		return;
	// Leaving root node
	if (!isRoot){
		if (root->leaf){
			name[pos] = cha;
			name[pos + 1] = 0;
			(*count)++;
			printf("Word : %s\nFrequency : %d\n----\n", name, root->frequency);
		}
		else{
			name[pos] = cha;
		}
	}
	else
		isRoot = false;
	for (int i = 0; i < 26; i++){
		print(i + 'a', root->character[i], pos + 1, isRoot, count);
	}
}
node *root_tree = NULL;
void fetch(char cha, Trie * root, int pos, bool isRoot) {
	static char name[30];
	if (root == NULL)
		return;
	// Leaving root node
	if (!isRoot){
		if (root->leaf){
			name[pos] = cha;
			name[pos + 1] = 0;
			node * data = create_node(name, root->frequency);
			root_tree = add_to_tree(root_tree, data);
		}
		else{
			name[pos] = cha;
		}
	}
	else
		isRoot = false;
	for (int i = 0; i < 26; i++){
		fetch(i + 'a', root->character[i], pos + 1, isRoot);
	}
}

void stop(Trie * root){
	printf("\nPress any key to continue...");
	_getch();
	start(NULL, root);
}

void top_k(Trie * mroot, node * root, int * count, int k){
	if (root == NULL)
		return;
	top_k(mroot, root->left, count, k);
	if (*count < k || k == -1)
		printf("%15s -> %d\n", root->word, root->frequency);
	else
		stop(mroot);
	(*count)++;
	top_k(mroot, root->right, count, k);
}

void show_tree(Trie * root, int c){
	if (root_tree == NULL)
		fetch(0, root, -1, true);
	int count = 0;
	top_k(root, root_tree, &count, c);
	printf("\nPress any key to continue...");
	_getch();
}

void show_all(Trie * root){
	int count = 0;
	system("cls");
	printf("\nAll the words :\n\n----\n");
	print(0, root, -1, true, &count);
	printf("\n%d word(s) found...\n", count);
	printf("\nPress any key to continue...");
	_getch();
}

void start(char * msg, Trie * root){
	system("cls");
	printf("My word dashbord :\n");
	printf("-----------------\n");
	if (msg != NULL)
		printf("\nStatus : %s\n\n", msg);
	printf("[1] Add word\n[2] Get all words\n[3] Show tree\n[4] Top k words\n[5] exit with status 0\n\n");
	printf("Your choice :");
	int ack;
	scanf("%d", &ack);
	if (ack == 1){
		printf("\nEnter the word :");
		char word[30];
		fflush(stdin);
		gets(word);
		root = add_to_Trie(root, word);
		start("Added to list successfully...", root);
	}
	else if (ack == 2) {
		show_all(root);
		start(NULL, root);
	}
	else if (ack == 3) {
		show_tree(root, -1);
		start(NULL, root);
	}
	else if(ack == 4){
		int count = 0, n;
		printf("Enter k value :");
		scanf("%d", &n);
		//top_k(root, root_tree, &count, n);
		show_tree(root, n);
		printf("\nPress any key to continue...");
		_getch();
		start(NULL, root);
	}
	else{
		printf("\nBye!");
		_getch();
	}
}

void main(){
	Trie * root = NULL;
	FILE * fp = fopen("words.txt", "r");
	char word[30];
	int count = 0;
	while (1){
		memset(word, 0, 30);
		fgets(word, 30, fp);
		root = add_to_Trie(root, word);
		count++;
		if (count == 1000)break;
	}
	start(NULL, root);
}
