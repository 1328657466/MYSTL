#ifndef RB_TREE_IMPL_H_
#define RB_TREE_IMPL_H_

namespace mySTL {
    //---------------------------------��������--------------------------------
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& 
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator= (const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {
        if (this != &x) {
            clear();
            node_count = 0;
            if (x.root() == nullptr) {
                header->parent = nullptr;
                leftmost() = header;
                rightmost() = header;
            }
            else {
                root() = __copy(x.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
                node_count = x.node_count;
            }
        }
        return *this;
    }

    //------------------------------��������-------------------------------
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
        if (node_count != 0) {
            __erase(root());
            root() = nullptr;
            node_count = 0;
            leftmost() = header;
            rightmost() = header;
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) {
        link_type p = header;
        link_type x = root();

        while (x != nullptr) {
            p = x;
            x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        }
        return __insert(x, p, v);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
        link_type p = header;
        link_type x = root();
        bool comp = true;
        while (x != nullptr) {
            p = x;
            comp = key_compare(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }

        iterator j(p);
        if (comp)
            if (p == begin()) { //�������㸸�ڵ�Ϊ�����ӽڵ�
                return pair<iterator, bool>(__insert(x, p, v), true);
            }
            else
                --j;    //��Сj�����Ƿ����ظ�

        //�½ڵ�Key���ظ�
        if (key_compare(key(j.node), KeyOfValue()(v)))
            return pair<iterator, bool>(__insert(x, p, v), true);
        //Key�ظ�
        return pair<iterator, bool>(j, false);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator pos) {
        link_type x = nullptr;
        link_type y = pos.node;     
        link_type z = y;            //Ҫɾ���Ľڵ�
        link_type px = nullptr;     //parent of x

        //��z�����������У���z�滻�������������ҽڵ�(����������ڵ��㷨����)
        if (y->left == nullptr)
            x = y->right;
        else if (y->right == nullptr)
            x = y->left;
        else {      //������������
            y = y->left;
            while (y->right != nullptr)
                y = y->right;
            x = y->left;
        }
        
        if (y == z) {   //����ɾ����pos���ֻ��һ������ʱ
            px = y->parent;
            if (x != nullptr)   //����һ������
                x->parent = z->parent;
            if (root() == z)
                root() = x;
            else if (z == z->parent->left)
                z->parent->left = x;
            else
                z->parent->right = x;

            if (z == leftmost())
                leftmost() = (x == nullptr ? z->parent : minimum(x));
            if (z == rightmost())
                rightmost() = (x == nullptr ? z->parent : maximum(x));
        }
        else {  //z����������
            //z���������ӵ�y��������
            z->right->parent = y;
            y->right = z->right;
            if (y != z->left) {
                //y���������ӵ�z��������������
                px = y->parent;
                z->left->right = y->left;
                if (x != nullptr)
                    x->parent = z->left;
                //z�������ӵ�y��������
                y->left = z->left;
                y = z->left->parent;
            }
            else
                px = y;
            
            if (z == root())
                root() = y;
            else if (z == z->parent->left)
                y = z->parent->left;
            else
                y = z->parent->right;

            y->parent = z->parent;
            mySTL::swap(y->color, z->color);
            y = z;
        }

        /* ������pos�Ľڵ��Ǻڽڵ�, ��Ϊ����ɾ���ˣ�
        ���԰���x��·������һ���ڽڵ㣬������ƽ�� */
        if (y->color == rb_tree_black)
            rb_tree_rebalance_for_erase(x, px);
        destroy_node(y);
        --node_count;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
        if (first == begin() && last == end())
            clear();
        else {
            while (first != last)
                erase(first++);
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x) {
        pair<iterator, iterator> p = equal_range(begin(), end(), x);
        erase(p.first, p.second);
    }
    //------------------------------�ײ�ʵ�ֺ���--------------------------
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x, link_type p) {
        if (x == nullptr)
            return nullptr;
        link_type root = clone_node(x);
        root->parent = p;
        root->right = __copy(x->right, x);
        root->left = __copy(x->left, x);
        return root;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x) {
        if (x != nullptr) {
            destroy_node(x);
            if (x->right != nullptr)
                __erase(x->right);
            if (x->left != nullptr)
                __erase(x->left);
        }
    }


    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_left(link_type x) {
        //xΪ��ת֧�㣬��x�ҽڵ�������y����x
        link_type y = x->right;
        x->right = y->left;

        //�踸�ڵ�
        if (y->left != nullptr)
            y->left->parent = x;
        y->parent = x->parent;

        if (x == root())
            root() = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        //���ӵߵ�
        y->left = x;
        x->parent = y;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_right(link_type x) {
        //xΪ��ת֧�㣬��x��ڵ�������y����x
        link_type y = x->left;
        x->left = y->right;

        if (y->right != nullptr)
            y->right->parent = x;
        y->parent = x->parent;

        if (x == root())
            root() = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        //���ӵߵ�
        y->right = x;
        x->parent = y;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(link_type x, link_type y, const value_type& v) {
        link_type z = creat_node(v);
        
        if (y == header || x != nullptr || key_compare(KeyOfValue()(v), key(y))) {
            //���
            left(y) = z;
            if (y == header) {
                root() = z;
                rightmost() = z;
            }
            else if (y == leftmost())
                leftmost() = z;
        }
        else {  //�Ҳ�
            right(y) = z;
            if (y == rightmost())
                rightmost() = z;
        }

        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;

        rb_tree_rebalance_for_insert(z);
        ++node_count;
        return iterator(z);
    } 

    /*
        ���ֲ�ƽ�⣺  
            1. ��ǰ�ڵ㸸�ڵ�Ϊ�죬����ڵ�Ϊ��
            2. ��ǰ�ڵ㸸�ڵ�Ϊ�죬����ڵ�Ϊ�ڣ���ǰ�ڵ�Ϊ���ڵ�����
            3. ��ǰ�ڵ㸸�ڵ�Ϊ�죬����ڵ�Ϊ�ڣ���ǰ�ڵ�Ϊ���ڵ�����
            
        ��Ӧ�ⷨ��     (���ڵ�Ϊ�游�ڵ�����������£� ����������)
            1. ���ڵ������ڵ�Ϳ�ڣ��游�ڵ�Ϳ�죬�游�ڵ�Ϊ�µĵ�ǰ�ڵ�
            2. ���ڵ�Ϊ�µĵ�ǰ�ڵ㣬���µĵ�ǰ�ڵ�Ϊ֧���������˺�ػ�
               ���³������3
            3. ���ڵ�Ϳ�ڣ��游�ڵ�Ϳ�죬���游�ڵ�Ϊ֧������
            
    */
    
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance_for_insert(link_type x) {
        x->color = rb_tree_red;
        while (x != root() && x->parent->color == rb_tree_red) {
            //���ڵ�Ϊ���ӽڵ�
            if (x->parent == x->parent->parent->left) {
                link_type uncle = x->parent->parent->right;
                if (uncle != nullptr && uncle->color == rb_tree_red) {
                    //case1
                    x->parent->color = rb_tree_black;
                    uncle->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    x = x->parent->parent;
                }
                else {
                    //case2
                    if (x == x->parent->right) {
                        x = x->parent;
                        rb_tree_rotate_left(x);
                    }
                    //case3
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    rb_tree_rotate_right(x->parent->parent);
                }
            }
            else {  //���ڵ�Ϊ����
                //case1
                link_type uncle = x->parent->parent->left;
                if (uncle != nullptr && uncle->color == rb_tree_red) {
                    uncle->color = rb_tree_black;
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    x = x->parent->parent;
                }
                else {
                    //case2
                    if (x == x->parent->left) {
                        x = x->parent;
                        rb_tree_rotate_right(x);
                    }
                    //case3
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    rb_tree_rotate(x->parent->parent);
                }
            }
        }
        root()->color = rb_tree_black;
    }

    //����x��·��������һ���ڽڵ�
    /*
        ���ֲ�ƽ�����: 
            1. ��ǰ�ڵ�Ϊ�ڣ��ֵ�Ϊ��
            2. ��ǰ�ڵ�Ϊ�ڣ��ֵ�Ϊ�ڣ��ֵܽڵ�������ӽڵ㶼Ϊ��
            3. ��ǰ�ڵ�Ϊ�ڣ��ֵ�Ϊ�ڣ��ֵ�����Ϊ�죬�ֵ�����Ϊ��
            4. ��ǰ�ڵ�Ϊ�ڣ��ֵ�Ϊ�ڣ��ֵ�����Ϊ�죬����Ϊ����ɫ
        ��Ӧ�ⷨ: (��ǰ�ڵ�Ϊ������ʱ��Ϊ�������㷨����)
            1. �ֵ�Ϳ�ڣ����ڵ�Ϳ�죬�Ե�ǰ�ڵ㸸�ڵ�Ϊ֧������(ת��Ϊ
               �ֵܽڵ�Ϊ�ڵ����)
            2. �ֵ�Ϳ�죬��ǰ�ڵ��Ƶ���ǰ�ڵ㸸�ڵ�
            3. �ֵ�Ϳ�죬�ֵ�����Ϳ�ڣ����ֵ�Ϊ֧��������ת�������
            4. ��ǰ�ڵ��ֵܽڵ�Ϳ�ɸ��ڵ���ɫ�����ڵ�Ϳ�ڣ��ֵ�����Ϳ��
               �Ե�ǰ�ڵ㸸�ڵ�Ϊ֧��������
        �ⷨ������Ŀ�Ķ���Ϊ�˰ѵ�ǰ�ڵ㸸�ڵ�֮�µ�·���ĺڽڵ�������ƽ��(�ٸ���
        �ӣ�����ǰ�ڵ�Ϊ����������Ϊ֮ǰɾ���ĵ�ǰ�ڵ�ĸ��ڵ��Ǻ�ɫ�ģ������ڵ�
        ǰ�ڵ�ĸ��ڵ��������е�·���Ϻ�ɫ�ڵ�ĸ���������1������ͨ���任�ѵ�ǰ
        �ڵ㸸�ڵ������������еĺ�ɫ�ڵ�Ҳ����1���������ǰ�ڵ㸸�ڵ������·��
        �ڽڵ�����1�� �ѵ�ǰ�ڵ��Ƶ���ǰ�ڵ㸸�ڵ㣬���Ϊ�����Ϊ���������Ϳ��
        ���������Ϊ���������Щ����)
    */
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance_for_erase(link_type x, link_type parent_of_x) {
        while (x != root() && (x == nullptr || x->color == rb_tree_black)) {
            //��ǰ�ڵ�Ϊ������
            if (x == parent_of_x->right) {
                link_type silbing = parent_of_x->left;
                // case1
                if (silbing->color == rb_tree_red) {
                    silbing->color = rb_tree_black;
                    parent_of_x->color = rb_tree_red;
                    rb_tree_rotate_right(parent_of_x);
                    silbing = parent_of_x->left;
                }

                // case2
                if ((silbing->left == nullptr || silbing->left->color == rb_tree_black) && 
                    (silbing->right == nullptr || silbing->right->color == rb_tree_black)) {
                    silbing->color = rb_tree_red;
                    x = parent_of_x;
                    parent_of_x = parent_of_x->parent;
                }
                else {
                    //case3
                    if (silbing->left == nullptr ||
                        silbing->left->color == rb_tree_black) {
                        //if (silbing->right != nullptr)
                        silbing->right->color = rb_tree_black;
                        silbing->color = rb_tree_red;
                        rb_tree_rotate_left(silbing);
                        silbing = parent_of_x->left;
                    }
                    //case4
                    silbing->color = parent_of_x->color;
                    parent_of_x->color = rb_tree_black;
                    silbing->left->color = rb_tree_black;
                    rb_tree_rotate_right(parent_of_x);
                    break;
                }
            }
            else {      //��ǰ�ڵ�Ϊ������
                link_type silbing = parent_of_x->right;
                // case1
                if (silbing->color == rb_tree_red) {
                    silbing->color = rb_tree_black;
                    parent_of_x->color = rb_tree_red;
                    rb_tree_rotate_left(parent_of_x);
                    silbing = parent_of_x->right;
                }

                // case2
                if ((silbing->left == nullptr || silbing->left->color == rb_tree_black) &&
                    (silbing->right == nullptr || silbing->right->color == rb_tree_black)) {
                    silbing->color = rb_tree_red;
                    x = parent_of_x;
                    parent_of_x = parent_of_x->parent;
                }
                else {
                    // case3
                    if (silbing->right == nullptr ||
                        silbing->right->color == rb_tree_black) {
                        silbing->color = rb_tree_red;
                        silbing->left->color = rb_tree_red;
                        rb_tree_rotate_right(silbing);
                        silbing = parent_of_x->right;
                    }
                    // case4
                    silbing->color = parent_of_x->color;
                    parent_of_x->color = rb_tree_black;
                    silbing->right->color = rb_tree_black;
                    rb_tree_rotate_left(parent_of_x);
                    break;
                }
            }
        }

        if (x != nullptr)
            x->color = rb_tree_black;
    }
}

#endif