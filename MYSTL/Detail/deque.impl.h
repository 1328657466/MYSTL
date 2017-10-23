#ifndef DEQUE_IMPL_H_
#define DEQUE_IMPL_H_

namespace mySTL {
    /**********************���죬�������***********************/
    template <class T, class Alloc>
    deque<T, Alloc>::deque(const deque& other) {
        size_type deqsize = deque_buffer_size(sizeof(T));
        create_map_and_nodes(other.size());
        map_pointer res = start.node;
        for (map_pointer cur = other.start.node; cur < other.finish.node; ++cur)
            uninitialized_copy(*cur, *cur + deqsize, *res++);
        uninitialized_copy(other.finish.first, other.finish.cur, *res);
    }

    template <class T, class Alloc>
    template <class InputIterator>
    deque<T, Alloc>::deque(InputIterator first, InputIterator last) {
        deque_aux(first, last, typename std::is_integral<InputIterator>::type());
    }

    template <class T, class Alloc>
    deque<T, Alloc>& deque<T, Alloc>::operator= (deque<T, Alloc>& other) {
        size_type len = other.size();
        if (&other != this) {
            if (len <= size()) {
                erase(std::copy(other.begin(), other.end(), start), finish);
            }
            else {
                iterator mid = other.begin() + size();
                std::copy(other.begin(), mid, start);
                insert(finish, mid, other.finish);
            }
        }
        return *this;
    }

    template <class T, class Alloc>
    deque<T, Alloc>::~deque() {
        destroy_and_deallocate();
    }

