#include "catch2/catch.hpp"
#include "type_traits_cvrp.h"
#include "type_traits_type.h"
#include <type_traits>

USE_META

TEST_CASE("traits cvrp", "[trais][cvrp]" ) {
	typedef void test_func();
	typedef void test_func_const() const;

	SECTION("add cv") {
		REQUIRE(is_same<const int, add_const<int>::type>());
		REQUIRE(is_same<volatile int, add_volatile<int>::type>());
		REQUIRE(is_same<volatile const int, add_cv<int>::type>());
		REQUIRE(is_same<const volatile int, add_cv<int>::type>());

		REQUIRE(is_same<int* const, add_const<int*>::type>());
		REQUIRE(is_same<int* volatile, add_volatile<int*>::type>());
		REQUIRE(is_same<int* const volatile, add_cv<int*>::type>());

		// cv-qualifier may not be applied to a reference
		REQUIRE(is_same<int&, add_const<int&>::type>());
		REQUIRE(is_same<int&, add_volatile<int&>::type>());
		REQUIRE(is_same<int&, add_cv<int&>::type>());
		REQUIRE(is_same<int&&, add_const<int&&>::type>());
		REQUIRE(is_same<int&&, add_volatile<int&&>::type>());
		REQUIRE(is_same<int&&, add_cv<int&&>::type>());

		// cv-qualifier on function type has no effect
		REQUIRE(is_same<test_func, add_const<test_func>::type>());
		REQUIRE(is_same<test_func, add_volatile<test_func>::type>());
		REQUIRE(is_same<test_func, add_cv<test_func>::type>());
	}

	SECTION("remove cv") {
		REQUIRE(is_same<int, remove_const<int>::type>());
		REQUIRE(is_same<int, remove_const<add_const<int>::type>::type>());
		REQUIRE(is_same<add_volatile<int>::type, remove_const<add_cv<int>::type>::type>());

		REQUIRE(is_same<int, remove_volatile<int>::type>());
		REQUIRE(is_same<int, remove_volatile<add_volatile<int>::type>::type>());
		REQUIRE(is_same<add_const<int>::type, remove_volatile<add_cv<int>::type>::type>());

		REQUIRE(is_same<int, remove_cv<int>::type>());
		REQUIRE(is_same<int, remove_cv<add_const<int>::type>::type>());
		REQUIRE(is_same<int, remove_cv<add_volatile<int>::type>::type>());
		REQUIRE(is_same<int, remove_cv<add_cv<int>::type>::type>());

		// const volatile int*, the pointer is not const/volatile
		// it just point to a const volatile int.
		// But int* const volatile, the pointer is const, volatile
		REQUIRE(is_same<int*, remove_const<int* const>::type>());
		REQUIRE(is_same<const int*, remove_const<const int*>::type>());
		REQUIRE(is_same<int*, remove_volatile<int* volatile>::type>());
		REQUIRE(is_same<volatile int*, remove_volatile<volatile int*>::type>());
		REQUIRE(is_same<int*, remove_cv<int* const volatile>::type>());
		REQUIRE(is_same<const volatile int*, remove_cv<const volatile int*>::type>());

		// const/volatile defined the referenced value, not the reference 
		REQUIRE(is_same<const int&, remove_const<const int&>::type>());
		REQUIRE(is_same<volatile int&, remove_volatile<volatile int&>::type>());
		REQUIRE(is_same<const volatile int&, remove_cv<const volatile int&>::type>());
	}

	SECTION("reference") {
		REQUIRE(is_same<int&, add_lvalue_reference<int>::type>());
		REQUIRE(is_same<int&, add_rvalue_reference<add_lvalue_reference<int>::type>::type>());
		REQUIRE(is_same<int&, add_lvalue_reference<add_rvalue_reference<int>::type>::type>());
		REQUIRE(is_same<int&&, add_rvalue_reference<int>::type>());

		REQUIRE(is_same<const int&, add_lvalue_reference<const int>::type>());
		REQUIRE(is_same<const int&&, add_rvalue_reference<const int>::type>());

		REQUIRE(is_same<int, remove_reference<add_lvalue_reference<int>::type>::type>());
		REQUIRE(is_same<int, remove_reference<add_rvalue_reference<int>::type>::type>());

		REQUIRE(is_same<int, remove_reference<int>::type>());

		// void can't add reference
		REQUIRE(is_same<void, add_lvalue_reference<void>::type>());
		REQUIRE(is_same<void, add_rvalue_reference<void>::type>());

		REQUIRE(is_same<test_func&, add_lvalue_reference<test_func>::type>());
		REQUIRE(is_same<test_func&, add_lvalue_reference<test_func&>::type>());
		REQUIRE(is_same<test_func&, add_lvalue_reference<test_func&&>::type>());
		REQUIRE(is_same<test_func&&, add_rvalue_reference<test_func>::type>());
	}

	SECTION("pointer") {
		REQUIRE(is_same<int, remove_pointer<int*>::type>());
		REQUIRE(is_same<int, remove_pointer<int* const>::type>());
		REQUIRE(is_same<int, remove_pointer<int* volatile>::type>());
		REQUIRE(is_same<int, remove_pointer<int* const volatile>::type>());

		REQUIRE(is_same<int**, add_pointer<int*>::type>());

		REQUIRE(is_same<const int, remove_pointer<const int*>::type>());
		REQUIRE(is_same<volatile int, remove_pointer<volatile int*>::type>());
		REQUIRE(is_same<const volatile int, remove_pointer<const volatile int*>::type>());

		REQUIRE(is_same<const int*, add_pointer<const int>::type>());
		REQUIRE(is_same<volatile int*, add_pointer<volatile int>::type>());
		REQUIRE(is_same<const volatile int*, add_pointer<const volatile int>::type>());

		REQUIRE(is_same<test_func*, add_pointer<test_func>::type>());
		REQUIRE(is_same<int()&, add_pointer<int()&>::type>());
		REQUIRE(is_same<int()&&, add_pointer<int()&&>::type>());
		REQUIRE(is_same<int() const, add_pointer<int() const>::type>());
		REQUIRE(is_same<int() volatile, add_pointer<int() volatile>::type>());
	}

	SECTION("cvref") {
		REQUIRE(is_same<int, remove_cvref<const volatile int&>::type>());
	}
}