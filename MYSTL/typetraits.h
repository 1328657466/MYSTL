#ifndef TYPE_TRAITS_
#define TYPE_TRAITS_

/***********************************************
	type_traits.h
	������Բ�ͬ���ͱ����ԣ��ڱ�������ɺ�������
	�����ͱ��Ƿ����non_trivial default ctor? non_trivial copy ctor?
	non_trivial assignment operator? non_trivial dtor? ������߱��������ڶ�
	��Щ�ͱ���и��ƣ���������������ֵʱ�����õ���constructor,destructor,��
	�����ڴ�ֱ�Ӳ�������malloc(),memcpy()��
*************************************************/

namespace mySTL {
	//������ֻ�����class objec��ʽ�Ĳ����Ż���в����Ƶ�
	struct __true_type {};
	struct __false_type {};

	template <class type>
	struct __type_traits {
		typedef __true_type this_dummu_member_must_be_first;

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
}

#endif