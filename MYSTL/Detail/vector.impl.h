#ifndef VECTOR_IMPL_H_
#define VECTOR_IMPL_H_

namespace mySTL {
/********************�����������***********************/
    template <class T, class Alloc>
    vector<T, Alloc>::vector(const size_type n) {
        allocate_and_fill(n, value_type());
    }

    template <class T, class Alloc>
    vector<T, Alloc>::vector(const size_type n, const T& value) {
        allocate_and_fill(n, value);
    }

    template <class T, class Alloc>
    vector<T, Alloc>::vector(const vector<T, Alloc>& v) {
        allocate_and_copy(v.size(), v.start, v.finish);
    }

    template <class T, class Alloc>
    template <class InputIterator>
    vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
        vector_aux(first, last, typename std::is_integral<InputIterator>::type());
    }

    template <class T, class Alloc>
    vector<T, Alloc>::~vector() {
        destroy_and_deallocate();
    }

    template <class T, class Alloc>
    vector<T>& vector<T, Alloc>::operator= (const vector<T>& v) {
        if (this != &v) {
            allocate_and_copy(v.size(), v.begin(), v.end());
        }
        return *this;
    }

    /********************�������******************************/
    template <class T, class Alloc>
    void vector<T, Alloc>::reserve(size_type new_cap){
        if (new_cap > capacity()) {
            iterator old_start = start;
            iterator old_finish = finish;
            size_type old_cap = capacity();
            allocate_and_copy(new_cap, old_start, old_finish);
            //������ ����ԭ���ռ�
            destroy(old_start, old_finish);
            data_allocator::deallocate(old_start, old_cap);
        }
    }
    //���ն���ռ�ʹ������size��ͬ
    template <class T, class Alloc>
    void vector<T, Alloc>::shrink_to_fit() {
        data_allocator::deallocate(finish, end_of_storage - finish);
        end_of_storage = finish;
    }


    /*************************�����������*****************************/
    template <class T, class Alloc>
    void vector<T, Alloc>::clear() {
        destroy_and_deallocate();
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert(iterator pos, size_type n, const T& val) {
        difference_type dis = pos - start;
        if (n != 0) {
            if (end_of_storage - finish >= n) {
                //���ÿռ����
                const size_type elems_after = finish - pos;
                iterator old_finish = finish;
                if (elems_after > n) {
                    //�������Ԫ�ظ�����������Ԫ�ظ���
                    uninitialized_copy(finish - n, finish, finish);
                    finish += n;
                    std::copy_backward(pos, old_finish - n, old_finish);
                    std::fill(pos, pos + n, val);
                }
                else {
                    //�������Ԫ��С�ڵ�������Ԫ�ظ���
                    uninitialized_fill_n(finish, n - elems_after, val);
                    finish += n - elems_after;
                    uninitialized_copy(pos, old_finish, finish);
                    finish += elems_after;
                    std::fill(pos, old_finish, val);
                }
            }
            else {
                //���ÿռ�С������Ԫ�ظ���,�ط��䣬�����վ��ڴ�
                const size_type old_size = size();
                const size_type len = old_size + max(n, old_size);
                iterator new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;

                new_finish = uninitialized_copy(start, pos, new_start);
                new_finish = uninitialized_fill_n(new_finish, n, val);
                new_finish = uninitialized_copy(pos, finish, new_finish);

                destroy(start, finish);
                data_allocator::deallocate(start, capacity());

                start = new_start;
                finish = new_finish;
                end_of_storage = start + len;
            }
        }

        return start + dis;
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator  
    vector<T, Alloc>::insert(iterator pos, const T& val) {
        return insert(pos, 1, val);
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::erase(iterator pos) {
        if (pos + 1 != finish)
            std::copy(pos + 1, finish, pos);
        --finish;
        destroy(finish);
        return pos;
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(iterator first, iterator last) {
        iterator t = std::copy(last, finish, first);
        destroy(t, finish);
        finish = finish - (last - first);
        return first;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::push_back(const T& val) {
        insert(finish, val);
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::pop_back() {
        erase(finish - 1);
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::resize(size_type n) {
        if (n <= size()) {
            erase(start + n, finish);
        }
        else {
            if (n <= capacity()) {
                finish = uninitialized_fill_n(finish, n - size(), T());
            }
            else {
                iterator old_start = start;
                iterator old_finish = finish;
                size_type old_cap = capacity();
                start = data_allocator::allocate(n);
                finish = uninitialized_copy(old_start, old_finish, start);
                finish = uninitialized_fill_n(finish, n - size(), T());
                end_of_storage = finish;

                destroy(old_start, old_finish);
                data_allocator::deallocate(old_start, old_cap);
            }
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::swap(vector<T>& other) {
        mySTL::swap(start, other.start);
        mySTL::swap(finish, other.finish);
        mySTL::swap(end_of_storage, other.end_of_storage);
    }

    /*************************��С�Ƚ����**************************/
    template <class T, class Alloc>
    bool vector<T, Alloc>::operator== (const vector<T, Alloc>& v) {
        if (size() == v.size()) {
            iterator first1 = start;
            iterator last1 = finish;
            iterator first2 = v.start;
            iterator last2 = v.finish;

            while (first1 != last1) {
                if (*first1++ != *first2++)
                    return false;
            }

            return true;
        }
        else
            return false;
    }

    template <class T, class Alloc>
    bool vector<T, Alloc>::operator> (const vector<T, Alloc>& v) {
        iterator first1 = start;
        iterator last1 = finish;
        iterator first2 = v.start;
        iterator last2 = v.finish;
        while (first1 != last1 && first2 != last2) {
            if (*first1++ < *first2++)
                return false;
        }
        return (first1 != last1) && (first2 == last2);
    }

    template <class T, class Alloc>
    bool vector<T, Alloc>::operator< (const vector<T, Alloc>& v) {
        iterator first1 = start;
        iterator last1 = finish;
        iterator first2 = v.start;
        iterator last2 = v.finish;
        while (first1 != last1 && first2 != last2) {
            if (*first1++ > *first2++)
                return false;
        }
        return (first1 == last1) && (first2 != last2);
    }

    template <class T, class Alloc>
    bool vector<T, Alloc>::operator!= (const vector<T, Alloc>& v) {
        return !(*this == v);
    }

    template <class T, class Alloc>
    bool vector<T, Alloc>::operator<= (const vector<T, Alloc>& v) {
        return !(*this > v);
    }

    template <class T, class Alloc>
    bool vector<T, Alloc>::operator>= (const vector<T, Alloc>& v) {
        return !(*this < v);
    }



    /*****************˽�к���******************/
    template <class T, class Alloc>
    void vector<T, Alloc>::allocate_and_fill(size_type n, const T& value) {
        start = data_allocator::allocate(n);
        uninitialized_fill_n(start, n, value);
        finish = start + n;
        end_of_storage = finish;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::allocate_and_copy(size_type n, iterator begin, iterator end) {
        start = data_allocator::allocate(n);
        finish = uninitialized_copy(begin, end, start);
        end_of_storage = start + n;
    }
 
    template <class T, class Alloc>
    void vector<T, Alloc>::destroy_and_deallocate() {
        destroy(start, finish);
        data_allocator::deallocate(start, capacity());
        finish = start;
        end_of_storage = start;
    }

    template <class T, class Alloc>
    template <class InputIterator>
    void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type) {
        allocate_and_copy(last - first, first, last);
    }

    template <class T, class Alloc>
    template <class Integer>
    void vector<T, Alloc>::vector_aux(Integer n, const value_type &val, std::true_type) {
        allocate_and_fill(n, val);
    }
    

}


#endif