#ifndef VECTOR_H_
#define VECTOR_H_

#include "construct.h"
#include "uninitialized.h"
#include "algorithm.h"
#include "allocator.h"
#include "iterator.h"



namespace mySTL {
    template <class T, class Alloc = allocator<T>>
    class vector {
    public:
        typedef T				value_type;
        typedef T*				iterator;
        typedef T&				reference;
        typedef iterator		pointer;
        typedef size_t			size_type;
        typedef ptrdiff_t		difference_type;

    private:
        iterator	start;
        iterator	finish;
        iterator	end_of_storage;
        typedef		Alloc		data_allocator;
    public:			//�����������
        vector() :start(0), finish(0), end_of_storage(0) {}
        explicit vector(const size_type n);
        vector(const size_type n, const T& value);

        vector(const vector<T, Alloc>& v);
        //ע�ͺ��ܱ���ͨ��
        //�˹��캯�������InputIterator �ж��Ƿ�����������������
        //������ִ�к���Ϊvector_aux
        template <class InputIterator>
        vector(InputIterator first, InputIterator last);

        ~vector();
        vector<T>& operator= (const vector<T>& v);

    public:		//��Ա�������
        reference operator[] (size_type index) { return *(start + index); }
        reference front() { return *start; }
        reference back() { return *(finish - 1); }

    public:		//��ȡ������
        iterator begin() { return start; }
        iterator end() { return finish; }

    public:		//�������
        bool empty() { return start == finish; }
        size_type size() const { return finish - start; }
        size_type capacity() const { return end_of_storage - start; }
        void reserve(size_type new_cap);
        void shrink_to_fit();	//���ն���ռ�

    public:		//�����������
        void clear();
        iterator insert(iterator pos, size_type n, const T& val);
        iterator insert(iterator pos, const T& val);
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        void push_back(const T& val);
        void pop_back();
        void resize(size_type n);
        void swap(vector<T>& other);

    public:		//�ֵ���Ƚ����
        bool operator== (const vector<T, Alloc>& v);
        bool operator< (const vector<T, Alloc>& v);
        bool operator> (const vector<T, Alloc>& v);
        bool operator!= (const vector<T, Alloc>& v);
        bool operator>= (const vector<T, Alloc>& v);
        bool operator<= (const vector<T, Alloc>& v);
    private:
        void allocate_and_copy(size_type n, iterator begin, iterator end);
        void allocate_and_fill(size_type n, const T& value);
        void destroy_and_deallocate();

        template <class InputIterator>
        void vector_aux(InputIterator first, InputIterator last, std::false_type);

        //��Ȼ�����ȷʵ��ָ�룬���ǰ����ҵ���⣬����������֪���ǲ��ǣ�����
        //�����Ƿ���������Ҫ��true_type false_type,
        //û����������޷�����ͨ��
        template <class Integer>
        void vector_aux(Integer n, const value_type &val, std::true_type);
    };
}

#include "Detail/vector.impl.h"
#endif
