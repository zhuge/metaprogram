#include "catch2/catch.hpp"
#include "type_traits_cvrp.h"
#include "type_traits_type.h"

USE_META

TEST_CASE("traits cvrp", "[trais][cvrp]" ) {
	typedef void BasicFunc();
	typedef const int ConstRetFunc();
	typedef volatile int VolatileRetFunc();
	typedef void ConstFunc() const;
	typedef void VolatileFunc() volatile;
	typedef void LRefFunc() &;
	typedef void RRefFunc() &&;

	SECTION("add cv") {
		// basic type
		REQUIRE(is_same<const int, add_const<int>::type>());
		REQUIRE(is_same<volatile int, add_volatile<int>::type>());
		REQUIRE(is_same<volatile const int, add_cv<int>::type>());
		REQUIRE(is_same<const volatile int, add_cv<int>::type>());

		// pointer
		REQUIRE(is_same<int* const, add_const<int*>::type>());
		REQUIRE(is_same<int* volatile, add_volatile<int*>::type>());
		REQUIRE(is_same<int* const volatile, add_cv<int*>::type>());

		// exception 1, already has cv-qualifier
		REQUIRE(is_same<const int, add_const<const int>::type>());
		REQUIRE(is_same<volatile int, add_volatile<volatile int>::type>());
		REQUIRE(is_same<const volatile int, add_cv<const volatile int>::type>());

		// exception 2, function
		REQUIRE(is_same<BasicFunc, add_cv<BasicFunc>::type>());
		REQUIRE(is_same<ConstRetFunc, add_cv<ConstRetFunc>::type>());
		REQUIRE(is_same<VolatileRetFunc, add_cv<VolatileRetFunc>::type>());
		REQUIRE(is_same<VolatileFunc, add_cv<VolatileFunc>::type>());
		REQUIRE(is_same<LRefFunc, add_cv<LRefFunc>::type>());
		REQUIRE(is_same<RRefFunc, add_cv<RRefFunc>::type>());

		// exception 3, reference
		REQUIRE(is_same<int&, add_const<int&>::type>());
		REQUIRE(is_same<int&, add_volatile<int&>::type>());
		REQUIRE(is_same<int&, add_cv<int&>::type>());
		REQUIRE(is_same<int&&, add_const<int&&>::type>());
		REQUIRE(is_same<int&&, add_volatile<int&&>::type>());
		REQUIRE(is_same<int&&, add_cv<int&&>::type>());
	}

	SECTION("remove cv") {
		// basic
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

		// const/volatile function, which meanings it call object is const/volatile not the function
		// const/volatile return type function, which meaning its return value is const/volatile not the function 
		REQUIRE(is_same<BasicFunc, remove_cv<BasicFunc>::type>());
		REQUIRE(is_same<ConstRetFunc, remove_const<ConstRetFunc>::type>());
		REQUIRE(is_same<VolatileRetFunc, remove_volatile<VolatileRetFunc>::type>());
		REQUIRE(is_same<ConstFunc, remove_const<ConstFunc>::type>());
		REQUIRE(is_same<VolatileFunc, remove_volatile<VolatileFunc>::type>());
	}

	SECTION("add reference") {
		// basic
		REQUIRE(is_same<int&, add_lvalue_reference<int>::type>());
		REQUIRE(is_same<int&, add_rvalue_reference<add_lvalue_reference<int>::type>::type>());
		REQUIRE(is_same<int&, add_lvalue_reference<add_rvalue_reference<int>::type>::type>());
		REQUIRE(is_same<int&&, add_rvalue_reference<int>::type>());

		REQUIRE(is_same<const int&, add_lvalue_reference<const int>::type>());
		REQUIRE(is_same<const int&&, add_rvalue_reference<const int>::type>());

		REQUIRE(is_same<volatile int&, add_lvalue_reference<volatile int>::type>());
		REQUIRE(is_same<volatile int&&, add_rvalue_reference<volatile int>::type>());

		REQUIRE(is_same<BasicFunc&, add_lvalue_reference<BasicFunc>::type>());
		REQUIRE(is_same<BasicFunc&&, add_rvalue_reference<BasicFunc>::type>());
		REQUIRE(is_same<ConstRetFunc&, add_lvalue_reference<ConstRetFunc>::type>());
		REQUIRE(is_same<ConstRetFunc&&, add_rvalue_reference<ConstRetFunc>::type>());
		REQUIRE(is_same<VolatileRetFunc&, add_lvalue_reference<VolatileRetFunc>::type>());
		REQUIRE(is_same<VolatileRetFunc&&, add_rvalue_reference<VolatileRetFunc>::type>());
		
		// exception 1, function type that has  cv- or ref- qualifier
		REQUIRE(is_same<ConstFunc, add_lvalue_reference<ConstFunc>::type>());
		REQUIRE(is_same<ConstFunc, add_rvalue_reference<ConstFunc>::type>());
		REQUIRE(is_same<VolatileFunc, add_lvalue_reference<VolatileFunc>::type>());
		REQUIRE(is_same<VolatileFunc, add_rvalue_reference<VolatileFunc>::type>());
		REQUIRE(is_same<LRefFunc, add_lvalue_reference<LRefFunc>::type>());
		REQUIRE(is_same<LRefFunc, add_rvalue_reference<LRefFunc>::type>());
		REQUIRE(is_same<RRefFunc, add_lvalue_reference<RRefFunc>::type>());
		REQUIRE(is_same<RRefFunc, add_rvalue_reference<RRefFunc>::type>());

		// exception 2, void
		REQUIRE(is_same<void, add_lvalue_reference<void>::type>());
		REQUIRE(is_same<void, add_rvalue_reference<void>::type>());
	}

	SECTION("remove reference") {
		// basic
		REQUIRE(is_same<int, remove_reference<int>::type>());

		REQUIRE(is_same<int, remove_reference<add_lvalue_reference<int>::type>::type>());
		REQUIRE(is_same<int, remove_reference<add_rvalue_reference<int>::type>::type>());

		REQUIRE(is_same<const int, remove_reference<add_lvalue_reference<const int>::type>::type>());
		REQUIRE(is_same<volatile int, remove_reference<add_rvalue_reference<volatile int>::type>::type>());
	
		// reference function, the reference is for the call object not the function
		REQUIRE(is_same<LRefFunc, remove_reference<LRefFunc>::type>());
		REQUIRE(is_same<RRefFunc, remove_reference<RRefFunc>::type>());
	}

	SECTION("add pointer") {
		// basic
		REQUIRE(is_same<int*, add_pointer<int>::type>());
		REQUIRE(is_same<const int*, add_pointer<const int>::type>());
		REQUIRE(is_same<volatile int*, add_pointer<volatile int>::type>());
		REQUIRE(is_same<const volatile int*, add_pointer<const volatile int>::type>());

		REQUIRE(is_same<BasicFunc*, add_pointer<BasicFunc>::type>());
		REQUIRE(is_same<ConstRetFunc*, add_pointer<ConstRetFunc>::type>());
		REQUIRE(is_same<VolatileRetFunc*, add_pointer<VolatileRetFunc>::type>());

		// exception 1, reference
		REQUIRE(is_same<int*, add_pointer<int&>::type>());
		REQUIRE(is_same<const int*, add_pointer<const int&>::type>());
		REQUIRE(is_same<volatile int*, add_pointer<volatile int&>::type>());
		REQUIRE(is_same<const volatile int*, add_pointer<const volatile int&>::type>());

		// exception 2, cv- or ref-qualified function type
		REQUIRE(is_same<ConstFunc, add_pointer<ConstFunc>::type>());
		REQUIRE(is_same<VolatileFunc, add_pointer<VolatileFunc>::type>());
		REQUIRE(is_same<LRefFunc, add_pointer<LRefFunc>::type>());
		REQUIRE(is_same<RRefFunc, add_pointer<RRefFunc>::type>());
	}

	SECTION("remove pointer") {
		// basic
		REQUIRE(is_same<int, remove_pointer<int*>::type>());
		REQUIRE(is_same<int, remove_pointer<int* const>::type>());
		REQUIRE(is_same<int, remove_pointer<int* volatile>::type>());
		REQUIRE(is_same<int, remove_pointer<int* const volatile>::type>());

		REQUIRE(is_same<const int, remove_pointer<const int*>::type>());
		REQUIRE(is_same<volatile int, remove_pointer<volatile int*>::type>());
		REQUIRE(is_same<const volatile int, remove_pointer<const volatile int*>::type>());

		REQUIRE(is_same<BasicFunc, remove_pointer<BasicFunc*>::type>());
		REQUIRE(is_same<ConstRetFunc, remove_pointer<ConstRetFunc*>::type>());
		REQUIRE(is_same<VolatileRetFunc, remove_pointer<VolatileRetFunc*>::type>());

		REQUIRE(is_same<int**, add_pointer<int*>::type>());
	}

	SECTION("cvref") {
		REQUIRE(is_same<int, remove_cvref<const volatile int&>::type>());
	}
}