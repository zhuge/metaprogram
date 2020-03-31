//
//  type_traits_type.h
//  metaprogram
//
//  Created by Gong Wenzhu on 2020/03/27.
//  Copyright © 2020 Gong Wenzhu. All rights reserved.
//

#ifndef type_traits_type_h
#define type_traits_type_h

#include <cstdint>

#include "config.h"
#include "type_traits_helper.h"
#include "type_traits_cvrp.h"

NS_META_BEG

// Judge if two types is the same type (taking into account const/volatile qualifications), 
// provides the member constant value equal to true. Otherwise value is false.
// Example:
// 		static_assert(!is_same<int, float>::value, "int and float are not the same type")
template <class T, class U>
struct is_same : public false_type {
};

template <class T>
struct is_same<T,T> : public true_type {
};

/******************************* Primary type categories ******************
Provides the member constant value that is equal to true, if T is the type
1. P
2. const P
3. volatile P
4. const volatile P
**************************************************************************/
template <class T>
struct is_void : public is_same<void, typename remove_cv<T>::type> {
};

template <class T>
struct is_null_pointer : public is_same<std::nullptr_t, typename remove_cv<T>::type> {
};

// Checks whether T is an integral type. Provides the member constant value 
// which is equal to true, if T is the type bool, char, char8_t, char16_t, 
// char32_t, wchar_t, short, int, long, long long, or any implementation-defined 
// extended integer types, including any signed, unsigned, and cv-qualified 
// variants. Otherwise, value is equal to false.
template <class T>
struct is_integral : 
	public bool_constant<
		is_same<bool, typename remove_cv<T>::type>::value
		|| is_same<char, typename remove_cv<T>::type>::value
		|| is_same<unsigned char, typename remove_cv<T>::type>::value
		// || is_same<char8_t, typename remove_cv<T>::type>::value // c++20 has the type
		|| is_same<char16_t, typename remove_cv<T>::type>::value
		|| is_same<char32_t, typename remove_cv<T>::type>::value
		|| is_same<wchar_t, typename remove_cv<T>::type>::value
		|| is_same<short, typename remove_cv<T>::type>::value
		|| is_same<unsigned short, typename remove_cv<T>::type>::value
		|| is_same<int, typename remove_cv<T>::type>::value
		|| is_same<unsigned int, typename remove_cv<T>::type>::value
		|| is_same<long, typename remove_cv<T>::type>::value
		|| is_same<unsigned long, typename remove_cv<T>::type>::value
		|| is_same<long long, typename remove_cv<T>::type>::value
		|| is_same<unsigned long long, typename remove_cv<T>::type>::value
		|| is_same<std::int8_t, typename remove_cv<T>::type>::value
		|| is_same<std::int16_t, typename remove_cv<T>::type>::value
		|| is_same<std::int32_t, typename remove_cv<T>::type>::value
		|| is_same<std::int64_t, typename remove_cv<T>::type>::value
		|| is_same<std::int_fast8_t, typename remove_cv<T>::type>::value
		|| is_same<std::int_fast16_t, typename remove_cv<T>::type>::value
		|| is_same<std::int_fast32_t, typename remove_cv<T>::type>::value
		|| is_same<std::int_fast64_t, typename remove_cv<T>::type>::value
		|| is_same<std::int_least8_t, typename remove_cv<T>::type>::value
		|| is_same<std::int_least16_t, typename remove_cv<T>::type>::value
		|| is_same<std::int_least32_t, typename remove_cv<T>::type>::value
		|| is_same<std::int_least64_t, typename remove_cv<T>::type>::value
		|| is_same<std::intmax_t, typename remove_cv<T>::type>::value
		|| is_same<std::intptr_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint8_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint16_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint32_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint64_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint_fast8_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint_fast16_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint_fast32_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint_fast64_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint_least8_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint_least16_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint_least32_t, typename remove_cv<T>::type>::value
		|| is_same<std::uint_least64_t, typename remove_cv<T>::type>::value
		|| is_same<std::uintmax_t, typename remove_cv<T>::type>::value
		|| is_same<std::uintptr_t, typename remove_cv<T>::type>::value
		> {};

