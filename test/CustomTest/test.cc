#include "catch.hpp"
#include "testclass.hh"

TEST_CASE("Test with a class", "[TestClass]") {

    TestClass myTest;

    REQUIRE( myTest.getMyInt() == 0 );
    REQUIRE( myTest.getMyDouble() == Approx(0) );
    REQUIRE( myTest.getMyString().empty() );

    SECTION("Setter int") {
        myTest.setMyInt(10);

        REQUIRE( myTest.getMyInt() == 10 );
    }

    SECTION("Setter double") {
        myTest.setMyDouble(5.0);

        REQUIRE( myTest.getMyDouble() == Approx(5.0) );

        myTest.setMyDouble(0.9999);

        REQUIRE( myTest.getMyDouble() == Approx(1).margin(0.0001) );

        REQUIRE_FALSE( myTest.getMyDouble() == Approx(1).margin(0.00000001) );
    }

    SECTION("Setter string")  {
        myTest.setMyString("Bob POWER");

        REQUIRE( myTest.getMyString() == "Bob POWER" );
    }

    SECTION("Exception") {

        REQUIRE_THROWS( myTest.throwException() );
        REQUIRE_THROWS_AS( myTest.throwException() , std::runtime_error);

        REQUIRE_NOTHROW( myTest.noThrowException() );

        REQUIRE_THROWS( myTest.throwCustomException() );
        REQUIRE_THROWS_AS( myTest.throwCustomException() , TestException );
    }


}
