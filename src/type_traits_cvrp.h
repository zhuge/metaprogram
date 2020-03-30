//
//  type_traits_cv.h
//  metaprogram
//
//  Created by Gong Wenzhu on 2020/03/27.
//  Copyright © 2020 Gong Wenzhu. All rights reserved.
//

#ifndef type_traits_cv_h
#define type_traits_cv_h

#include "config.h"
#include "type_traits_helper.h"

NS_META_BEG

// Remove topmost const, the topmost volatile if present
// Examples:
//      static_assert(is_same<int, remove_cv<const volatile int>::type>(), 
//                  "remove_cv<const volatile int>::type is same as int");
template <class T>
struct remove_const : type_identity<T> {};

template <class T>
struct remove_const<const T> : type_identity<T> {};

template <class T>
struct remove_volatile : type_identity<T> {};

template <class T>
struct remove_volatile<volatile T> : type_identity<T> {};

template <class T>
using remove_cv = remove_const<typename remove_volatile<T>::type>;

// Add cv-qualifier to the type T, except it has a cv-qualifier added 
// (unless T is a function,  a reference, or already has this cv-qualifier)
// Example:
//      static_assert(is_same<const volatile int, add_cv<int>::type>(), 
//                  "add_cv<int>::type is same as const volatile int");
// Implementation Note:
// Cv-qualified references are ill-formed except when the cv-qualifiers are 
// introduced through the use of a typedef (7.1.3) or of a template type 
// argument (14.3), in which case the cv-qualifiers are ignored.
template <class T>
struct add_const : type_identity<const T> {};

template <class T>
struct add_volatile : type_identity<volatile T> {};

template <class T>
using add_cv = add_const<typename add_volatile<T>::type>;

// If the type T is a reference type, provides the member typedef type which 
// is the type referred to by T. Otherwise type is T.
// Example:
//      static_assert(is_same<int, remove_reference<int&>::type>(), 
//                  "remove_reference<int&>::type is same as int");
//      static_assert(is_same<int, remove_reference<int&&>::type>(), 
//                  "remove_reference<int&&>::type is same as int");
template <class T>
struct remove_reference : type_identity<T> {};

template <class T>
struct remove_reference<T&> : type_identity<T> {};

template <class T>
struct remove_reference<T&&> : type_identity<T> {};

namespace detail {
    template <class T>
    auto try_add_lref(int) -> type_identity<T&>;

    template <class T>
    auto try_add_lref(...) -> type_identity<T>;

    template <class T>
    auto try_add_rref(int) -> type_identity<T&&>;

    template <class T>
    auto try_add_rref(...) -> type_identity<T>;
}

// Creates a lvalue or rvalue reference type of T.
// Example:
//      static_assert(is_same<int&, add_lvalue_reference<int>::type>(), 
//                  "add_lvalue_reference<int>::type is same as int&");
//      static_assert(is_same<int&&, add_rvalue_reference<int>::type>(), 
//                  "add_rvalue_reference<int>::type is same as int&&");
// Implementation Note:
// The major difference to directly using T& is that std::add_lvalue_reference<void>::type
// is void, while void& leads to a compilation error.
template <class T>
struct add_lvalue_reference : public decltype(detail::try_add_lref<T>(0)) {};

template <class T>
struct add_rvalue_reference : public decltype(detail::try_add_rref<T>(0)) {};

// Provides the member typedef type which is the type pointed to by T, 
// or, if T is not a pointer, then type is the same as T.
// Example: 
//      static_assert(is_same<int, remove_pointer<int*>::type>(), 
//                  "remove_pointer<int*>::type is same as int");
// Implementation Note:
// remove_pointer should remove all cv-qualifier for the pointer
template <class T>
struct remove_pointer : type_identity<T> {};

template <class T>
struct remove_pointer<T*> : type_identity<T> {};

template <class T>
struct remove_pointer<T* const> : type_identity<T> {};

template <class T>
struct remove_pointer<T* volatile> : type_identity<T> {};

template <class T>
struct remove_pointer<T* const volatile> : type_identity<T> {};

namespace detail {
    template <class T>
    auto try_add_pointer(int) -> type_identity<typename remove_reference<T>::type*>;
    template <class T>
    auto try_add_pointer(...) -> type_identity<T>;
};

// If T is a reference type, then provides the member typedef type 
// which is a pointer to the referred type.
// Otherwise provide the member typedef type which is the type T*.
// Example: 
//      static_assert(is_same<int*, add_pointer<int>::type>(), 
//                  "add_pointer<int>::type is same as int*");
// Implementation Note:
// if T is a cv- or ref-qualified function type, provides the member 
// typedef type which is the type T.
// They are functions like (all be member functions): 
//      int f() const;
//      int f() &;
//      int f() &&;
template <class T>
struct add_pointer : decltype(detail::try_add_pointer<T>(0)) {};

// If the type T is a reference type, provides the member typedef type
// which is the type referred to by T with its topmost cv-qualifiers removed. 
// Otherwise type is T with its topmost cv-qualifiers removed.
// Example: 
//      static_assert(is_same<int, remove_cvref<const volatile int&>::type>(), 
//                  "remove_cvref<const volatile int&>::type is same as int");
template <class T>
struct remove_cvref : type_identity<typename remove_cv<typename remove_reference<T>::type>::type> {};

NS_META_END

#endif /* type_traits_cv_h */
