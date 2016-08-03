#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

//Structure of each node in a trie
struct Trie{
	Trie * character[26];
	char number[12];
	bool leaf;
};

void start(char *, Trie *);

Trie * add_to_Trie(Trie * root, char * name, char * mobile){
	int pos = 0;
	Trie * fin = root;
	//Adding till end of the string
	while (name[pos] != 0){
		if (root == NULL){
			//Allocating new branch..
			root = (Trie *)malloc(sizeof(Trie));
			root->leaf = false;
			for (int i = 0; i < 26; i++)
				root->character[i] = NULL;
			if (pos == 0)fin = root;
		}
		if (name[pos] != 0){
			if (root->character[name[pos] - 'a'] == NULL){
				root->character[name[pos] - 'a'] = (Trie *)malloc(sizeof(Trie));
				root->character[name[pos] - 'a']->leaf = false;
				for (int i = 0; i < 26; i++)
					root->character[name[pos] - 'a']->character[i] = NULL;
			}
			root = root->character[name[pos] - 'a'];
		}
		pos++;
	}
	//If a node is already leaf, and it has a contact?
	if (root->leaf){
		printf("\nReplace contact? (Y/N) :");
		char ch = getchar();
		if (ch == 'Y' || ch == 'y'){
			strcpy(root->number, mobile);
		}
	}
	else{
		root->leaf = true;
		strcpy(root->number, mobile);
	}
	return fin;
}

char * get_mobile_num(Trie * root, char * name){
	//Passing through the branches
	int pos = 0; bool error = false;
	while (name[pos] != 0){
		if (root == NULL){
			error = true;
			break;
		}
		if (name[pos] != 0)
			root = root->character[name[pos] - 'a'];
		pos++;
	}
	//If branch has no phone number..
	if (root == NULL || error || !root->leaf)
		return NULL;
	return root->number;
}

void print(char * sugg, char cha, Trie * root, int pos, bool isRoot, int * count) {
	static char name[30];
	if (root == NULL)
		return;
	// Leaving root node
	if (!isRoot){
		if (root->leaf){
			name[pos] = cha;
			name[pos + 1] = 0;
			(*count)++;
			printf("Name : %s%s\nMobile number : %s\n----\n", sugg, name, root->number);
		}
		else{
			name[pos] = cha;
		}
	}
	else
		isRoot = false;
	for (int i = 0; i < 26; i++){
		print(sugg, i + 'a', root->character[i], pos + 1, isRoot, count);
	}
}

void show_suggestions(Trie * root, char * name){
	int pos = 0;
	Trie * prev = root;
	while (name[pos] != 0){
		if (root == NULL){
			printf("\nNo suggestions..");
			return;
		}
		prev = root;
		root = root->character[name[pos] - 'a'];
		pos++;
	}int count = 0;
	int len = strlen(name);
	name[len - 1] = 0;
	system("cls");
	printf("\nAll the available suggestions :\n\n----\n");
	print(name, 0, prev, -1, true, &count);
	printf("\n%d contact(s) found...\n", count);
}

void show_all_contacts(Trie * root){
	system("cls");
	if (root == NULL){
		start("No contacts. Add some...", root);
		return;
	}
	int count = 0;
	print("", 0, root, -1, true, &count);
	printf("\n%d contact(s) found...\n", count);
}

void start(char * msg, Trie * root){
	system("cls");
	printf("My contact book :\n");
	printf("-----------------\n");
	if (msg != NULL)
		printf("\nStatus : %s\n\n", msg);
	printf("[1] Add contact\n[2] search for contact\n[3] Show suggestions\n[4] Show all contacts\n\n");
	printf("Your choice :");
	int ack;
	scanf("%d", &ack);
	if (ack == 1){
		printf("\nMax length of user name is 30, all small letters, no numbers in name, and no country codes in mobile number..\n");
		char name[30];
		printf("\nEnter name :");
		fflush(stdin);
		gets(name);
		printf("\nEnter mobile number :");
		char mobile[11];
		fflush(stdin);
		gets(mobile);
		root = add_to_Trie(root, name, mobile);
		start("Added to contact successfully..", root);
	}
	else if (ack == 2) {
		printf("\nEnter name of the contact :");
		char name[30];
		fflush(stdin);
		gets(name);
		char * mobile = get_mobile_num(root, name);
		if (mobile != NULL){
			printf("\nMobile number : %s", mobile);
			printf("\nPress any key to continue...");
			_getch();
			start(NULL, root);
		}
		else{
			start("No contact with such name...", root);
		}
	}
	else if (ack == 3) {
		printf("\nEnter name :");
		char name[30];
		fflush(stdin);
		gets(name);
		show_suggestions(root, name);
		printf("\nPress any key to continue...");
		_getch();
		start(NULL, root);
	}
	else{
		show_all_contacts(root);
		printf("\nPress any key to continue...");
		_getch();
		start(NULL, root);
	}
}

void main(){
	Trie * root = NULL;
	start(NULL, root);
	_getch();
}