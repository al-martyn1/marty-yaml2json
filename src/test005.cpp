/*! \file
    \brief Regression tests for marty::yaml2json::detectValueType/makeQuoted
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <random>
#include <cstdlib>
#include <exception>
#include <stdexcept>


#include "test_utils.h"

#include "gtest/gtest.h"

#include "yaml_utils.h"

 
//using namespace std;

/*
  TEST(FactorialTest, Positive) {
  EXPECT_EQ(1, Factorial(1));
  EXPECT_EQ(2, Factorial(2));
  EXPECT_EQ(6, Factorial(3));
  EXPECT_EQ(40320, Factorial(8));

  EXPECT_FALSE(IsPrime(1));
  EXPECT_TRUE(IsPrime(2));

  EXPECT_STREQ(nullptr, s.c_string());

  ASSERT_TRUE(n != nullptr);
}
*/

// Usage TEST(TestSuiteName, TestName)

// enum class DetectedValueType
// {
//     literal   , // false, true, null
//     number    ,
//     string
// };



TEST(YamlUtils, detectValueType)
{
    using namespace marty::yaml2json;

    //EXPECT_EQ( DetectedValueType::string , detectValueType("000"));

    
    EXPECT_TRUE( DetectedValueType::literal == detectValueType("null" ));
    EXPECT_TRUE( DetectedValueType::literal == detectValueType("true" ));
    EXPECT_TRUE( DetectedValueType::literal == detectValueType("false"));
    EXPECT_TRUE( DetectedValueType::number  == detectValueType("0"));
    EXPECT_TRUE( DetectedValueType::number  == detectValueType("1"));
    EXPECT_TRUE( DetectedValueType::number  == detectValueType("6"));
    EXPECT_TRUE( DetectedValueType::string  == detectValueType("+6"));
    EXPECT_TRUE( DetectedValueType::number  == detectValueType("-1"));
    EXPECT_TRUE( DetectedValueType::number  == detectValueType("3.14"));
    EXPECT_TRUE( DetectedValueType::string  == detectValueType("+3.14"));
    EXPECT_TRUE( DetectedValueType::number  == detectValueType("0.0"));
    EXPECT_TRUE( DetectedValueType::number  == detectValueType("-0.1"));
    EXPECT_TRUE( DetectedValueType::string  == detectValueType("00"));
    EXPECT_TRUE( DetectedValueType::string  == detectValueType("000"));
    EXPECT_TRUE( DetectedValueType::string  == detectValueType("0.0.0"));
    EXPECT_TRUE( DetectedValueType::string  == detectValueType("2016-09"));
}


TEST(YamlUtils, makeQuoted)
{
    using namespace marty::yaml2json;

    //EXPECT_EQ( DetectedValueType::string , detectValueType("000"));

    // std::string makeQuoted( const std::string & str, bool forceQuoted = false )
    // std::string makeQuoted( const std::string & str, const std::string &tag )
    
    EXPECT_TRUE( "null"     == makeQuoted("null" )      );  // не принудительно, должен обнаружится литерал - без скобок
    EXPECT_TRUE( "\"null\"" == makeQuoted("null", true ));  // принудительно квотится
    EXPECT_TRUE( "null"     == makeQuoted("null", "!"  ));  // признак строки, но должен найтись литерал - без скобок
    EXPECT_TRUE( "null"     == makeQuoted("null", "?"  ));  // признак числа, всё равно должен найтись литерал

#if 0
    EXPECT_TRUE( "false" == makeQuoted("false" ));
    EXPECT_TRUE( "false" == makeQuoted("false" ));
    EXPECT_TRUE( "false" == makeQuoted("false" ));
    EXPECT_TRUE( "false" == makeQuoted("false" ));

    EXPECT_TRUE( "true" == makeQuoted("true" ));
    EXPECT_TRUE( "true" == makeQuoted("true" ));
    EXPECT_TRUE( "true" == makeQuoted("true" ));
    EXPECT_TRUE( "true" == makeQuoted("true" ));
#endif
}




// int main( int argc, char* argv[] )
// {
//     return 0;
// }

#include "src/gtest_main.cc"


