/*! \file
    \brief Тестировал/разбирался, как работает либа yaml-cpp
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

#if 0
//----------------------------------------------------------------------------
YAML::Node parse(std::istream& input)
{
    return YAML::Load(input);
    //std::cout << doc << "\n";
  //} catch (const YAML::Exception& e) {
  //  std::cerr << e.what() << "\n";
  //}
}

//------------------------------
YAML::Node parse(const char *inputName)
{
    if (std::strcmp(inputName,"-")==0)
    {
        return parse(std::cin);
    }

    std::ifstream fin;
    fin.open(inputName);
    return parse(fin);
}
#endif

inline
std::string getNodeTypeName( YAML::NodeType::value type )
{
    switch( type )
    {
        case YAML::NodeType::value::Undefined: return "Undefined";
        case YAML::NodeType::value::Null     : return "Null";
        case YAML::NodeType::value::Scalar   : return "Scalar";
        case YAML::NodeType::value::Sequence : return "Sequence";
        case YAML::NodeType::value::Map      : return "Map";
        default: return "<UNKNOWN>";
    }
}

inline
std::string makeIndentStr( int indent )
{
    if (indent>=0)
        return std::string( (std::string::size_type)indent, ' ' );

    return std::string();
}


#define DUMP_SCALAR_INFO

/*
    Tag() can be one of:
    ?
    !

openapi: 3.0.0
float_val: 4.0
like_float: '5.0'
int_val: -6
like_int: '-7'
info:
  version: 1.0.0
  termsOfService: "#"


Found tags:
!
    #
    -7     // like_int
    200
    5.0    // like_float
    500
?
    -6     // int_val
    1.0.0
    3.0.0
    4.0    // float_val


! - строки
? - вероятно, числа, но не факт

*/


static std::map<std::string, std::set<std::string> > foundTags;

inline
bool isAnyTextChar( char ch )
{
    if (ch>=' ' && ch<='\"')
        return true;
    if (ch>='$' && ch<=',')
        return true;
    if (ch>=':' && ch<='?')
        return true;
    if (ch>='[' && ch<='`')
        return true;
    if (ch>='{' && ch<='~')
        return true;
    if (ch>='a' && ch<='z')
        return true;
    if (ch>='A' && ch<='Z')
        return true;
    if (ch>126)
        return true;
    if (ch<' ')
        return true;

    return false;
}

inline
bool isTextChar( char ch )
{
    if (ch>='a' && ch<='z')
        return true;
    if (ch>='A' && ch<='Z')
        return true;
    if (ch>126)
        return true;
    if (ch<' ')
        return true;

    return false;
}

inline
bool isAnyTextStr(const std::string &s)
{
    for( auto ch : s )
    {
        if (isAnyTextChar(ch))
            return true;
    }

    return false;
}

inline
bool isTextStr(const std::string &s)
{
    for( auto ch : s )
    {
        if (isTextChar(ch))
            return true;
    }

    return false;
}


