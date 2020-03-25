#pragma once
struct list_head {
	struct list_head* next, * prev;
};
#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

#define LIST_HEAD_INIT(name) { &(name), &(name) }   

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

/*
	结构体地址偏移
*/
#define offsetof(TYPE, MEMBER)((size_t)&((TYPE *)0)->MEMBER)
//该代码安全的原因不是没有写入任何内存位置，而是因为没有访问过任何内存位置。

#define container_of(ptr, type, member)({	\
	const typeof( ((type*)0)->member) *__mptr = (ptr); \
	(type *)((char *) __mptr - offsetof(type,member));})


//遍历链表
#define list_for_each(pos,head) \
	for(pos = (head)->next; /*prefetch(pos->next),*/ pos != (head); \
	pos = pos->next)
//链表初始化
static inline void INIT_LIST_HEAD(struct list_head* list)
{
	list->next = list;
	list->prev = list;
}

//结构体 首地址函数 容器结构体的地址
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

//获取链表容器结构的地址
#define list_for_each_entry(pos, head, member)	\
	for(pos = list_entry((head)->next, typeof(*pos), member); \
		&pos->member != (head);	\
		pos = list_entry(pos->member.next, typeof(*pos), member))


static inline void __list_add(struct list_head* new,
								struct list_head* prev,
								struct list_head* next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}
//增加节点
static inline void list_add(struct list_head* new, struct list_head* head)
{
	__list_add(new, head, head->next);
}

//删除节点
static inline void __list_del(struct list_head* prev,struct list_head *next)
{
	next->prev=prev;
	prev->next=next;
}
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev,entry->next);
	entry->next=LIST_POISON1;
	entry->prev=LIST_POISON2;
}
//安全执行链表删除操作

#define list_for_each_entry_safe(pos, n, head, member)	\
	for(pos = list_entry( (head)->next, typeof(*pos), member),	\
	n = list_entry(pos->member.next,typeof(*pos),member);	\
	&pos->member !=(head);	\
	pos = n,n = list_entry(n->member.next, typeof(*n),member))


static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

//尾插方法
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new,head->prev,head);
}

//替换链表操作
static inline void list_replace(struct list_head *old, 
								struct list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

//list_replace没有将old的前驱和后继断开
static inline void list_replace_init(struct list_head *old,
					struct list_head *new)
{
	list_replace(old,new);
	INIT_LIST_HEAD(old);
}

//删除list指针所处的容器结构节点，然后将其重新以头插法添加到另一个头结点中去，
//head可以是该链表自身，也可以是其他链表的头指针。
static inline void list_move(struct list_head *list,
				struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add(list, head);
}

static inline void list_move_tail(struct list_head *list,
					struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}