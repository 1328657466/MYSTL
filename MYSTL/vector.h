#ifndef VECTOR_H_
#define VECTOR_H_

#include "allocator.h"
#include "iterator.h"
#include "construct.h"
#include "uninitialized.h"

namespace mySTL {
	template <class T, class Alloc = alloc>
	class vector {
	public :
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*    const_pointer;
		typedef T*			iterator;
		typedef const T*	const_iterator
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
	protected:
		typedef allocator<value_type, Alloc> date_allocator;
		iterator	start;				//Ŀǰʹ�ÿռ��ͷ
		iterator	finish;				//Ŀǰʹ�ÿռ��β
		iterator	end_of_storage;		//Ŀǰ���ÿռ��β
		void fill_initialize();

	public :
		//�������
		size_type size() { return (size_type)(finish - start); }
		size_type capacity() const { return (size_type)(end_of_storage - start);}
		bool empty() const { return (finish - start) == 0; }
		reference operator [] const (size_type n) {
			return *(start + n);
		}

	public:		//���죬��������
		vector() :start(0), end(0), end_of_storage(0);
		vector(size_type n, const T& value);

		template <class InputIterator>
		vector(InputIterator first, InputIterator last);
		//explicit ���ƹ��캯������ʽת��
		explicit vector(size_type n);
		vector<T, Alloc>& operator = (const vector<T, Alloc>& v);

		~vector() {}

	public:	//���������أ��ֵ���ȽϺ���
		template <class T, class Alloc>
		friend bool operator == (const vector<T, Alloc>& v1,
			const vector<T, Alloc>& v2);

		template <class T, class Alloc>
		friend bool operator != (const vector<T, Alloc>& v1,
			const vector<T, Alloc>& v2);

		template <class T, class Alloc>
		friend bool operator > (const vector<T, Alloc>& v1,
			const vector<T, Alloc>& v2);

		template <class T, class Alloc>
		friend bool operator >= (const vector<T, Alloc>& v1,
			const vector<T, Alloc>& v2);

		template <class T, class Alloc>
		friend bool operator < (const vector<T, Alloc>& v1,
			const vector<T, Alloc>& v2);

		template <class T, class Alloc>
		friend bool operator <= (const vector<T, Alloc>& v1,
			const vector<T, Alloc>& v2);

	public:	//�����������
		
		iterator begin() { return start; }
		iterator end() { return finish; }
		reference front() { return *start; }
		reference back() { return *(finish - 1); }
		//�ͷŵ�δʹ�õ��ڴ�
		void shink_to_fit();
		void push_back(const T& x);
		void pop_back();
		iterator erase(iterator postion);
		iterator erase(iterator first, iterator last);
		iterator resize(size_type new_size, const T& x);
		iterator resize(size_type new_size);
		void	 reserve(size_type n);
		iterator insert(const_iterator pos, size_type n, const T& value);
		iterator insert(const_iterator pos, const T& value);
		void     clear();

	private:
		iterator allocate_and_fill(size_type n, const T& x);
		void allocate_and_copy(size_type n, iterator first, iterator last);
		void destroy_and_deallocate();
		
	};
}

#endif