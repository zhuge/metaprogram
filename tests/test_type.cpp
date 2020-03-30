#include "catch2/catch.hpp"
#include "type_traits_type.h"

USE_META

TEST_CASE("traits type", "[trais][type]" ) {
	SECTION("is same") {
		// diffrent type
		REQUIRE(is_same<int,int>());
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
}