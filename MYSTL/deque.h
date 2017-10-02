/*
    deque ˫�򿪿ڵ�����
    ͨ���п���map(�洢ָ�������)ά������������ڴ�ռ䣬ʹdequeʹ������������
    һ�������Ŀռ�
*/

#ifndef DEQUE_H_
#define DEQUE_H_

#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"
#include "algorithm.h"
#include "iterator.h"


#define MIN_NODES   8

namespace mySTL {
    inline size_t deque_buffer_size(size_t size) {
        return (size < 512 ? (512 / size) : size_t(1));
    }

    template <class T>
    class deque_iterator {
    public:
        typedef deque_iterator<T>           iterator;
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef ptrdiff_t                   difference_type;
        typedef T**                         map_pointer;
        typedef deque_iterator              self;

        T*  cur;            //ָ����ָ��������ǰԪ��
        T*  first;          //��Ԫ��
        T*  last;           //βԪ�غ�һ��
        map_pointer node;   //ָ���������

        static size_t buffer_size() {
            return deque_buffer_size(sizeof(T));
        }

        //��������ָ��ܿ����ĵ�ָ�룬�޸�first, lastָ�򻺳���
        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = *new_node + (difference_type)buffer_size();
        }

        reference operator* () const {
            return *cur;
        }

        pointer operator-> () const {
            return &(operator*());
        }

        difference_type operator- (const self& x) const {
            return difference_type((buffer_size() * (node - x.node - 1))
                + (cur - first) + (x.last - x.cur));
        }
        //ǰ��++
        self& operator++ () {
            ++cur;
            if (cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        //����++
        self operator++ (int) {
            self temp = *this;
            ++*this;
            return temp;
        }
        //ǰ��--
        self& operator-- () {
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        //����--
        self operator-- (int) {
            self temp = *this;
            --*this;
            return temp;
        }

        //ʵ�������ȡ
        self& operator+= (difference_type n) {
            difference_type offset = n + cur - first;
            size_t size = buffer_size();
            if (offset >= 0 && offset < size)
                cur += n;
            else {
                //����ͬһ��������
                difference_type node_offset = offset > 0 ?
                    offset / size :
                    -difference_type((-offset - 1) / size) - 1;
                //����node,����curָ��
                set_node(node + node_offset);
                cur = first + (offset - node_offset * size);//offset����������
            }
            return *this;
        }

        self operator+ (difference_type n) const {
            self temp = *this;
            return temp += n;
        }

        self& operator-= (difference_type n) {
            return *this += -n;
        }

        self operator- (difference_type n) const {
            self temp = *this;
            return temp -= n;
        }

        //����[]�������ȡ
        reference operator[] (difference_type n) const {
            return *(*this + n);
        }

        bool operator== (const self& x) { return cur == x.cur; }
        bool operator!= (const self& x) { return cur != x.cur; }
        bool operator< (const self& x) {
            return (node == x.node) ? (cur < x.cur) : (node < x.node);
        }
    };

    
    template <class T, class Alloc = allocator<T>>
    class deque {
    public:
        typedef T       value_type;
        typedef T*      pointer;
        typedef T&      reference;
        typedef size_t  size_type;
        typedef deque_iterator<T>  iterator;

    protected:
        typedef T**                 map_pointer;
        typedef Alloc               data_allocator; //ÿ�η���һ��Ԫ�ش�С
        typedef allocator<pointer>  map_allocator;//ÿ������һ��ָ���С

    protected:
        iterator start;         //��ʾ��һ���ڵ�
        iterator finish;        //���һ���ڵ�
        map_pointer map;        //ָ��map,map�е�Ԫ��Ϊָ�򻺳�����ָ��
        size_type map_size;     //map�ܴ��ָ�����

    public: //��������
        deque(size_type n, const value_type& val)
            :map(0), map_size(0) {
            fill_initialize(n, val);
        }
        
        deque(const deque& other);
        //template <class InputIterator>
        //deque(InputIterator first, InputIterator last);
        //deque& operator= (const deque& other);
        //~deque();

    public: //��Ա�������
        iterator begin() { return start; }
        iterator end() { return finish; }
        reference front() const { return *start }
        reference back() const { return *finish }
        reference operator[] (size_type n) {
            return start[n];
        }

    public:     //�������
        bool empty() const { return start == finish; }
        size_type size() const { return finish - start; }
        //�ͷŶ���ռ�
        void shrink_to_fit();
           
    public:     //�����������
        void clear();
        iterator insert(iterator pos, const value_type& val);
        iterator erase(iterator pos);
        void push_back(const T& val);
        void pop_back();
        void push_front(const T& val);
        void pop_front();
        void resize(size_type n);
        void swap();

    private:
        void fill_initialize(size_type n, const value_type& val);
        void create_map_and_nodes(size_type n);
    };
};


#include "Detail\deque.impl.h"
#endif
