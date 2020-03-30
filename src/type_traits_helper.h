//
//  type_traits_helper.h
//  metaprogram
//
//  Created by Gong Wenzhu on 2020/03/27.
//  Copyright © 2020 Gong Wenzhu. All rights reserved.
//

#ifndef type_traits_helper_h
#define type_traits_helper_h

#include "config.h"

NS_META_BEG

// Provides the member typedef type that names T 
// Example:
// 		typename type_identity<int>::type value;
template <class T>
struct type_identity {
    using type = T;
};

// integral_constant wraps a static constant of specified type. 
// It is the base class for the C++ type traits.
// The behavior of a program that adds specializations for integral_constant is undefined.
// Example:
// 		typedef std::integral_constant<int, 2> two_t;
//    	typedef std::integral_constant<int, 4> four_t;
//		
//		static_assert(two_t::value*2 == four_t::value, "2*2 != 4");
template <class T, T t>
struct integral_constant {
    static constexpr T value = t;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

// bool constant
template <bool B>
using bool_constant = integral_constant<bool,B>;

// true/false
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

NS_META_END 

#endif /* type_traits_helper_h */