// Checks whether T is a floating-point type. Provides the member constant value
// which is equal to true, if T is the type float, double, long double, including
// any cv-qualified variants. Otherwise, value is equal to false.
template <class T>
struct is_floating_point :
    public bool_constant<
        is_same<float, typename remove_cv<T>::type>::value
        || is_same<double, typename remove_cv<T>::type>::value
        || is_same<long double, typename remove_cv<T>::type>::value
    > {};

// Checks whether T is an array type with bound. Provides the member constant value which is 
// equal to true, if T is an array type with bound. Otherwise, value is equal to false.
template <class T>
struct is_bound_array : public false_type {};

template <class T, size_t N>
struct is_bound_array<T[N]> : public true_type {};

// Checks whether T is an array type without bound. Provides the member constant value which is 
// equal to true, if T is an array type without bound. Otherwise, value is equal to false.
template <class T>
struct is_unbound_array : public false_type {};

template <class T>
struct is_unbound_array<T[]> : public true_type {};

// Checks whether T is an array type. Provides the member constant value which is 
// equal to true, if T is an array type. Otherwise, value is equal to false.
template <class T>
struct is_array : public bool_constant<
    is_bound_array<T>::value
    || is_unbound_array<T>::value> {};


namespace detail {
    // Implementation detail
    // 1. SFINAE
    // 2. if T is class/union then T::* is well-formed
    // 3. the int T::* don't mean T must has a member which type is int
    //      until we set it, like (int T::*m = &T::member), now we just
    //      do a declaration, so it is just fine
	template <class T> true_type is_class_or_union(int T::*);
	template <class T> false_type is_class_or_union(...);
}

// Checks whether T is a union class type.
// Implemention Note:
//      1. We can't distinguish between class and union
//      2. is_union need some compoiler feature so use the std version
template <class T>
using is_union = std::is_union<T>;

//Checks whether T is a non-union class type.
template <class T>
struct is_class : public bool_constant<decltype(detail::is_class_or_union<T>(0))::value && !is_union<T>::value> {};


// Checks whether T is a function type. Types like 
// 		1. std::function
//		2. lambdas
//		3. classes with overloaded operator() 
//		4. pointers to functions 
//		5. member functions
// don't count as function types. 
// Implementation Note:
// The cases are very many:
// 1. regular functions (1)
// 2. variadic functions (1)
// 3. functions with cv-qualifiers (4)
// 4. ref-qualifiers (3)
// 5. noexcept version functions (2)
// 6. sum = (1+1) * 4 * 3 * 2 = 48

// primary template
template <class T>
struct is_function : public false_type {};

// regular functions
template <class R, class... ARGS>
struct is_function<R(ARGS...)> : true_type {};

// variadic functions
template <class R, class... ARGS> 
struct is_function<R(ARGS..., ...)> : true_type {};

// cv-qualifiers
template <class R, class... ARGS> // const
struct is_function<R(ARGS...) const> : true_type {};

template <class R, class... ARGS> // const
struct is_function<R(ARGS...) volatile> : true_type {};

template <class R, class... ARGS> // const
struct is_function<R(ARGS...) const volatile> : true_type {};

template <class R, class... ARGS> // const
struct is_function<R(ARGS..., ...) const> : true_type {};

template <class R, class... ARGS> // const
struct is_function<R(ARGS...,...) volatile> : true_type {};

template <class R, class... ARGS> // const
struct is_function<R(ARGS...,...) const volatile> : true_type {};

// ref-qualifiers
template <class R, class... ARGS> // regular
struct is_function<R(ARGS...) &> : true_type {};

