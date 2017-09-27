#ifndef  LIST_H_
#define  LIST_H_

#include "allocator.h"
#include "construct.h"
#include "iterator.h"      

namespace mySTL {
    //list �ڵ�
    template <class T>
    struct __list_node {
        __list_node<T>*     prev;
        __list_node<T>*     next;
        T                   data;
    };

    //list ������
    template <class T>
    class list_iterator {
    public:
        typedef list_iterator<T>    iterator;
        typedef list_iterator<T>    self;

        typedef bidirectional_iterator_tag  iterator_tag;
        typedef T                           value;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef __list_node<T>*             link_type;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;

        link_type   node;   //ָ��list�ڵ�

        list_iterator(link_type x) : node(x) {}
        list_iterator() {}
        list_iterator(const iterator& x) :node(x.node) {}

        bool operator== (const self& x) const {
            return node == x.node;
        }

        bool operator!= (const self& x) const {
            return node != x.node;
        }

        reference operator*() const {
            return (*node).data;
        }

        pointer operator-> () const {
            return &(operator*());
        }
        //ǰ��++
        self& operator++ () {
            node = (link_type)((*node).next);
            return *this;
        }
        //����++
        self operator++ (int) {
            self temp = *this;
            ++*this;
            return temp;
        }

        self& operator-- () {
            node = (link_type)((*node).prev);
            return *this;
        }

        self operator-- (int) {
            self temp = *this;
            --*this;
            return temp;
        }
    };
    
    //list �����ݽṹ ��״˫������
    template <class T, class Alloc = allocator<__list_node<T>>>
    class list {
    protected:
        typedef __list_node<T>      list_node;
        //ÿ�η���һ���ڵ�
        typedef Alloc               list_node_allocator;

    public:
        typedef T                   value_type;
        typedef value_type*         pointer;
        typedef const value_type*   const_pointer;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef size_t              size_type;
        typedef ptrdiff_t           difference_type;

        typedef list_node*          link_type;
        typedef list_iterator<T>    iterator;

    protected:          //node����ʾ����ת��Ϊ list_iterator<T>
        link_type   node;   //ָ��սڵ��γ�ǰ�պ����䣬nodeΪend()����ֵ

    public:     //���죬��������
        list() { empty_initialize(); }
        ~list();
        list(const list<T, Alloc>& other);
    public:         //��Ա����
        iterator begin() {
            return node->next;
        }

        iterator end() { return node; };
        bool empty() const { return node->next == node; }
        size_type size() const;
        reference front() { return *begin(); }
        reference back() { return *(--end()); }
        void push_front(const T& x);
        void push_back(const T& x);
        void pop_front();
        void pop_back();
        void clear();
        iterator insert(iterator pos, const T& val);
        iterator erase(iterator pos);
        void splice(iterator pos, list<T, Alloc>& x);
        void remove(const T& value);
        void merge(list<T, Alloc>& x);
        void reverse();
        void swap(list<T, Alloc>& x);
        //���ù鲢�����㷨
        void sort();

    protected:      //���ã��ͷţ����죬���ٽڵ�
        link_type get_node() { return list_node_allocator::allocate(); }

        void deallocate_node(link_type p) {
            list_node_allocator::deallocate(p);
        }

        link_type create_node(const T& x) {
            link_type p = get_node();
            construct(&p->data, x);
            return p;
        }
        void destroy_node(link_type p) {
            destroy(&p->data);
            deallocate_node(p);
        }

        void empty_initialize() {
            node = get_node();
            node->next = node;
            node->prev = node;
        }
        //[first, last)�ڵ�����Ԫ���ƶ���pos֮ǰ
        void transfer(iterator pos, iterator first, iterator last);
    };


}


#include "Detail\list.impl.h"
#endif // ! LIST_H_
