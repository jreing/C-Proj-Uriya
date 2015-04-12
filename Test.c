
#include "TreeUtils.h"

//#include <stdio.h>
//#include <stdlib.h>


int main(){
	//INITIALIZE FOR NON STATIC POINTER TYPES
	int a=1,b=2,c=3,d=4,e=5,f=6,g=7;
	TreeRef temp = newTree((void*)&a);
	insertChild(temp, &b);
	insertChild(temp, &c);
	insertChild(temp, &d);
	TreeRef Two=(TreeRef)(headData(getChildren(temp)));
	insertChild(Two, &e);
	insertChild(Two, &f);
	insertChild((TreeRef)(headData(tail(tail(getChildren(temp))))), &g);

	DFSPrint(temp);
	return 0;
}

