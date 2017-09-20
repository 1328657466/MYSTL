#ifndef TYPE_TRAITS_H_
#define TYPE_TRAITS_H_

/***********************************************
	type_traits.h
	������Բ�ͬ���ͱ����ԣ��ڱ�������ɺ�������
	�����ͱ��Ƿ����non_trivial default ctor? non_trivial copy ctor?
	non_trivial assignment operator? non_trivial dtor? ������߱��������ڶ�
	��Щ�ͱ���и��ƣ���������������ֵʱ�����õ���constructor,destructor,��
	�����ڴ�ֱ�Ӳ�������malloc(),memcpy()��
*************************************************/


/*
	POD ���� ��Specifies that the type is POD (Plain Old Data) type. 
	This means the type is compatible with the types used in the C programming language,
	can be manipulated using C library functions: it can be created with std::malloc, 
	it can be copied with std::memmove, etc, and can be exchanged with C libraries directly, 
	in its binary form.			

	Requirements
		Either:
			a scalar type;
			a class type (class or struct or union) that is:
			an aggregate type;
			has no non-static members that are non-POD;
			has no members of reference type;
			has no user-defined copy constructor;
			has no user-defined destructor;
		(until C++11)
			a trivial type;
			a standard layout type;
			has no non-static members that are non-POD;
		(since C++11)
			an array of such type.
											//ժ��cpp reference
*/

namespace mySTL {
	//������ֻ�����class objec��ʽ�Ĳ����Ż���в����Ƶ�
	struct __true_type {};
	struct __false_type {};

	template <class type>
	struct __type_traits {
		typedef __true_type this_dummy_member_must_be_first;

		//�����ع��ƣ�Ĭ���ͱ�û��
		typedef __false_type has_trivial_default_constuctor;
		typedef __false_type has_trivial_copy_constructor;
		typedef __false_type has_trivial_assignment_operator;
		typedef __false_type has_trivial_destructor;
		typedef __false_type is_POD_type;
	};

	//�������C++�����������ػ�
	template <>
	struct __type_traits<bool> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<char> {
		typedef __true_type has_trivial_default_constructsor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned char> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<short> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned short> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<int> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned int> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<long> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned long> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<long long> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned long long> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<float> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<double> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<long double> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	//���ԭ��ָ����ƫ�ػ�
	template <class T>
	struct __type_traits<T*> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template<class T>
	struct __type_traits<const T*>
	{
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};
}

#endif