    /*********************�����������*****************************/
    //����Ҫ����һ���������� ����clear ��push_back����
    template <class T, class Alloc>
    void deque<T, Alloc>::clear() {
        size_type bufsize = deque_buffer_size(sizeof(T));
        for (map_pointer cur = start.node + 1; cur < finish.node; ++cur) {
            destroy(*cur, *cur + bufsize);
            data_allocator::deallocate(*cur, bufsize);
        }
        //����������������
        if (start.node != finish.node) {
            destroy(start.first, start.last);
            destroy(finish.first, finish.cur);
            //����β������
            data_allocator::deallocate(finish.first, bufsize);
        }
        else {
            destroy(start.first, start.cur);
        }
        
        finish = start;
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::push_back(const value_type& val) {
        if (finish.cur != finish.last - 1) {
            construct(finish.cur, val);
            ++finish.cur;
        }
        else     //ֻʣһ��Ԫ�ؿռ�, finishҪ����
            push_back_aux(val);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::push_front(const value_type& val) {
        if (start.cur != start.first) {
            construct(start.cur - 1, val);
            --start.cur;
        }
        else   //ֻʣһ��Ԫ�ؿռ�, startҪ����
            push_front_aux(val);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::pop_back() {
        if (finish.cur != finish.first) {
            --finish;
            destroy(finish.cur);
        }
        else
            pop_back_aux();
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::pop_front() {
        if (start.cur != start.last - 1) {
            destroy(start.cur);
            ++start;
        }
        else
            pop_front_aux();
    }

    template <class T, class Alloc>
    template <class InputIterator>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last) {
        return insert_aux(pos, first, last, typename std::is_integral<InputIterator>::type());
    }

    template <class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert(iterator pos, size_type n, const value_type& val) {
        //�����ͷ��
        difference_type elems_before = pos - start;
        if (n <= 0)
            return pos;
        if (start.cur == pos.cur) {
            difference_type left = start.cur - start.first;
            if (left < n)
                new_elements_at_front(n - left);
            start -= n;
            std::fill_n(start, n, val);
            //uninitialized_fill_n(start, n, val);
        }
        else if (finish.cur == pos.cur) {  //�����β��
            size_type left = finish.last - finish.cur;
            if (left < n)
                new_elements_at_back(n - left);
            //uninitialized_fill_n(finish, n, val);
            finish += n;
        }
        else 1; //�м� 
            //insert_aux(pos, n, val);
        return start + elems_before;
    }

    template <class T, class Alloc>
    typename deque<T, Alloc>::iterator 
    deque<T, Alloc>::erase(iterator pos) {
        //�ı�Ԫ�����ƶ��ı�
        iterator next = pos;
        ++next;
        difference_type index = pos - start;
        if (index < (size() >> 1)) {
            std::copy_backward(start, pos, next);
            pop_front();
        }
        else {
            std::copy(next, finish, pos);
            pop_back();
        }
        return start + index;
    }

    template <class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::erase(iterator first, iterator last) {
        if (first == start && last == finish) {
            clear();
            return finish;
        }
        else {      //�ı����ƶ��ı�
            difference_type n = last - first;
            difference_type elems_before = first - start;
            if (elems_before < (size() << 1)) {
                std::copy_backward(start, first, last);
                iterator new_start = start + n;
                destroy(start, new_start);
                //���տռ�
                for (map_pointer cur = start.node; cur < new_start.node; ++cur)
                    data_allocator::deallocate(*cur, deque_buffer_size(sizeof(T)));
                start = new_start;
            }
            else {
                std::copy(last, finish, first);
                iterator new_finish = finish - n;
                destroy(new_finish, finish);
                //���տռ�
                for (map_pointer cur = finish.node; cur > new_finish.node; --cur)
                    data_allocator::deallocate(*cur, deque_buffer_size(sizeof(T)));
                finish = new_finish;
            }

            return start + elems_before;
        }
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::swap(deque<T, Alloc>& other) {
        mySTL::swap(start, other.start);
        mySTL::swap(finish, other.finish);
        mySTL::swap(map, other.map);
        mySTL::swap(map_size, other.map_size);
    }

    /**************************�ײ��������************************/

    template <class T, class Alloc>
    void deque<T, Alloc>::fill_initialize(size_type n, 
        const value_type& val){
        //����ռ����ýڵ�
        create_map_and_nodes(n);
        //uninitialized_fill() bug fixed 2017.10.02
        for (map_pointer cur = start.node; cur < finish.node; ++cur)
            uninitialized_fill(*cur, *cur + deque_buffer_size(sizeof(T)), val);
        uninitialized_fill(finish.first, finish.cur, val);
    }

    //����ռ����ýڵ�
    template <class T, class Alloc>
    void deque<T, Alloc>::create_map_and_nodes(size_type n) {
        size_type bufsize = deque_buffer_size(sizeof(T));
        size_type num_nodes = n / bufsize + 1;
        map_size = MIN_NODES > (num_nodes + 2) ? MIN_NODES : (num_nodes + 2);
        map = map_allocator::allocate(map_size);
        //ʹnstart, nfinishָ��nodes�м����Σ�ʹͷβ�����㹻�ռ�
        map_pointer nstart = map + ((map_size - num_nodes) >> 1);
        map_pointer nfinish = nstart + num_nodes - 1;

        for (map_pointer cur = nstart; cur <= nfinish; ++cur)
            *cur = data_allocator::allocate(bufsize);
        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.first + n % bufsize;
    }

    template <class T, class Alloc>
    template <class InputIterator>
    void deque<T, Alloc>::deque_aux(InputIterator first, InputIterator last, std::false_type) {
        create_map_and_nodes(0);
        for (; first != last; ++first)
            push_back(*first);
    }

    template <class T, class Alloc>
    template <class Integer>
    void deque<T, Alloc>::deque_aux(Integer n, const value_type& val, std::true_type) {
        fill_initialize(n, val);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::destroy_and_deallocate() {
        size_type bufsize = deque_buffer_size(sizeof(T));
        destroy(start, finish);
        map_pointer cur = start.node;
        for (; cur != finish.node; ++cur)
            data_allocator::deallocate(*cur, bufsize);
        data_allocator::deallocate(*cur, bufsize);
        map_allocator::deallocate(map, map_size);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::push_back_aux(const value_type& val) {
        reserve_map_at_back();      //����������map
        size_type bufsize = deque_buffer_size(sizeof(T));
        *(finish.node + 1) = data_allocator::allocate(bufsize);

        construct(finish.cur, val);
        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::push_front_aux(const value_type& val) {
        reserve_map_at_front();
        size_type bufsize = deque_buffer_size(sizeof(T));
        *(start.node - 1) = data_allocator::allocate(bufsize);
        start.set_node(start.node - 1);
        start.cur = start.last - 1;
        construct(start.cur, val);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::pop_back_aux() {
        data_allocator::deallocate(finish.first, deque_buffer_size(sizeof(T)));
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        destroy(finish.cur);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::pop_front_aux() {
        destroy(start.cur);
        data_allocator::deallocate(start.first, deque_buffer_size(sizeof(T)));
        start.set_node(start.node + 1);
        start.cur = start.first;
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::new_elements_at_front(size_type n) {
        size_type bufsize = deque_buffer_size(sizeof(T));
        size_type nodes_to_add = (n + bufsize - 1) / bufsize;
        reserve_map_at_front(nodes_to_add);
        for (int i = 1; i <= nodes_to_add; ++i)
            *(start.node - i) = data_allocator::allocate(bufsize);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::new_elements_at_back(size_type n) {
        size_type bufsize = deque_buffer_size(sizeof(T));
        size_type nodes_to_add = (n + bufsize - 1) / bufsize;
        reserve_map_at_back(nodes_to_add);
        for (int i = 1; i <= nodes_to_add; ++i)
            *(finish.node + i) = data_allocator::allocate(bufsize);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::reserve_map_at_back(size_type nodes_to_add = 1) {
        //mapβ�˱��ýڵ㲻�㣬�ٷ���map
        if (nodes_to_add + 1 > map_size - (finish.node - map))
            reallocate_map(nodes_to_add, false);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::reserve_map_at_front(size_type nodes_to_add = 1) {
        //mapͷ�����ýڵ㲻�㣬�ٷ���
        if (nodes_to_add > start.node - map)
            reallocate_map(nodes_to_add, true);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
        size_type old_num_nodes = finish.node - map + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_start;
        if (map_size > (new_num_nodes >> 1)) {
            //map_size����Ҫ���������� �����ٷ���map
            //��ʹ�õĽڵ�λ��map��������
            new_start = map + ((map_size - new_num_nodes) >> 1) +
                (add_at_front ? nodes_to_add : 0);
            if (new_start < start.node)
                std::copy(start.node, finish.node + 1, new_start);
            else
                std::copy_backward(start.node, finish.node + 1, new_start + old_num_nodes);
        }
        else {  //�����¿ռ䣬 �������¿ռ䣬 ���վɿռ�
            size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_start = new_map + ((new_map_size - new_num_nodes) >> 1) + (add_at_front ? nodes_to_add : 0);
            std::copy(start.node, finish.node + 1, new_start);
            map_allocator::deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        }

        //����start, finish
        start.set_node(new_start);
        finish.set_node(new_start + old_num_nodes - 1);
    }

    template <class T, class Alloc>
    typename deque<T, Alloc>::iterator 
    deque<T, Alloc>::insert_aux(iterator pos, size_type n, const value_type& val) {
        difference_type elems_before = pos - start;
        size_type length = size();
        if (elems_before < (length >> 1)) {
            start = reserve_elems_at_front(n);
            iterator old_start = start + n;
            pos = start + elems_before;
           std::copy(old_start, old_start + elems_before, start);
           std::fill(pos, pos + n, val);
        }
        else {
            size_type elems_after = length - elems_before;
            finish = reserve_elems_at_back(n);
            iterator old_finish = finish - n;

            pos = old_finish - elems_after;
            std::copy_backward(pos, old_finish, finish);
            std::fill(pos, pos + n, val);
        }
        return pos;
    }

    template <class T, class Alloc>
    template <class InputIterator>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type) {
        difference_type elems_before = pos - start;
        difference_type n = last - first;
        //��ͷ����
        if (pos.cur == start.cur) {
            size_type left = start.cur - start.first;
            if (n > left)
                reserve_elems_at_front(n - left);
            start -= n;
            std::copy(first, last, start);
        }   
        else if (pos.cur == finish.cur) {
            //��β����
            size_type left = finish.last - pos.cur;
            if (n > left)
                reserve_elems_at_back(n - left);
            finish += n;
            std::copy_backward(first, last, finish);
        }
        else
            insert_aux(pos, first, last, n);
        return start + elems_before;
    }

    template <class T, class Alloc>
    template <class Integer>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert_aux(iterator pos, Integer n, const value_type& val, std::true_type) {
        //����ת��Ϊsize_type���ͣ���ƥ�����inputiterator �汾�������ĵ����ߣ�������ѭ��
        return insert(pos, (size_type)n, val);
    }

    template <class T, class Alloc>
    template <class InputIterator>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::insert_aux(iterator pos, InputIterator first, InputIterator last, size_type n) {
        difference_type elems_before = pos - start;
        size_type size = this->size();
        if (elems_before < (size >> 1)) {
            start = reserve_elems_at_front(n);
            iterator old_start = start + n;
            pos = old_start + elems_before;
            std::copy(old_start, pos, start);
            std::copy(first, last, pos - n);
        }
        else {
            size_type elems_after = size - elems_before;
            finish = reserve_elems_at_back(n);
            iterator old_finish = finish - n;

            pos = old_finish - elems_after;
            std::copy_backward(pos, old_finish, finish);
            std::copy(first, last, pos);
        }
        return start + elems_before;
    }

    template <class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::reserve_elems_at_front(size_type n) {
        size_type vacancies = start.cur - start.first;
        //��ȱλ����
        if (vacancies < n)
            new_elements_at_front(n - vacancies);
        return start - n;
    }

    template <class T, class Alloc>
    typename deque<T, Alloc>::iterator
    deque<T, Alloc>::reserve_elems_at_back(size_type n) {
        size_type vacancies = finish.last - finish.cur;
        //��ȱλ����
        if (vacancies < n)
            new_elements_at_back(n - vacancies);
        return finish + n;
    }
}

#endif