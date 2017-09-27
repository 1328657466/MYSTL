/*
* alloc.c
* ʵ���ڴ�أ��ռ����
*/

#include "../alloc.h"

namespace mySTL {
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
		//bug fix  for (int i = 1; i < nobjs; i++)
		//ԭ��������������һ���ռ䣬����ʱ�䶨λ��bug
		for (int i = 2; i < nobjs; i++) {
			next_node = (obj *)((char *)cur_node + n);
			cur_node->next = next_node;
			cur_node = next_node;
		}
		cur_node->next = nullptr;

		return result;
	}

	//ʵ���ڴ��
	char *alloc::chunk_alloc(size_t size, int& nobjs) {
		char *result = 0;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes) {
			//�ڴ��ʣ��ռ���������
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= size) {
			//�ڴ��ʣ��ռ䲻������������������һ����������
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else{
			//�ڴ�ؿռ�һ������Ҳ����
			//�������Ŀռ��СΪ 2 * total_bytes + n
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);

			//���ڴ���еĲ���ռ仹�����ü�ֵ,������ռ����free_list
			if (bytes_left > 0) {
				size_t index = FREELIST_INDEX(bytes_left);
				((obj*)start_free)->next = free_list[index];
				free_list[index] = (obj*)start_free;
			}

			//��ϵͳ����ռ������ڴ��
			start_free = (char*)malloc(bytes_to_get);
			if (start_free == 0) {
				//����ռ�ʧ��
				//���free_list������δ���õ����飬�����鹻��
				size_t index = 0;
				obj *p = NULL;
				for (size_t i = size; i <= MAX_BYTES; i += ALIGN) {
					index = FREELIST_INDEX(i);
					p = free_list[index];
					if (!p) {
						free_list[index] = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						//�ݹ�����Լ�������nobjs
						return chunk_alloc(size, nobjs);
					}
				}
				//��ϵͳ����ռ�ʧ�ܣ���free list��Ҳû��δʹ�õı�size�������
				end_free = 0;
			}

			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//�ݹ�����Լ������� nobjs,����start_free
			return chunk_alloc(size, nobjs);
		}
	}
}