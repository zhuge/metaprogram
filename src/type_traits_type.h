//
//  type_traits_type.h
//  metaprogram
//
//  Created by Gong Wenzhu on 2020/03/27.
//  Copyright © 2020 Gong Wenzhu. All rights reserved.
//

#ifndef type_traits_type_h
#define type_traits_type_h

#include "config.h"
#include "type_traits_helper.h"

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

NS_META_END

#endif // type_traits_type_h