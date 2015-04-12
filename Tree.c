
//#include "ListUtils.h"
#include "TreeUtils.h"
#include <stdio.h>
#include <stdlib.h>



struct Tree{
	struct List* Children;
	TreeRef Parent;
	void* value;
	int Marked;
};
typedef struct Tree* TreeRef;


/*
int main(){
	//INITIALIZE FOR NON STATIC POINTER TYPES
	int a=1,b=2,c=3,d=4,e=5,f=6,g=7;
	TreeRef temp = newTree((void*)&a);
	insertChild(temp, &b);
	insertChild(temp, &c);
	insertChild(temp, &d);
	insertChild((TreeRef)(headData(temp->Children)), &e);
	insertChild((TreeRef)(headData(temp->Children)), &f);
	insertChild((TreeRef)(headData(tail(tail(temp->Children)))), &g);

	DFSPrint(temp);
	return 0;
}*/

TreeRef newTree(void* headData){
	TreeRef new;
	new = malloc(sizeof(struct Tree));
	new->value = headData;
	new->Children = newList(NULL);
	new->Marked=0;
	new->Parent=NULL;
	return new;
}

void* rootData(TreeRef tree){
	//printf ("headData");
	//printBoard((*Child)(list->value)->state);
	return (isEmptyTree(tree))? NULL : tree->value;

}

TreeRef getParent(TreeRef child){
	return (TreeRef) (child->Parent);
}

ListRef getChildren(TreeRef parent){
    return (ListRef) (parent->Children);
}


void insertChild(TreeRef parent, void* data){
    TreeRef temp =newTree(data);
    temp->Parent=parent;
	append((parent->Children), temp);

}


void DFSPrint (TreeRef tree){
	if(isEmpty(tree->Children)){//tree.children==NULL
		tree->Marked=1;
		printf("%d\n",*(int*)(tree->value));
		return;
	}

	ListRef curChild=tree->Children;
	while(curChild!=NULL){
		if((((TreeRef)(headData(curChild)))->Marked)==0){
			DFSPrint(headData(curChild));
		}
		curChild=tail(curChild);
	}
	tree->Marked=1;
	printf("%d\n",*(int*)(tree->value));
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

