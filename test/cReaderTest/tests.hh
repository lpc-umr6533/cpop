#ifndef TESTS_HPP
#define TESTS_HPP

#include "cReader/sectionreader.hh"
#include "cReader/configreader.hh"
#include "cReader/sectionreaderfactory.hh"

#include "customsection.hh"
#include "customobject.hh"

TEST(SectionReader, DefaultConstructor) {

    conf::SectionReader<> section;
}

TEST(SectionReader, TestCheck) {
    conf::SectionReader<> section;
    zz::cfg::CfgParser parser("../data/testcheck.cfg");

    ASSERT_TRUE( section.check(parser("Check")["notempty"]) );
    ASSERT_FALSE( section.check(parser("Check")["empty"]) );
}

TEST(SectionReader, TestLoad1Throw) {
    conf::SectionReader<> section;
    zz::cfg::CfgParser parser("../data/testcheck.cfg");

    section.setParser(&parser);

    EXPECT_THROW(section.load<int>("Check","empty"), std::invalid_argument);

}

TEST(SectionReader, TestLoadInt) {

    conf::SectionReader<> section;
    zz::cfg::CfgParser parser("../data/testcheck.cfg");

    section.setParser(&parser);

    int myInt = section.load<int>("Load","int");
    EXPECT_EQ(myInt, 1);

}

TEST(SectionReader, TestLoadDouble) {

    conf::SectionReader<> section;
    zz::cfg::CfgParser parser("../data/testcheck.cfg");

    section.setParser(&parser);

    double myDouble = section.load<double>("Load","double");
    EXPECT_EQ(myDouble, 2.0);

}

TEST(SectionReader, TestLoadBool) {

    conf::SectionReader<> section;
    zz::cfg::CfgParser parser("../data/testcheck.cfg");

    section.setParser(&parser);

    bool myBool = false;
    myBool = section.load<bool>("Load","bool");
    EXPECT_TRUE(myBool);

}

TEST(SectionReader, TestLoadVector) {

    conf::SectionReader<> section;
    zz::cfg::CfgParser parser("../data/testcheck.cfg");

    section.setParser(&parser);

    std::vector<double> myVector = section.load<std::vector<double> >("Load", "vector");
    EXPECT_EQ(myVector.size(), 2);
    EXPECT_EQ(myVector[0], 1.2);
    EXPECT_EQ(myVector[1], 2.3);

}

TEST(SectionReader, Inheritance){
    CustomObject* objToFill = new CustomObject();
    conf::SectionReader<CustomObject>* section = new conf::CustomSection<CustomObject>();
    zz::cfg::CfgParser parser("../data/testcheck.cfg");

    section->setParser(&parser);
    section->setObjToFill(objToFill);

    section->fill();
    ASSERT_EQ(objToFill->getMyInt(), 1);
    ASSERT_EQ(objToFill->getMyDouble(), 2.0);
    ASSERT_TRUE(objToFill->getMyBool());

    std::vector<double> myVector = objToFill->getMyVector();
    EXPECT_EQ(myVector.size(), 2);
    EXPECT_EQ(myVector[0], 1.2);
    EXPECT_EQ(myVector[1], 2.3);


    delete objToFill;
    delete section;

}

TEST(ConfigReader, DefaultConstructor) {
    conf::ConfigReader<CustomObject>* reader = new conf::ConfigReader<CustomObject>;

    delete reader;
}

TEST(ConfigReader, TestLoadThrow) {

    conf::ConfigReader<CustomObject>* reader = new conf::ConfigReader<CustomObject>;

    EXPECT_THROW( reader->parse("../data/testcheck.cfg"), std::logic_error);

    delete reader;
}

TEST(ConfigReader, TestLoad) {

    conf::ConfigReader<CustomObject>* reader = new conf::ConfigReader<CustomObject>();
    reader->addSection(new conf::CustomSection<CustomObject>() );


    CustomObject* objToFill = reader->parse("../data/testcheck.cfg");


    ASSERT_TRUE(objToFill != nullptr);
    EXPECT_EQ(objToFill->getMyInt(), 1);
    EXPECT_EQ(objToFill->getMyDouble(), 2.0);
    EXPECT_TRUE(objToFill->getMyBool());

    std::vector<double> myVector = objToFill->getMyVector();
    EXPECT_EQ(myVector.size(), 2);
    EXPECT_EQ(myVector[0], 1.2);
    EXPECT_EQ(myVector[1], 2.3);


    delete objToFill;
    delete reader;

}

TEST(ConfigReader, TestLoadFinal) {

    conf::ConfigReader<CustomObject>* reader = new conf::ConfigReader<CustomObject>();
    reader->addSection<conf::CustomSection>();


    CustomObject* objToFill = reader->parse("../data/testcheck.cfg");


    ASSERT_TRUE(objToFill != nullptr);
    EXPECT_EQ(objToFill->getMyInt(), 1);
    EXPECT_EQ(objToFill->getMyDouble(), 2.0);
    EXPECT_TRUE(objToFill->getMyBool());

    std::vector<double> myVector = objToFill->getMyVector();
    EXPECT_EQ(myVector.size(), 2);
    EXPECT_EQ(myVector[0], 1.2);
    EXPECT_EQ(myVector[1], 2.3);


    delete objToFill;
    delete reader;

}


#endif
