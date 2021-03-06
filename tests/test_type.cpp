#include "catch2/catch.hpp"
#include "type_traits_type.h"

USE_META

template <class T>
struct PM_Traits {};

template <class T, class U>
struct PM_Traits<U T::*> {
	using member_type = U;
};

union TestUnion {};

struct TestClass {
	void mf() {}
	void operator()() {}
	int mo;
	static void sf();
	static int so;
};

enum TestEnum{
};

enum class TestEnumClass {
};

TEST_CASE("traits type", "[trais][type]" ) {
	typedef void (*FuncPointer)();
	typedef void BasicFunc();
	typedef void VariadicFunc(...);
	typedef const int ConstRetFunc();
	typedef volatile int VolatileRetFunc();
	typedef void ConstFunc() const;
	typedef void VolatileFunc() volatile;
	typedef void LRefFunc() &;
	typedef void RRefFunc() &&;

	SECTION("is same") {
		// diffrent type
		REQUIRE(is_same<int, int>());
		REQUIRE_FALSE(is_same<float, int>());

		//cvrp
		REQUIRE(is_same<const int, const int>());
		REQUIRE_FALSE(is_same<int, const int>());

		REQUIRE(is_same<int&, int&>());
		REQUIRE_FALSE(is_same<int, int&>());

		REQUIRE(is_same<volatile int, volatile int>());
		REQUIRE_FALSE(is_same<int, volatile int>());

		REQUIRE(is_same<int*, int*>());
		REQUIRE_FALSE(is_same<int, int*>());
	}

	SECTION("Primary types") {
		// void
		REQUIRE(is_void<void>());
		REQUIRE(is_void<const void>());
		REQUIRE(is_void<volatile void>());
		REQUIRE(is_void<const volatile void>());
		REQUIRE_FALSE(is_void<int>());

		// nullptr
		REQUIRE(is_null_pointer<std::nullptr_t>());
		REQUIRE(is_null_pointer<const std::nullptr_t>());
		REQUIRE(is_null_pointer<volatile std::nullptr_t>());
		REQUIRE(is_null_pointer<const volatile std::nullptr_t>());
		REQUIRE_FALSE(is_null_pointer<void>());

		// integral
		REQUIRE(is_integral<char>());
		REQUIRE(is_integral<signed char>());
		REQUIRE(is_integral<unsigned char>());
		REQUIRE(is_integral<std::uint32_t>());
		REQUIRE_FALSE(is_integral<float>());

		// float
		REQUIRE(is_floating_point<float>());
		REQUIRE(is_floating_point<double>());
		REQUIRE(is_floating_point<long double>());
		REQUIRE_FALSE(is_floating_point<int>());

		// array
		REQUIRE(is_bounded_array<int[6]>());
		REQUIRE_FALSE(is_bounded_array<int[]>());
		REQUIRE_FALSE(is_bounded_array<int>());
		REQUIRE(is_unbounded_array<int[]>());
		REQUIRE_FALSE(is_unbounded_array<int[6]>());
		REQUIRE_FALSE(is_unbounded_array<int>());
		REQUIRE(is_array<int[6]>());
		REQUIRE(is_array<int[]>());
		REQUIRE_FALSE(is_array<int>());
	}
    
    SECTION("class/union") {
        REQUIRE(is_union<TestUnion>());
        REQUIRE_FALSE(is_union<TestClass>());
        REQUIRE_FALSE(is_union<int>());
        REQUIRE(is_class<TestClass>());
        REQUIRE_FALSE(is_class<TestUnion>());
        REQUIRE_FALSE(is_class<int>());
    }

    SECTION("function") {
		// basic
		REQUIRE(is_function<BasicFunc>());
		REQUIRE(is_function<VariadicFunc>());
		REQUIRE(is_function<ConstRetFunc>());
		REQUIRE(is_function<VolatileRetFunc>());
		REQUIRE(is_function<ConstFunc>());
		REQUIRE(is_function<VolatileFunc>());
		REQUIRE(is_function<LRefFunc>());
		REQUIRE(is_function<RRefFunc>());
		REQUIRE(is_function<PM_Traits<decltype(&TestClass::mf)>::member_type>());
		REQUIRE(is_function<decltype(TestClass::sf)>());

		// exception 1, std::function
		REQUIRE_FALSE(is_function<std::function<void()>>());

		// exception 2, lambda
		auto l = [](){};
		REQUIRE_FALSE(is_function<decltype(l)>());

		// exception 3, classes with overloaded operator() 
		REQUIRE_FALSE(is_function<TestClass>());

		// exception 4, pointers to function
		REQUIRE_FALSE(is_function<FuncPointer>());

		// exception 5, member function
		REQUIRE_FALSE(is_function<decltype(&TestClass::mf)>());

		// others
		REQUIRE_FALSE(is_function<void>());
    }
    
    SECTION("pointer") {
    	// basic
    	REQUIRE(is_pointer<int*>());
    	REQUIRE(is_pointer<FuncPointer>());
    	REQUIRE(is_pointer<decltype(&TestClass::sf)>());

    	REQUIRE_FALSE(is_pointer<int>());
    	REQUIRE_FALSE(is_pointer<std::nullptr_t>());

    	// exception 1, pointer to member function/object
    	REQUIRE_FALSE(is_pointer<decltype(&TestClass::mf)>());
    	REQUIRE_FALSE(is_pointer<decltype(&TestClass::mo)>());
    }

    SECTION("reference") {
    	// basic
    	REQUIRE(is_lvalue_reference<int&>());
    	REQUIRE(is_rvalue_reference<int&&>());
    	REQUIRE(is_reference<int&>());
    	REQUIRE(is_reference<int&&>());

    	REQUIRE_FALSE(is_lvalue_reference<int>());
    	REQUIRE_FALSE(is_rvalue_reference<int>());
    	REQUIRE_FALSE(is_reference<int>());

    	REQUIRE_FALSE(is_lvalue_reference<LRefFunc>());
    	REQUIRE_FALSE(is_rvalue_reference<RRefFunc>());
    	REQUIRE_FALSE(is_reference<LRefFunc>());
    	REQUIRE_FALSE(is_reference<RRefFunc>());
    }

    SECTION("class member") {
    	// basic
    	REQUIRE(is_member_function_pointer<decltype(&TestClass::mf)>());
    	REQUIRE(is_member_object_pointer<decltype(&TestClass::mo)>());
    	REQUIRE(is_member_pointer<decltype(&TestClass::mf)>());
    	REQUIRE(is_member_pointer<decltype(&TestClass::mo)>());

    	REQUIRE_FALSE(is_member_object_pointer<decltype(&TestClass::mf)>());
    	REQUIRE_FALSE(is_member_function_pointer<decltype(&TestClass::mo)>());

    	REQUIRE_FALSE(is_member_function_pointer<BasicFunc>());
    	REQUIRE_FALSE(is_member_object_pointer<int*>());
    	REQUIRE_FALSE(is_member_pointer<BasicFunc>());
    	REQUIRE_FALSE(is_member_pointer<int*>());

    	// exception, static member function/object pointer
    	REQUIRE_FALSE(is_member_function_pointer<decltype(&TestClass::sf)>());
    	REQUIRE_FALSE(is_member_object_pointer<decltype(&TestClass::so)>());
    	REQUIRE_FALSE(is_member_pointer<decltype(&TestClass::sf)>());
    	REQUIRE_FALSE(is_member_pointer<decltype(&TestClass::so)>());
    }

    SECTION("enum") {
    	// basic
    	REQUIRE(is_enum<TestEnum>()); // enum
    	REQUIRE(is_enum<TestEnumClass>()); // enum

    	REQUIRE_FALSE(is_enum<int>()); // int 
    	REQUIRE_FALSE(is_enum<float>()); // float
    	REQUIRE_FALSE(is_enum<void>()); // void
    	REQUIRE_FALSE(is_enum<std::nullptr_t>()); // nullptr
    	REQUIRE_FALSE(is_enum<int&>()); // reference
    	REQUIRE_FALSE(is_enum<int*>()); // pointer
    	REQUIRE_FALSE(is_enum<FuncPointer>()); // function pointer
    	REQUIRE_FALSE(is_enum<decltype(&TestClass::mf)>()); // pointer to member function
    	REQUIRE_FALSE(is_enum<decltype(&TestClass::mo)>()); // pointer to member object
    	REQUIRE_FALSE(is_enum<BasicFunc>()); // function
    	REQUIRE_FALSE(is_enum<TestUnion>()); // union
    	REQUIRE_FALSE(is_enum<TestClass>()); // class
    	REQUIRE_FALSE(is_enum<int[]>()); // array
    	REQUIRE_FALSE(is_enum<int[6]>()); // array
    }

    SECTION("arithmetic") {
    	// basic
    	REQUIRE(is_arithmetic<int>()); // int 
    	REQUIRE(is_arithmetic<float>()); // float
    	REQUIRE_FALSE(is_arithmetic<void>()); // void
    	REQUIRE_FALSE(is_arithmetic<std::nullptr_t>()); // nullptr
    	REQUIRE_FALSE(is_arithmetic<int&>()); // reference
    	REQUIRE_FALSE(is_arithmetic<int*>()); // pointer
    	REQUIRE_FALSE(is_arithmetic<FuncPointer>()); // function pointer
    	REQUIRE_FALSE(is_arithmetic<decltype(&TestClass::mf)>()); // pointer to member function
    	REQUIRE_FALSE(is_arithmetic<decltype(&TestClass::mo)>()); // pointer to member object
    	REQUIRE_FALSE(is_arithmetic<BasicFunc>()); // function
    	REQUIRE_FALSE(is_arithmetic<TestEnum>()); // enum
    	REQUIRE_FALSE(is_arithmetic<TestUnion>()); // union
    	REQUIRE_FALSE(is_arithmetic<TestClass>()); // class
    	REQUIRE_FALSE(is_arithmetic<int[]>()); // array
    	REQUIRE_FALSE(is_arithmetic<int[6]>()); // array
    }
   
   	SECTION("fundamental") {
    	// basic
    	REQUIRE(is_fundamental<int>()); // int 
    	REQUIRE(is_fundamental<float>()); // float
    	REQUIRE(is_fundamental<void>()); // void
    	REQUIRE(is_fundamental<std::nullptr_t>()); // nullptr
    	REQUIRE_FALSE(is_fundamental<int&>()); // reference
    	REQUIRE_FALSE(is_fundamental<int*>()); // pointer
    	REQUIRE_FALSE(is_fundamental<FuncPointer>()); // function pointer
    	REQUIRE_FALSE(is_fundamental<decltype(&TestClass::mf)>()); // pointer to member function
    	REQUIRE_FALSE(is_fundamental<decltype(&TestClass::mo)>()); // pointer to member object
    	REQUIRE_FALSE(is_fundamental<BasicFunc>()); // function
    	REQUIRE_FALSE(is_fundamental<TestEnum>()); // enum
    	REQUIRE_FALSE(is_fundamental<TestUnion>()); // union
    	REQUIRE_FALSE(is_fundamental<TestClass>()); // class
    	REQUIRE_FALSE(is_fundamental<int[]>()); // array
    	REQUIRE_FALSE(is_fundamental<int[6]>()); // array
    }

    SECTION("scalar") {
    	// basic
    	REQUIRE(is_scalar<int>()); // int 
    	REQUIRE(is_scalar<float>()); // float
    	REQUIRE_FALSE(is_scalar<void>()); // void
    	REQUIRE(is_scalar<std::nullptr_t>()); // nullptr
    	REQUIRE_FALSE(is_scalar<int&>()); // reference
    	REQUIRE(is_scalar<int*>()); // pointer
    	REQUIRE(is_scalar<FuncPointer>()); // function pointer
    	REQUIRE(is_scalar<decltype(&TestClass::mf)>()); // pointer to member function
    	REQUIRE(is_scalar<decltype(&TestClass::mo)>()); // pointer to member object
    	REQUIRE_FALSE(is_scalar<BasicFunc>()); // function
    	REQUIRE(is_scalar<TestEnum>()); // enum
    	REQUIRE_FALSE(is_scalar<TestUnion>()); // union
    	REQUIRE_FALSE(is_scalar<TestClass>()); // class
    	REQUIRE_FALSE(is_scalar<int[]>()); // array
    	REQUIRE_FALSE(is_scalar<int[6]>()); // array
    }

    SECTION("object") {
    	REQUIRE(is_object<int>()); // int 
    	REQUIRE(is_object<float>()); // float
    	REQUIRE_FALSE(is_object<void>()); // void
    	REQUIRE(is_object<std::nullptr_t>()); // nullptr
    	REQUIRE_FALSE(is_object<int&>()); // reference
    	REQUIRE(is_object<int*>()); // pointer
    	REQUIRE(is_object<FuncPointer>()); // function pointer
    	REQUIRE(is_object<decltype(&TestClass::mf)>()); // pointer to member function
    	REQUIRE(is_object<decltype(&TestClass::mo)>()); // pointer to member object
    	REQUIRE_FALSE(is_object<BasicFunc>()); // function
    	REQUIRE(is_object<TestEnum>()); // enum
    	REQUIRE(is_object<TestUnion>()); // union
    	REQUIRE(is_object<TestClass>()); // class
    	REQUIRE(is_object<int[]>()); // array
    	REQUIRE(is_object<int[6]>()); // array
    }

    SECTION("compound") {
    	REQUIRE_FALSE(is_compound<int>()); // int 
    	REQUIRE_FALSE(is_compound<float>()); // float
    	REQUIRE_FALSE(is_compound<void>()); // void
    	REQUIRE_FALSE(is_compound<std::nullptr_t>()); // nullptr
    	REQUIRE(is_compound<int&>()); // reference
    	REQUIRE(is_compound<int*>()); // pointer
    	REQUIRE(is_compound<FuncPointer>()); // function pointer
    	REQUIRE(is_compound<decltype(&TestClass::mf)>()); // pointer to member function
    	REQUIRE(is_compound<decltype(&TestClass::mo)>()); // pointer to member object
    	REQUIRE(is_compound<BasicFunc>()); // function
    	REQUIRE(is_compound<TestEnum>()); // enum
    	REQUIRE(is_compound<TestUnion>()); // union
    	REQUIRE(is_compound<TestClass>()); // class
    	REQUIRE(is_compound<int[]>()); // array
    	REQUIRE(is_compound<int[6]>()); // array
    }
}
