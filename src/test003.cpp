/*! \file
    \brief Тест конвертера JSON->YAML
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
#include "json_utils.h"

 
using namespace std;




#if 0

{
    "compiler": {
        "c++": "199711",  // Language version
        "family": "msvc", 
        "version": 1929   /* MSVC version */
    },
    "copyright": "(C) 2013-2022 Niels Lohmann",
    "name": "JSON for Modern C++",
    "platform": "win32",
    "url": "https://github.com/nlohmann/json",
    "version": {
        "major": 3,
        "minor": 10,
        "patch": 5,
        "string": "3.10.5"
    },
    "enum": [
      any,
      object,
      number,
      string,
      15,
      3.14,
      {
        "name": "sandbox",
        "description": "Операция в sandbox"
      }
    ],
    "tags": [ {
        "name": "sandbox",
        "description": "Операция в sandbox"
        },
        {
            "name": "orders",
            "description": "Операции заявок"
        },
        {
            "name": "portfolio",
            "description": "Операции с портфелем пользователя"
        },
        {
            "name": "market",
            "description": "Получении информации по бумагам"
        },
        {
            "name": "operations",
            "description": "Получении информации по операциям"
        },
        {
            "name": "user",
            "description": "Получении информации по брокерским счетам"
        }
    ]
}


compiler:
  c++: 199711
  family: msvc
  version: 1929
copyright: (C) 2013-2022 Niels Lohmann
name: JSON for Modern C++
platform: win32
url: https://github.com/nlohmann/json
version:
  major: 3
  minor: 10
  patch: 5
  string: 3.10.5
enum": [
  - any
  - object
  - number
  - string
  - 15
  - 3.14
  - name: sandbox
    description: Операция в sandbox
tags:
  - name: sandbox
    description: Операция в sandbox
  - name: orders
    description: Операции заявок
  - name: portfolio
    description: Операции с портфелем пользователя
  - name: market
    description: Получении информации по бумагам
  - name: operations
    description: Получении информации по операциям
  - name: user
    description: Получении информации по брокерским счетам


#endif

// Examples: F:\_github\marty-far-lsp\src\test006.cpp

template<typename StreamType>
void jsonNodeTypeCheckDump( StreamType &s, nlohmann::json &j ) // j - не меняется, просто нет константной версии begin/end
{
    if (j.is_object())
    {
        for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
        {
            s << it.key() << " : " << it.value() << "\n\n"; // " (";
            auto key = it.key();
            auto val = it.value();
            // lout << "k: " << it.key() << "; v: " << it.value() << ")\n";
            jsonNodeTypeCheckDump( s, it.value() );
        }

    }
    else if (j.is_array())
    {
        for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
        {
          s << *it << "\n\n";
          jsonNodeTypeCheckDump( s, *it );
        }
    }
    else
    {
        s << "j.is_null()   : " << j.is_null()    << "\n"; // scalar
        s << "j.is_boolean(): " << j.is_boolean() << "\n"; // scalar
        s << "j.is_number() : " << j.is_number()  << "\n"; // scalar
        s << "j.is_object() : " << j.is_object()  << "\n"; // map    !!!
        s << "j.is_array()  : " << j.is_array()   << "\n"; // vector !!!
        s << "j.is_string() : " << j.is_string()  << "\n"; // scalar
    }


}


// 1>F:\_github\marty-yaml2json\_libs\nlohmann\detail\iterators\iter_impl.hpp(140,19): error C2440: 'initializing': 
// cannot convert from 
// 'const nlohmann::basic_json<std::map,std::vector,std::string,bool,int64_t,uint64_t,double,std::allocator,nlohmann::adl_serializer,std::vector<unsigned char,std::allocator<unsigned char>>> *const '
// to 
// 'nlohmann::basic_json<std::map,std::vector,std::string,bool,int64_t,uint64_t,double,std::allocator,nlohmann::adl_serializer,std::vector<unsigned char,std::allocator<unsigned char>>> *'




#define USE_EXACT_TEST
#define USE_FAST_STREAM

int main( int argc, char* argv[] )
{
    try
    {
        #ifdef USE_EXACT_TEST
       
            INIT_TEST_INPUT_FILE_EX("001");
       
        #else
       
            INIT_TEST_INPUT_FILE_ARG();
       
        #endif
       

        nlohmann::json j = marty::json_utils::parseJsonOrYaml( in
                                                             , true // allowComments
                                                             );

        if (j.is_null())
        {
            std::cerr << "Processing JSON failed: " << "unknown error or simple null value found" << endl;
            return 2;
        }


        jsonNodeTypeCheckDump( std::cout, j );

        #if 0
        marty::yaml2json::FastSimpleStringStream fssm;
       
        #ifndef USE_FAST_STREAM
        
            marty::yaml2json::writeJson(std::cout, rootNode, 2);
       
        #else
       
            marty::yaml2json::writeJson(fssm, rootNode, 2);
            std::cout << fssm.str();
       
        #endif
        #endif

    }
    catch (const std::exception& e)
    {
        std::cerr << "Processing JSON failed: " << e.what() << endl;
        return 2;
    }
    catch (...)
    {
        std::cerr << "Processing JSON failed: " << "unknown error" << endl;
        return 2;
    }


    return 0;
}