template <class R, class... ARGS> // const
struct is_function<R(ARGS...) const &> : true_type {};

template <class R, class... ARGS> // volatile
struct is_function<R(ARGS...) volatile &> : true_type {};

template <class R, class... ARGS> // const volatile
struct is_function<R(ARGS...) const volatile &> : true_type {};

template <class R, class... ARGS> // variadic
struct is_function<R(ARGS..., ...) &> : true_type {};

template <class R, class... ARGS> // const
struct is_function<R(ARGS..., ...) const &> : true_type {};

template <class R, class... ARGS> // volatile
struct is_function<R(ARGS..., ...) volatile &> : true_type {};

template <class R, class... ARGS> // const volatile
struct is_function<R(ARGS..., ...) const volatile &> : true_type {};

template <class R, class... ARGS> // regular
struct is_function<R(ARGS...) &&> : true_type {};

template <class R, class... ARGS> // const
struct is_function<R(ARGS...) const &&> : true_type {};

template <class R, class... ARGS> // volatile
struct is_function<R(ARGS...) volatile &&> : true_type {};

template <class R, class... ARGS> // const volatile
struct is_function<R(ARGS...) const volatile &&> : true_type {};

template <class R, class... ARGS> // variadic
struct is_function<R(ARGS..., ...) &&> : true_type {};

template <class R, class... ARGS> // const
struct is_function<R(ARGS..., ...) const &&> : true_type {};

template <class R, class... ARGS> // volatile
struct is_function<R(ARGS..., ...) volatile &&> : true_type {};

template <class R, class... ARGS> // const volatile
struct is_function<R(ARGS..., ...) const volatile &&> : true_type {};

// noexcept (C++17 and later)
// template <class R, class... ARGS> // regular
// struct is_function<R(ARGS...) noexcept> : true_type {};

// template <class R, class... ARGS>  // variadic
// struct is_function<R(ARGS..., ...) noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS...) const noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS...) volatile noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS...) const volatile noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS..., ...) const noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS..., ...) volatile noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS..., ...) const volatile noexcept> : true_type {};

// template <class R, class... ARGS> // regular
// struct is_function<R(ARGS...) & noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS...) const & noexcept> : true_type {};

// template <class R, class... ARGS> // volatile
// struct is_function<R(ARGS...) volatile & noexcept> : true_type {};

// template <class R, class... ARGS> // const volatile
// struct is_function<R(ARGS...) const volatile & noexcept> : true_type {};

// template <class R, class... ARGS> // variadic
// struct is_function<R(ARGS..., ...) & noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS..., ...) const & noexcept> : true_type {};

// template <class R, class... ARGS> // volatile
// struct is_function<R(ARGS..., ...) volatile & noexcept> : true_type {};

// template <class R, class... ARGS> // const volatile
// struct is_function<R(ARGS..., ...) const volatile & noexcept> : true_type {};

// template <class R, class... ARGS> // regular
// struct is_function<R(ARGS...) && noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS...) const && noexcept> : true_type {};

// template <class R, class... ARGS> // volatile
// struct is_function<R(ARGS...) volatile && noexcept> : true_type {};

// template <class R, class... ARGS> // const volatile
// struct is_function<R(ARGS...) const volatile && noexcept> : true_type {};

// template <class R, class... ARGS> // variadic
// struct is_function<R(ARGS..., ...) && noexcept> : true_type {};

// template <class R, class... ARGS> // const
// struct is_function<R(ARGS..., ...) const && noexcept> : true_type {};

// template <class R, class... ARGS> // volatile
// struct is_function<R(ARGS..., ...) volatile && noexcept> : true_type {};

// template <class R, class... ARGS> // const volatile
// struct is_function<R(ARGS..., ...) const volatile && noexcept> : true_type {};

// Checks whether T is a pointer to object or a pointer to function, except :
// 		1. pointer to member/member function
template <class T>
struct is_pointer : public false_type {};

