
#include "TreeUtils.h"

#include <stdio.h>
#include <stdlib.h>


int main(){

    struct Widget {
        int x, y, width, height;
        int isSelected;
        char* img_filename;
        char* caption;
    };
    typedef struct Widget* WidgetRef;

    WidgetRef newWidget(int x, int y, int width, int height, int isSelected, char* img_filename, char* caption){
        WidgetRef new;
        new = malloc(sizeof(struct Widget));
        new->x= x;
        new->y= y;
        new->width= width;
        new->height= height;
        new->isSelected= isSelected;
        new->img_filename= img_filename;
        new->caption= caption;

        return new;
    }


	//INITIALIZE FOR NON STATIC POINTER TYPES
    ///TEST 1
    /*WidgetRef a=newWidget(1,1,500,500,1,"hello","HiThere");
    TreeRef temp= newTree((void*)a);
    printf("x equals= %d\n",((WidgetRef)rootData(temp))->x);
    printf("y equals= %d\n",((WidgetRef)rootData(temp))->y);
    printf("width equals= %d\n",((WidgetRef)rootData(temp))->width);
    printf("height equals= %d\n",((WidgetRef)rootData(temp))->height);
    printf("isSelected equals= %d\n",((WidgetRef)rootData(temp))->isSelected);
    printf("filename equals= %p\n",((WidgetRef)rootData(temp))->img_filename);

    WidgetRef b=newWidget(2,2,502,502,2,"hello2","HiThere2");
    insertChild(temp, b);

    //printf("x of child is %d ", ((WidgetRef)(headData(getChildren(temp))))->x);
    DFSTwo(temp);
    */

    ///TESTTTT 2- DFS, DFS2 working, using void** instead of int*
    /*
	int a=1,b=2,c=3,d=4,e=5,f=6,g=7;

	//lets make a fun int print func
	void printInt (void* data){
        printf("I hope this fucking works %d\n", (int)data);
	}
	TreeRef temp = newTree((void*)&a);
	insertChild(temp, &b);
	insertChild(temp, &c);
	insertChild(temp, &d);
	TreeRef Two=(TreeRef)(headData(getChildren(temp)));
	//
	*(int*)(tree->value)
	void** p= (rootData(Two));
	printf("this is the fucking right value thou %d\n", *p);
	printf("this is TWO %d\n", *(int*)(rootData(Two)));// works- no one knows why

    ///OBSERVATION- it seems that with each adding of child, we go one level down pointer wise, which inturn creates the problem
    //lets try it one level down, we want to print 5-e
	insertChild(Two, &e);
    TreeRef Three=(TreeRef)(headData(getChildren(Two)));

    void*** q= (rootData(Three));
	printf("this is the fucking right value thou- THREE %d\n", *q);
	printf("this is THREE %d\n", *(int*)(rootData(Three)));// works- no one knows why

    printf("DFS begins here\n\n");

	DFSTwo(temp,printInt);
    */
	//insertChild(Two, &f);
	//insertChild((TreeRef)(headData(tail(tail(getChildren(temp))))), &g);

    ///TEST 3//now working- must remember to use unMark!!
    /*int a=1,b=2;
	TreeRef temp = newTree((void*)&a);
	insertChild(temp, &b);
	//TreeRef Two=(TreeRef)(headData(getChildren(temp)));
	//printf("this is TWO %d\n", rootData(Two));
    printf("this is the Normal DFS\n");
    DFSPrint(temp);
    unMarkTree(temp);
    printf("this is the Alternate DFS\n");
    DFSTwo(temp);//not working proeperly
    */
    ///Test 4
    /*int a=1,b=2,c=3,d=4,e=5,f=6,g=7;
	TreeRef temp = newTree((void*)&a);
	insertChild(temp, &b);
	insertChild(temp, &c);
	insertChild(temp, &d);
	TreeRef Two=(TreeRef)(headData(getChildren(temp)));
	printf("this is TWO %d\n", rootData(Two));
	insertChild(Two, &e);
	insertChild(Two, &f);
	insertChild((TreeRef)(headData(tail(tail(getChildren(temp))))), &g);

	DFSPrint(temp);*/
	///TEST 5

	//lets make a fun int print func
	void printInt (void* data){
        printf("Int Printing %d\n", *(int*)data);
	}

	void printWidget(void* data){
	    WidgetRef temp=(WidgetRef) data;
        printf("x=%d,  y=%d,  width=%d,  height=%d, filename equals= %s\n",temp->x,temp->y,temp->width, temp->height, temp->img_filename);
        //printf("isSelected equals= %d\n",((WidgetRef)rootData(temp))->isSelected);
        //printf("filename equals= %p\n",((WidgetRef)rootData(temp))->img_filename);
	}
	int a=1,b=2,c=3,d=4,e=5,f=6,g=7;
	TreeRef temp = newTree((void*)&a);
	insertChild(temp, &b);
	insertChild(temp, &c);
	insertChild(temp, &d);
	TreeRef Two=(TreeRef)(headData(getChildren(temp)));
	//printf("this is TWO %d\n", rootData(Two));
	insertChild(Two, &e);
	insertChild(Two, &f);
	insertChild((TreeRef)(headData(tail(tail(getChildren(temp))))), &g);


    printf("DFS begins here\n\n");

	DFSPrint(temp,printInt);

    ///IF WE WANTED TO USE TEMP AGAIN, WE WOULD CALL unMark
	WidgetRef a2=newWidget(1,1,500,500,1,"hello","HiThere");
    TreeRef temp2= newTree((void*)a2);
    WidgetRef b2=newWidget(2,2,502,502,2,"hello2","HiThere2");
    insertChild(temp2, b2);
    DFSPrint(temp2,printWidget);
	return 0;
}

