/*
* alloc.c
* ʵ���ڴ�أ��ռ����
*/

#include "alloc.h"

namespace MYSTL {
	char *alloc::start_free = 0;
	char *alloc::end_free = 0;
	size_t alloc::heap_size = 0;
	alloc::obj *alloc::free_list[NFREELISTS] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	void *alloc::allocate(size_t n) {
		//����128����malloc
		if (n > MAX_BYTES) {
			return (malloc(n));
		}

		size_t index = FREELIST_INDEX(n);
		obj *list = free_list[index];

		//����п��ÿռ�
		if (list) {
			free_list[index] = list->next;
			return list;
		}
		else {	//�޿��ÿռ�
			return refill(ROUND_UP(n));
		}
	}

	void alloc::deallocate(void *p, size_t n) {
		if (n > MAX_BYTES) {
			free(p);
			return;
		}

		//�������
		size_t index = FREELIST_INDEX(n);
		obj *q = static_cast<obj *> (p);
		q->next = free_list[index];
		free_list[index] = q;
	}
	
	void *alloc::reallocate(void *p, size_t old_size, size_t new_size) {
		deallocate(p, old_size);
		p = allocate(new_size);
		return p;
	}

	//ȱʡȡ��20�������飬����ڴ�ؿռ䲻�㣬����С��20
	void* alloc::refill(size_t n) {
		int nobjs = 20;
		//����chunk_alloc�����Ի��nobjs��������Ϊfree_list���½ڵ�
		//nobjs�Ǵ�������
		char *chunk = chunk_alloc(n, nobjs);

		//ֻ���һ������������������
		if (nobjs == 1) {
			return chunk;
		}

		void *result = chunk;	//�˿�ռ䷵�ظ�������
		obj *cur_node = (obj *)(chunk + n);
		obj *next_node = nullptr;

		size_t index = FREELIST_INDEX(n);
		free_list[index] = cur_node;
		//��free_list�����ڵ㴮����
		for (int i = 1; i < nobjs; i++) {
			next_node = (obj *)((char *)cur_node + n);
			cur_node->next = next_node;
			cur_node = next_node;
		}
		cur_node->next = nullptr;

		return result;
	}

	//ʵ���ڴ��
	char *alloc::chunk_alloc(size_t size, int &nobjs) {

	}
}