inline
void dumpNode( const YAML::Node &node, int indent = 0 )
{
    auto nodeType = node.Type();

    //cout << makeIndentStr(indent) << "Node type: " << getNodeTypeName(nodeType) << endl;


    switch( nodeType )
    {
        case YAML::NodeType::value::Undefined:
             {
                 //cout << makeIndentStr(indent) << "Undefined node found, do nothing" << endl;
             }
             return;

        case YAML::NodeType::value::Null     :
             {
                 //cout << makeIndentStr(indent) << "Null node found, do nothing" << endl;
             }
             return;

        case YAML::NodeType::value::Scalar   :
             {
                 //cout << makeIndentStr(indent) << "Scalar node found" << endl;
                 std::string value  = node.as<std::string>();
                 cout << makeIndentStr(indent) << value << endl;
                 #ifdef DUMP_SCALAR_INFO
                 cout << makeIndentStr(indent) << "// Node type: " << getNodeTypeName(nodeType) << endl;
                 cout << makeIndentStr(indent) << "// Node tag : " << node.Tag() << endl;
                 #endif
                 foundTags[node.Tag()].insert(value);
             }
             return;

        case YAML::NodeType::value::Sequence :
             {
                 //cout << makeIndentStr(indent) << "Sequence node found" << endl;

                 bool isScalarSequence = marty::yaml2json::isNodeScalarSequence(node);
                 if (isScalarSequence)
                 {
                     cout << "";
                 }

                 YAML::Node::const_iterator it = node.begin();
                 for (; it!=node.end(); ++it)
                 {
                     bool isFirst = it==node.begin();
                     auto nextIt  = it; ++nextIt;
                     bool isLast  = nextIt==node.end();

                     // const auto & self   = *it       ;
                     // const auto & first  = it->first ;
                     // const auto & second = it->second;
                     // cout << makeIndentStr(indent) << "Seq item" <<  /* ": " << value << */ endl;

                     switch( it->Type() )
                     {
                         case YAML::NodeType::value::Scalar   :
                              {
                                  //cout << makeIndentStr(indent) << "Scalar in Sequence node found" << endl;
                                  std::string value  = it->as<std::string>();
                                  cout << makeIndentStr(indent) << value;
                                  #ifdef DUMP_SCALAR_INFO
                                  cout << endl << makeIndentStr(indent) << "// Node type: " << getNodeTypeName(it->Type());
                                  cout << endl << makeIndentStr(indent) << "// Node tag : " << it->Tag();
                                  #endif
                                  foundTags[it->Tag()].insert(value);
                                  if (!isLast)
                                      cout << ",";
                                  cout << endl;

                              }
                              break;

                         case YAML::NodeType::value::Sequence :
                              {
                                  //cout << makeIndentStr(indent) << "Sequence in Sequence node found" << endl;
                                  //cout << makeIndentStr(indent) << name << ": " << endl;
                                  cout << makeIndentStr(indent) << "[" << endl;
                                  dumpNode( *it, indent +4 );
                                  cout << makeIndentStr(indent) << "]"; // << endl;
                                  if (!isLast)
                                      cout << ",";
                                  cout << endl;
                              }
                              break;

                         case YAML::NodeType::value::Map      :
                              {
                                  //cout << makeIndentStr(indent) << "Map in Sequence node found" << endl;
                                  //cout << makeIndentStr(indent) << name << ": " << endl;
                                  cout << makeIndentStr(indent) << "{" << endl;
                                  dumpNode( *it, indent +4 );
                                  cout << makeIndentStr(indent) << "}"; // << endl;
                                  if (!isLast)
                                      cout << ",";
                                  cout << endl;

                              }
                              break;
                     }
                 }

             }
             return;

        case YAML::NodeType::value::Map      :
             {
                 //cout << makeIndentStr(indent) << "Map node found" << endl;
                 YAML::Node::const_iterator it = node.begin();
                 for (; it!=node.end(); ++it)
                 {
                     bool isFirst = it==node.begin();
                     auto nextIt  = it; ++nextIt;
                     bool isLast  = nextIt==node.end();

                     std::string name  = it->first.as<std::string>();
                     //cout << makeIndentStr(indent) << name <<  /* ": " << value << */  endl;
                     switch( it->second.Type() )
                     {
                         case YAML::NodeType::value::Scalar   :
                              {
                                  std::string value  = it->second.as<std::string>();
                                  cout << makeIndentStr(indent) << name << ": " << value;
                                  #ifdef DUMP_SCALAR_INFO
                                  cout << endl << makeIndentStr(indent) << "// Name Node type: " << getNodeTypeName(it->first.Type());
                                  cout << endl << makeIndentStr(indent) << "// Name Node tag : " << it->first.Tag();
                                  cout << endl << makeIndentStr(indent) << "// Val Node type: " << getNodeTypeName(it->second.Type());
                                  cout << endl << makeIndentStr(indent) << "// Val Node tag : " << it->second.Tag();
                                  #endif
                                  foundTags[it->first.Tag() ].insert(name);
                                  foundTags[it->second.Tag()].insert(value);
                                  if (!isLast)
                                      cout << ",";
                                  cout << endl;
                              }
                              break;

                         case YAML::NodeType::value::Sequence :
                              {
                                  cout << makeIndentStr(indent) << name << ": ";
                                  #ifdef DUMP_SCALAR_INFO
                                  cout << endl << makeIndentStr(indent) << "// Name Node type: " << getNodeTypeName(it->first.Type());
                                  cout << endl << makeIndentStr(indent) << "// Name Node tag : " << it->first.Tag();
                                  #endif
                                  foundTags[it->first.Tag()].insert(name);
                                  cout << endl;
                                  cout << makeIndentStr(indent) << "[" << endl;
                                  dumpNode( it->second, indent +4 );
                                  cout << makeIndentStr(indent) << "]"; // << endl;
                                  if (!isLast)
                                      cout << ",";
                                  cout << endl;
                              }
                              break;

                         case YAML::NodeType::value::Map      :
                              {
                                  cout << makeIndentStr(indent) << name << ": ";
                                  #ifdef DUMP_SCALAR_INFO
                                  cout << endl << makeIndentStr(indent) << "// Name Node type: " << getNodeTypeName(it->first.Type());
                                  cout << endl << makeIndentStr(indent) << "// Name Node tag : " << it->first.Tag();
                                  #endif
                                  foundTags[it->first.Tag()].insert(name);
                                  cout << endl;
                                  cout << makeIndentStr(indent) << "{" << endl;
                                  dumpNode( it->second, indent +4 );
                                  cout << makeIndentStr(indent) << "}"; // << endl;
                                  if (!isLast)
                                      cout << ",";
                                  cout << endl;
                              }
                              break;
                     }
                 }
             }
             return;

        default:
             {
             }
             return;

    }

#if 0

    if (parentType==YAML::NodeType::value::Undefined)
        return;

    if (parentType==YAML::NodeType::value::Null)
        return;

    YAML::Node::const_iterator it = node.begin();
    for (; it!=node.end(); ++it)
    {
        std::string name  = it->first .as<std::string>();
        std::string value = it->second.as<std::string>();

        cout << name << ": " << value << endl;

        cout << endl;

        //const auto &child = *it;

        //auto type = it->Type();

        //cout << "Node type: " << getNodeTypeName(type) << endl;

        // YAML::const_iterator

        // struct NodeType {
        //   enum value { Undefined, Null, Scalar, Sequence, Map };
        // };

        // class Node
        // NodeType::value Type()
        // bool IsDefined() const;
        // bool IsNull() const { return Type() == NodeType::Null; }
        // bool IsScalar() const { return Type() == NodeType::Scalar; }
        // bool IsSequence() const { return Type() == NodeType::Sequence; }
        // bool IsMap() const { return Type() == NodeType::Map; }

        // explicit operator bool() const { return IsDefined(); }
        // bool operator!() const { return !IsDefined(); }
        // const std::string& Scalar() const;
        // const std::string& Tag() const;
        //  
        // std::size_t size() const;
        //  
        // const_iterator begin() const;
        // iterator begin();
        //  
        // const_iterator end() const;
        // iterator end();
        //  
        // // sequence
        // template <typename T>
        // void push_back(const T& rhs);
        // void push_back(const Node& rhs);

    }
#endif

}




