#ifndef LISTUTILS_H_
#define LISTUTILS_H_


struct List;
typedef struct List* ListRef;
typedef void (*FreeFunc)(void* data);


ListRef newList(void* headData);
void* headData(ListRef list);
ListRef tail(ListRef list);
ListRef append(ListRef list, void* data);
int isEmpty(ListRef list);
#endif /* LISTUTILS_H_ */
