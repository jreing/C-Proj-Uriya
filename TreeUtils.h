#ifndef TREEUTILS_H_
#define TREEUTILS_H_


struct Tree;
typedef struct Tree* TreeRef;
typedef void (*FreeFunc)(void* data);


TreeRef newTree(void* headData);
int isEmptyTree(TreeRef tree);
TreeRef getParent(TreeRef child);
void insertChild(TreeRef parent, void* data);
void DFSPrint (TreeRef tree);
#endif /* TREEUTILS_H_ */

