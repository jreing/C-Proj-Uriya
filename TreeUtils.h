#ifndef TREEUTILS_H_
#define TREEUTILS_H_
#include "ListUtils.h"

struct Tree;
typedef struct Tree* TreeRef;
typedef void (*FreeFunc)(void* data);


TreeRef newTree(void* headData);
int isEmptyTree(TreeRef tree);
TreeRef getParent(TreeRef child);
void insertChild(TreeRef parent, void* data);
void DFSPrint (TreeRef tree);
ListRef getChildren(TreeRef parent);
void* rootData(TreeRef tree);
#endif /* TREEUTILS_H_ */