template <class T>
struct is_pointer<T*> : public true_type {};

// Checks whether T is a lvalue reference type
template <class T>
struct is_lvalue_reference : public false_type {};

template <class T>
struct is_lvalue_reference<T&> : public true_type {};

// Checks whether T is a rvalue reference type
template <class T>
struct is_rvalue_reference : public false_type {};

template <class T>
struct is_rvalue_reference<T&&> : public true_type {};

// Checks whether T is a rvalue reference type
template <class T>
struct is_reference 
	: public bool_constant<
		is_lvalue_reference<T>::value
		|| is_rvalue_reference<T>::value
	> {};

// Checks whether T is a non-static member function.
template <class T>
struct is_member_function_pointer : public false_type {};

template <class T, class U>
struct is_member_function_pointer<T U::*> : public is_function<T> {};

// Checks whether T is a non-static member object.
template <class T>
struct is_member_object_pointer : false_type {};

template <class T, class U>
struct is_member_object_pointer<U T::*> : bool_constant<!is_member_function_pointer<U T::*>::value> {};

// Checks whether T is a non-static member pointer.
template <class T>
struct is_member_pointer : public false_type {};

template <class T, class U>
struct is_member_pointer<T U::*> : public true_type {};

// Checks whether T is an enumeration type. 
// Implemetation Note:
// 1. We can't implement it directly, so use the exclusion
template <class T>
struct is_enum :
	public bool_constant<
        ! is_void<T>::value
        && ! is_null_pointer<T>::value
        && ! is_integral<T>::value
        && ! is_floating_point<T>::value
        && ! is_array<T>::value
        && ! is_union<T>::value
        && ! is_class<T>::value
        && ! is_function<T>::value
        && ! is_pointer<T>::value
        && ! is_lvalue_reference<T>::value
        && ! is_rvalue_reference<T>::value
        && ! is_member_function_pointer<T>::value
        && ! is_member_object_pointer<T>::value
    > {};

/***************************** Composite type categories ******************
Provides the member constant value that is equal to true, if T is the type
1. P
2. const P
3. volatile P
4. const volatile P
**************************************************************************/

// Check if T is an arithmetic type (that is, 
//    	1. an integral type 
//		2. or a floating-point type)
// or a cv-qualified version thereof
template <class T>
struct is_arithmetic : 
	public bool_constant<
		is_integral<T>::value
		|| is_floating_point<T>::value
	>{};

// Check if T is a fundamental type (that is, 
//		1. arithmetic type
//		2. or void
//		3. or nullptr_t)
template <class T>
struct is_fundamental :
	public bool_constant<
		is_arithmetic<T>::value
		|| is_void<T>::value
		|| is_null_pointer<T>::value
	>{};

// Check if T is a scalar type (that is a possibly cv-qualified
//		1. arithmetic
//		2. pointer
//		3. pointer to member
//		4. enumeration
//		5. or std::nullptr_t type)
template <class T>
struct is_scalar :
	public bool_constant<
		is_arithmetic<T>::value
		|| is_pointer<T>::value
		|| is_member_pointer<T>::value
		|| is_enum<T>::value
		|| is_null_pointer<T>::value
	> {};

// Check if T is an object type (that is any possibly cv-qualified type other than
//		1. function
//		2. reference
//		3. or void types)
template <class T>
struct is_object :
	public bool_constant<
		!is_function<T>::value
		&& !is_reference<T>::value
		&& !is_void<T>::value
	> {};

// Check if T is a compound type (that is, 
//		1. array
//		2. function
//		3. object pointer
//		4. function pointer
//		5. member object pointer
//		6. member function pointer
//		7. reference
//		8. class
//		9. union
//		10. or enumeration, 
// including any cv-qualified variants)
template <class T>
struct is_compound : public bool_constant<!is_fundamental<T>::value> {};

NS_META_END

#endif // type_traits_type_h
