#ifndef RB_TREE_H_
#define RB_TREE_H_

#include "allocator.h"
#include "construct.h"
#include "iterator.h"

namespace mySTL {
    typedef bool rb_tree_color_type;
    const rb_tree_color_type rb_tree_red = false;
    const rb_tree_color_type rb_tree_black = true;

    //�����ü̳�
    template <class T>
    struct rb_tree_node {
        typedef rb_tree_color_type  color_type;
        typedef rb_tree_node*       link_type;
        
        color_type      color;
        link_type       parent;
        link_type       left;
        link_type       right;
        T               value;

        static link_type minimum(link_type* p) {
            while (p->left != nullptr)
                p = p->left;
            return p;
        }

        static link_type maximum(link_type* p) {
            while (p->right != nullptr)
                p = p->right;
            return p;
        }
    };

    template <class T>
    class rb_tree_iterator {
        typedef T                           value_type;
        typedef T&                          reference;
        typedef T*                          pointer;
        typedef rb_tree_iterator<T>         self;
        typedef rb_tree_iterator<T>         iterator;
        typedef rb_tree_node<T>*            link_type;
    public:     //����
        rb_tree_iterator() {}
        rb_tree_iterator(link_type x) { node = x; }
        rb_tree_iterator(const iterator& it) { node = it.node; }

    public:     //����
        reference operator* () const { return node->value; }
        pointer operator-> () const { return &(operator*()); }

        //ǰ��++
        self& operator++() {
            if (node->right != nullptr) {
                //�������������ӽڵ�
                node = node->right;
                while (node->left != nullptr)
                    node = node->left;
            }
            else {
                //û�����ӽڵ㣬��һֱ���ݣ�ֱ����ǰ�ڵ������ӽڵ�
                link_type p = node->parent;
                while (p->right == node) {
                    node = p;
                    p = p->parent;
                }
                if (node->right != p)
                    node = p;       //��ʱΪend
            }
            return *this;
        }

        //����++
        self operator++ (int) {
            self temp = *this;
            ++temp;
            return temp;
        }

        //ǰ��--
        self& operator-- () {
            //ͷ���
            if (node->color == rb_tree_red && node == node->parent->parent)
                node = node->right;
            else if (node->left != nullptr) {
                //����������
                link_type t = node->left;
                while (t->right != nullptr)
                    t = t->right;
                node = t;
            }
            else {
                //���ݣ�ֱ����ǰΪ���ӽڵ�
                link_type p = node->parent;
                while (p->left == node) {
                    p = node;
                    p = p->parent;
                }
                node = p;
            }
            return *this;
        }

        //����--
        self operator-- (int) {
            self t = *this;
            --t;
            return t;
        }

    public:
        link_type        node;
    };


}



#endif