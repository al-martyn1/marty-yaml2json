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
#include <exception>
#include <stdexcept>

#include "test_utils.h"

#include "yaml-cpp/eventhandler.h"
#include "yaml-cpp/yaml.h"  // IWYU pragma: keep

#include "yaml_utils.h"

 
using namespace std;



//#define USE_EXACT_TEST
#define USE_FAST_STREAM

int main( int argc, char* argv[] )
{
    if (argc<2)
    {
        std::cerr << "No input file taken" << endl;
        std::cerr << "Usage: marty-yaml2json file [indent]" << endl;
        std::cerr << "  Optional indent can be <0 to generate resulting JSON into single line." << endl;
        return 1;
    }


    try
    {
        int indent = 2;

        if (argc>1)
            indent = std::stoi(argv[2]);

        std::ifstream in;
        in.open(argv[1], std::ios_base::in);
        if (!in)
        {
            std::cerr << "Failed to open input file: " << argv[1] << std::endl;
            return 1;
        }
       
       
        YAML::Node rootNode = YAML::Load(in);
       
        marty::yaml2json::FastSimpleStringStream fssm;
       
        #ifndef USE_FAST_STREAM
        
            marty::yaml2json::writeJson(std::cout, rootNode, indent);
       
        #else
       
            marty::yaml2json::writeJson(fssm, rootNode, indent);
            std::cout << fssm.str();
       
        #endif
    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Processing YAML failed: " << e.what() << endl;
        return 2;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Processing YAML failed: " << e.what() << endl;
        return 2;
    }
    catch (...)
    {
        std::cerr << "Processing YAML failed: " << "unknown error" << endl;
        return 2;
    }


    return 0;
}