// #define USE_EXACT_TEST


int main( int argc, char* argv[] )
{

    #ifdef USE_EXACT_TEST
    
        INIT_TEST_INPUT_FILE_EX("004");
    
    #else
    
        INIT_TEST_INPUT_FILE_ARG();
    
    #endif


    try
    {
        cout << "Reading '" << testInputFileName << endl;
       
        YAML::Node rootNode = YAML::Load(in); // parse(testInputFileName);
       
        dumpNode(rootNode);

        cout << "\n\n---------------\nFound tags:\n";
        for(auto [tag,tagedVals] : foundTags)
        {
            cout << tag <<"\n";
            for(auto val : tagedVals)
            {
                if (isTextStr(val))
                    continue;
                cout << "    " << val << "\n";
            }
        }

    }
    catch (const YAML::Exception& e)
    {
        std::cerr << "Failed to parse input file: " << testInputFileName << std::endl;
        std::cerr << "Processing YAML failed: " << e.what() << endl;
        return 2;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to parse input file: " << testInputFileName << std::endl;
        std::cerr << "Processing YAML failed: " << e.what() << endl;
        return 2;
    }
    catch (...)
    {
        std::cerr << "Failed to parse input file: " << testInputFileName << std::endl;
        std::cerr << "Processing YAML failed: " << "unknown error" << endl;
        return 2;
    }
    


    return 0;
}

// #include "src/gtest_main.cc"


