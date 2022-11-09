#include "catch.hpp"

#include "cReader/sectionreader.hh"
#include "cReader/configreader.hh"
#include "cReader/sectionreaderfactory.hh"

#include "customsection.hh"
#include "customobject.hh"

TEST_CASE("Section", "[cReader]") {
	conf::SectionReader<> section;
	zz::cfg::CfgParser parser("testcheck.cfg");
	
	double tol = 10E-9;
	
	SECTION("TestCheck") {
		REQUIRE( section.check(parser("Check")["notempty"]) );
		REQUIRE_FALSE( section.check(parser("Check")["empty"]) );
	}
	
	SECTION("LoadThrow") {
		section.setParser(&parser);
		REQUIRE_THROWS_AS( section.load<int>("Check","empty"), std::invalid_argument );
	}
	
	SECTION("LoadInt") {
		int myInt = section.load<int>("Load","int");
		REQUIRE( myInt == 1 );
	}
	
	SECTION("LoadDouble") {
		double myDouble = section.load<double>("Load","double");
		REQUIRE( myDouble == Approx(2.0).margin(tol) );
	}
	
	SECTION("LoadBool") {
		bool myBool = false;
		myBool = section.load<bool>("Load","bool");
		REQUIRE( myBool );
	}
	
	SECTION("LoadVector") {
		std::vector<double> myVector = section.load<std::vector<double> >("Load", "vector");
		REQUIRE( myVector.size() == 2 );
		REQUIRE( myVector[0] == Approx(1.2).margin(tol) );
		REQUIRE( myVector[1] == Approx(2.3).margin(tol) );
	}
	
	SECTION("Inheritance") {
		CustomObject* objToFill = new CustomObject();
		conf::SectionReader<CustomObject>* psection = new conf::CustomSection<CustomObject>();

		psection->setParser(&parser);
		psection->setObjToFill(objToFill);

		psection->fill();
		REQUIRE( objToFill->getMyInt() == 1 );
		REQUIRE( objToFill->getMyDouble() == Approx(2.0).margin(tol) );
		REQUIRE( objToFill->getMyBool() );

		std::vector<double> myVector = objToFill->getMyVector();
		REQUIRE( myVector.size() == 2 );
		REQUIRE( myVector[0] == Approx(1.2).margin(tol) );
		REQUIRE( myVector[1] == Approx(2.3).margin(tol) );
		
		delete psection;
	}
}

TEST_CASE("ConfigReader","[cReader]") {
	
	conf::ConfigReader<CustomObject>* reader = new conf::ConfigReader<CustomObject>;
	double tol = 10E-9;
	
	SECTION("LoadThrow") {
		REQUIRE_THROWS_AS( reader->parse("testcheck.cfg"), std::logic_error );
		delete reader;
	}
	
	SECTION("Load") {
		reader->addSection(new conf::CustomSection<CustomObject>() );
		CustomObject* objToFill = reader->parse("testcheck.cfg");
		
		REQUIRE( objToFill->getMyInt() == 1 );
		REQUIRE( objToFill->getMyDouble() == Approx(2.0).margin(tol) );
		REQUIRE( objToFill->getMyBool() );
		
		std::vector<double> myVector = objToFill->getMyVector();
		REQUIRE( myVector.size() == 2 );
		REQUIRE( myVector[0] == Approx(1.2).margin(tol) );
		REQUIRE( myVector[1] == Approx(2.3).margin(tol) );
		
		delete reader;
		delete objToFill;
	}
	
	SECTION("Final") {
		reader->addSection<conf::CustomSection>();
		CustomObject* objToFill = reader->parse("testcheck.cfg");
		
		REQUIRE( objToFill->getMyInt() == 1 );
		REQUIRE( objToFill->getMyDouble() == Approx(2.0).margin(tol) );
		REQUIRE( objToFill->getMyBool() );
		
		std::vector<double> myVector = objToFill->getMyVector();
		REQUIRE( myVector.size() == 2 );
		REQUIRE( myVector[0] == Approx(1.2).margin(tol) );
		REQUIRE( myVector[1] == Approx(2.3).margin(tol) );
		
		delete reader;
		delete objToFill;
	}
	
}
