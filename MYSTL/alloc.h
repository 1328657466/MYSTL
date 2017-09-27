#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>

namespace mySTL {
    class alloc {
    private:
        enum { ALIGN = 8 };         //��С����
        enum { MAX_BYTES = 128 };   //�������
        enum { NFREELISTS = MAX_BYTES / ALIGN };    //free_lists����
        enum { NJOBS = 20 };        //ÿ�����ӵĽڵ���

    private:
        //���ֽ��ϵ���8�ı���
        static size_t ROUND_UP(size_t bytes) {
            return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
        }
        
    private:
        //free_lists �Ľڵ㹹��
        union obj {
            obj *next;
            char client[1];
        };
        
        static obj *free_list[NFREELISTS];

    private:
        //��������Ĵ�С����ʹ�õ�n��free_list
        static size_t FREELIST_INDEX(size_t bytes) {
            return ((bytes + ALIGN - 1) / ALIGN - 1);
        }
        //����һ����СΪn�Ķ��󣬲����ܼ����СΪn����������free list
        static void *refill(size_t n);
        //����һ���ռ䣬����nobjs * size ��С�ռ�
        //������ܷ���nobjs���ռ䣬nobjs���ܽ���
        static char *chunk_alloc(size_t size, int &nobjs);
        //out of memory ������   ��δ�����ڴ治�㴦����set_new_handle

    private:
        static char *start_free;    //�ڴ����ʼλ��
        static char *end_free;      //�ڴ�ؽ���λ��
        static size_t heap_size;

    public:
        static void *allocate(size_t n);    //�ռ�����
        static void deallocate(void *p, size_t n);  //�ռ��ͷ�
        static void *reallocate(void *p, size_t old_size, size_t new_size);
    };

}

#endif // !ALLOC_H
