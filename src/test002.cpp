/*! \file
    \brief Тест конвертера YAML->JSON
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

#include "test_utils.h"

#include "yaml-cpp/eventhandler.h"
#include "yaml-cpp/yaml.h"  // IWYU pragma: keep

#include "yaml_utils.h"

 
using namespace std;



//#define USE_EXACT_TEST
#define USE_FAST_STREAM

int main( int argc, char* argv[] )
{
    #ifdef USE_EXACT_TEST

        INIT_TEST_INPUT_FILE_EX("002");

    #else

        INIT_TEST_INPUT_FILE_ARG();

    #endif


    YAML::Node rootNode = YAML::Load(in);

    marty::yaml2json::FastSimpleStringStream fssm;

    #ifndef USE_FAST_STREAM
    
        marty::yaml2json::writeJson(std::cout, rootNode, 2);

    #else

        marty::yaml2json::writeJson(fssm, rootNode, 2);
        std::cout << fssm.str();

    #endif

    return 0;
}



