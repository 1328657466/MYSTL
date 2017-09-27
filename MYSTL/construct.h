#ifndef CONSTRUCT_H_
#define CONSTRUCT_H_

#include "typetraits.h"
#include <new>

namespace mySTL {
    template <class T1, class T2 >
    inline void construct(T1 *p, const T2& value) {
        //placement new ����ֵ�趨��p��ָ�ռ�
        new (p) T1(value);	//����T1::T1(value)
    }

    template <class T>
    inline void destroy(T* pointer) {
        pointer->~T();
    }

    template <class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last) {
        __destroy(first, last, value_type(first));
    }

    template <class ForwardIterator, class T>
    inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
        typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }

    //���Ԫ������û�� trivial destructor
    template <class ForwardIterator>
    inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type) {
        for (; first < last; ++first)
            destroy(&*first);
    }

    //��trivial destructor  ʲôҲ����
    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

    //��Ի��������ػ�
    inline void destroy(char *, char *) {};

    inline void destroy(int *, int *) {};

    inline void destroy(long *, long *) {};
    inline void destroy(long long *, long long *) {};
    inline void destroy(float *, float *) {};
    inline void destroy(double *, double *) {};
}

#endif