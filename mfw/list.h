#ifndef _list_h
#define _list_h

typedef struct _m_structList_t
{
    struct _m_structList_t* prev;
    struct _m_structList_t* next;
}list_t;

/**
 * 说明 :
 *      初始化链表节点,使节点指向自身
 * 参数 :
 *      list_t* node //链表节点指针
 * 返回 : (void)
*/
static inline void list_init(list_t* node)
{
    node->next = node;
    node->prev = node;
}

/**
 * 说明 :
 *      在某节点之前插入一个新节点
 * 参数 :
 *      list_t* node //被插入的节点指针
 *      list_t* insertNode //要插入的节点指针
 * 返回 : (void)
*/
static inline void list_insertBefore(list_t* node,list_t* insertNode)
{
	insertNode->prev = node->prev;
	insertNode->prev->next = insertNode;
	insertNode->next = node;
	node->prev = insertNode;
}

/**
 * 说明 :
 *      移除节点
 * 参数 :
 *      list_t* removeNode //要移除的节点指针
 * 返回 : (void)
*/
static inline void list_remove(list_t* removeNode)
{
	removeNode->prev->next = removeNode->next;
	removeNode->next->prev = removeNode->prev;
	removeNode->next = removeNode;
	removeNode->prev = removeNode;
}

#endif
