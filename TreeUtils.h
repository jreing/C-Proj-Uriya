#ifndef TREEUTILS_H_
#define TREEUTILS_H_
#include "ListUtils.h"

struct Tree;
typedef struct Tree* TreeRef;
typedef void (*FreeFunc)(void* data);
typedef void (*printFunc)(void* data);

TreeRef newTree(void* headData);
int isEmptyTree(TreeRef tree);
TreeRef getParent(TreeRef child);
int getMarked(TreeRef tree);
void insertChild(TreeRef parent, void* data);
void DFSPrint (TreeRef tree, printFunc print);
void DFSTwo (TreeRef tree, printFunc print);
ListRef getChildren(TreeRef parent);
void* rootData(TreeRef tree);
#endif /* TREEUTILS_H_ */

