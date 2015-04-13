
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
	return (isEmptyTree(tree))? NULL : tree->value;

}

TreeRef getParent(TreeRef child){
	return (TreeRef) (child->Parent);
}

ListRef getChildren(TreeRef parent){
    return (ListRef) (parent->Children);
}

int getMarked(TreeRef tree){
        return tree->Marked;
}

void insertChild(TreeRef parent, void* data){
    TreeRef temp =newTree(data);
    temp->Parent=parent;
	append((parent->Children), temp);

}

void DFSPrint (TreeRef tree, printFunc print){


	if(isEmpty(getChildren(tree))){//tree.children==NULL
		tree->Marked=1;
		//printf("%d\n",*(void**)(rootData(tree)));
		print((void**)(rootData(tree)));///SENDS ACTUAL OBJECT, NOT A REFERENCE!!
		return;
	}

	ListRef curChild=getChildren(tree);
	while(curChild!=NULL){
        //printf("current Marked is %d",((TreeRef)(headData(curChild)))->Marked);
		if((getMarked((TreeRef)(headData(curChild))))==0){
			DFSPrint(headData(curChild),print);
		}
		curChild=tail(curChild);
	}
	tree->Marked=1;
	print((void**)(rootData(tree)));
}
void unMarkTree(TreeRef tree){
    if(isEmpty(getChildren(tree))){//tree.children==NULL
		tree->Marked=0;
		return;
	}

	ListRef curChild=getChildren(tree);
	while(curChild!=NULL){
        //printf("current Marked is %d",((TreeRef)(headData(curChild)))->Marked);
		unMarkTree(headData(curChild));
		curChild=tail(curChild);
	}
	tree->Marked=0;

}
void DFSTwo (TreeRef tree, printFunc print){

	if(isEmpty(getChildren(tree))){//tree.children==NULL
		tree->Marked=1;
		//printf("%d\n",*(void**)(rootData(tree)));
		print(*(void**)(rootData(tree)));///SENDS ACTUAL OBJECT, NOT A REFERENCE!!
		return;
	}

	ListRef curChild=getChildren(tree);
	while(curChild!=NULL){
        //printf("current Marked is %d",((TreeRef)(headData(curChild)))->Marked);
		if((getMarked((TreeRef)(headData(curChild))))==0){
			DFSTwo(headData(curChild),print);
		}
		curChild=tail(curChild);
	}
	tree->Marked=1;
	print(*(void**)(rootData(tree)));
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

