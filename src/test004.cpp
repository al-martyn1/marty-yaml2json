/*! \file
    \brief Draft конвертера JSON->YAML
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
    ],
    "parameters":[ {
        "name": "figi",
        "in": "query",
        "required": true,
        "description": "FIGI",
        "schema": {
            "type": "string"
            }
        },
        {
        "name": "depth",
        "in": "query",
        "required": true,
        "description": "Глубина стакана [1..20]",
        "schema": {
            "type": "string",
            "format": "int32"
            }
        }
    ],
    "required": [
        "trackingId",
        "payload",
        "status"
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
enum:
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
parameters:
  - name: figi
    in: query
    required: true
    description: FIGI
    schema:
      type: string
  - name: depth
    in: query
    required: true
    description: Глубина стакана [1..20]
    schema:
      type: integer
      format: int32
required:
  - trackingId
  - payload
  - status


#endif

// Examples: F:\_github\marty-far-lsp\src\test006.cpp

inline
std::string makeIndentStr( int indent )
{
    if (indent>=0)
        return std::string( (std::string::size_type)indent, ' ' );

    //return std::string(" ");
    return std::string();
}

inline
bool isScalar( nlohmann::json &j )
{
    if (j.is_null() || j.is_boolean() || j.is_number() || j.is_string())
        return true;
    return false;
}

template<typename StreamType> inline
bool writeScalar( StreamType &s, nlohmann::json &j )
{
    if (j.is_null())
    {
        s << "null";
    }
    else if (j.is_boolean())
    {
        auto val = j.get<bool>();
        s << (val?"true":"false");
    }
    else if (j.is_number_integer() && j.is_number_unsigned())
    {
        auto val = j.get<std::uint64_t>();
        s << val;
    }
    else if (j.is_number_integer())
    {
        auto val = j.get<std::int64_t>();
        s << val;
    }
    else if (j.is_number_float())
    {
        auto val = j.get<double>();
        s << val;
    }
    else if (j.is_string())
    {
        auto val = j.get<std::string>();
        if (val.empty() || val=="null")
            s << '\'' << val << '\'';
        else
            s << val;
    }
    else // j.is_object() || j.is_array()
    {
        return false;;
    }

    return true;
}


template<typename StreamType>
void writeNodeImpl( StreamType &s, nlohmann::json &j // j - не меняется, просто нет константной версии begin/end
                  , int indent, int indentInc, bool noFirstIndent = false
                  ) 
{
    if (indent<0)
        indent = 0;

    if (indentInc<1)
        indentInc = 1;

    if (writeScalar( s, j ))
    {
        return;
    }
    else if (j.is_object())
    {
        bool bFirst = true;
        for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
        {
            if (!(bFirst && noFirstIndent))
            {
                s << makeIndentStr(indent);
            }

            s << it.key() << ":";
            auto val = it.value();
            if (isScalar(val))
            {
                s << " "; 
                writeScalar(s,val);
                s << "\n";
            }
            else
            {
                s << "\n";
                writeNodeImpl( s, val, indent+indentInc, indentInc );
            }

            bFirst = false;
        }

    }
    else if (j.is_array())
    {
        bool bFirst = true;
        for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
        {
            s << makeIndentStr(indent) << "- ";
            if (isScalar(*it))
            {
                writeScalar(s,*it);
                s << "\n";
            }
            else
            {
                writeNodeImpl( s, *it, indent+indentInc, indentInc, true /* noFirstIndent */  );
            }

            bFirst = false;
        }
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
       

        std::string errMsg;
        marty::json_utils::FileFormat detectedFormat = marty::json_utils::FileFormat::unknown;

        nlohmann::json j = marty::json_utils::parseJsonOrYaml( in
                                                             , true // allowComments
                                                             , &errMsg
                                                             , 0 // pTmpJson
                                                             , &detectedFormat
                                                             );

        if (detectedFormat==marty::json_utils::FileFormat::unknown)
        {
            std::cerr << "Processing JSON failed: " << errMsg << endl;
            return 2;
        }


        // jsonNodeTypeCheckDump( std::cout, j );
        writeNodeImpl( std::cout, j, 0, 2 );

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





