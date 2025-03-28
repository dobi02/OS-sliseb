#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

struct birthday {
  int day;
  int month;
  int year;
  struct list_head list;
};

static LIST_HEAD(birthday_list);

struct birthday *createBirthday(int day, int month, int year) {
  struct birthday *b;
  b = kmalloc(sizeof(struct birthday), GFP_KERNEL); 	
  
  b->day = day;
  b->month = month;
  b->year = year;

  return b;
  /* TODO: 생일을 위한 메모리를 할당하고, 인자들을 채워 생일을 완성하세요. */
}

int simple_init(void) {
  printk("INSTALL MODULE: bdlist\n");
  struct birthday *birthday_node = createBirthday(23, 2, 1995);
  list_add_tail(&birthday_node->list, &birthday_list);
  birthday_node = createBirthday(19, 4, 1967);
  list_add_tail(&birthday_node->list, &birthday_list);
  birthday_node = createBirthday(7, 2, 1964);
  list_add_tail(&birthday_node->list, &birthday_list);

  /* TODO: 생일 목록을 하나씩 생성하는대로 연결리스트에 연결시키세요(노드 삽입). */

  struct birthday *cursor;
  list_for_each_entry(cursor, &birthday_list, list){	
    printk("DAY %d, %d, %d\n", cursor->day, cursor->month, cursor->year);
  }
  /* TODO: 완성된 연결리스트를 탐색하는 커널 함수를 사용하여 출력하세요. */

  return 0;
}

void simple_exit(void) {
  /* 모듈을 제거할 때는 생성한 연결 리스트도 하나씩 제거하며 끝내도록 하세요. */
  /* 제거를 하기 전에 리스트가 "비어있을 경우""에 대한 예외처리를 하는게 좋겠죠? */
  if(list_empty(&birthday_list)) {
    printk("List is Empty\n");
    return;
  }

  struct birthday *cur_node;				// 타입: pointer to node
  struct list_head *cursor, *next;			// 타입: pointer to list
  list_for_each_safe(cursor, next, &birthday_list) {		
	  cur_node = list_entry(cursor, struct birthday, list);	// list를 통해 node 정보 불러옴
	  printk("Removing: %d, %d, %d\n", cur_node->day, cur_node->month, cur_node->year);		// 노드 데이터 출력
	  list_del(cursor);						// 연결리스트에서 해당 list 제거
	  kfree(cur_node);						// 노드에 할당한 메모리 해제
  }

  /* TODO: 이제 본격적으로 연결리스트를 탐색하면서 하나씩 제거하도록 하시면 됩니다. */
  printk("REMOVE MODULE: bdlist\n");
  return;
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("make a list of birthdays and print");
MODULE_AUTHOR("DongbinShin_2021044720");
