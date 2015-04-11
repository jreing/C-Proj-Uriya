//#define LISTUTILS_H_
#include "ListUtils.h"
#include <stdio.h>
#include <stdlib.h>


struct Tree;
typedef struct Tree* TreeRef;



struct Tree{
	struct List* Children;
	TreeRef Parent;
	void* value;
	int Marked;
};

TreeRef newTree(void* headData);
int isEmptyTree(TreeRef tree);
TreeRef getParent(TreeRef child);
void insertChild(TreeRef parent, void* data);
void DFSPrint (TreeRef tree);



int main(){
	//INITIALIZE FOR NON STATIC POINTER TYPES
	int a=1;
	TreeRef temp = newTree((void*)&a);
    a=2;
	insertChild(temp, &a);/*
	insertChild(*a, 3);
	struct Tree b=a.Children->value;
	insertChild(*b, 4);
	insertChild(*b, 5);
	insertChild(*b, 6);*/
	DFSPrint(temp);
	return 0;
}

TreeRef newTree(void* headData){
	TreeRef new;
	new = malloc(sizeof(struct Tree));
	new->value = headData;
	new->Children = newList(NULL);
	new->Marked=0;
	new->Parent=NULL;
	return new;
}
TreeRef getParent(TreeRef child){
	return (TreeRef) (child->Parent);
}
void insertChild(TreeRef parent, void* data){
    TreeRef temp =newTree(data);
    temp->Parent=parent;
	append((parent->Children), temp);

}
void DFSPrint (TreeRef tree){
	if(isEmpty(tree->Children)){//tree.children==NULL
		tree->Marked=1;
		printf("%d",*(int*)(tree->value));
		return;
	}

	ListRef curChild=tree->Children;
	while(curChild!=NULL){
		if((((TreeRef)(headData(curChild)))->Marked)==0){

			DFSPrint(headData(curChild));
		}
		curChild=tail(curChild);
	}
}
int isEmptyTree(TreeRef tree){
	if (tree==NULL)
		return 1;
	if (tree->value==NULL && tree->Children==NULL)
		return 1;
	else {
		return 0;
	}
}


/*void destroyList(ListRef list, FreeFunc freeData){
	ListRef curNext;
	while (!isEmpty(list)){
		freeData (list->value);
		curNext=tail(list);
		free(list);
		list=curNext;
	}
	free(list);
}*/

