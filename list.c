#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define min(x,y) 	({      \
	typeof(x) _x = (x);		\
	typeof(y) _y = (y);		\
	(void)(&_x == &_y);		\
	_x < _y ? _x : _y; })
	//指针（地址）的比较也需要相同类型
	//(void) 是为了防止编译器警告语句的值没有被使用。
	
	
struct person
{
	int age;
	struct list_head list;
};
// static void for_test(struct person* p)
// {
// 	printf("%p\n%p\n", &p->age, &p->list);
// }
// static void for_test_2(struct person* p)
// {
// 	printf("%u\n%u\n",(unsigned int)&p->age,(unsigned int)&p->list);
// }

int main(int argc, char **argv)
{
	int i;
	struct person *p;
	struct person head;
	//struct list_head *pos;	
	//struct list_head *pos;
	struct person *pos;
	struct person new_obj={.age=100};
	INIT_LIST_HEAD(&head.list);
	for ( i = 0; i < 5; i++)
	{
		p = (struct person *)malloc(sizeof(struct person ));
		p->age = i*10;
		//list_add(&p->list,&head.list);
		list_add_tail(&p->list,&head.list);
	}
	list_for_each_entry_safe(pos,p,&head.list,list){
		if(pos->age == 30){
			//list_del(&pos->list);
			//break;
			list_replace(&pos->list,&new_obj.list);
		}
	}
	list_for_each_entry(pos,&head.list,list){
		printf("age = %d \n",pos->age);
	}
	return 0;
	// list_for_each(pos,&head.list){
	// 	int a=1;
	// 	typeof(a) c=1;
	// 	//printf("c=%d " ,c);
	// 	p = list_entry(pos,struct person,list);
	// 	printf("age=%d \n",p->age);
	// }
	// 	list_for_each_entry(pos2,&head.list,list){
	// 		printf("age = %d\n", pos2->age);
	// 	}
	// __auto_type a=1;
	// int b=min(8,4.3);
	// printf("a = %d\n",b);

}
// int main(int argc,char **argv)
// {
// 	struct person head;
// 	//for_test(&head);
// 	for_test_2(NULL);
// 	return 0;
// } 