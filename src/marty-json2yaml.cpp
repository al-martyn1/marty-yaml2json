/*! \file
    \brief marty-json2yaml - конвертер JSON->YAML
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

#include "umba/simple_formatter.h"

#include "yaml-cpp/eventhandler.h"
#include "yaml-cpp/yaml.h"  // IWYU pragma: keep

#include "yaml_utils.h"
#include "json_utils.h"

 
using namespace std;


using umba::lout;
using namespace umba::omanip;
using std::cout; 
using std::cerr; 



inline
int printUsage( const char *msg = 0 )
{
    if (msg)
        lout << msg << endl;
    cerr << "Usage: marty-json2yaml [OPTIONS] [yaml [json]]" << std::endl;
    cerr << "If the optional [yaml] file name not taken, the STDIN is used" << std::endl;
    cerr << "If the optional [json] file name not taken, the STDOUT is used" << std::endl;
    cerr << "OPTIONS can be one of the:" << std::endl;
    cerr << "  -h,--help   print this help and exit" << std::endl;
    return 1;
}




// #define USE_EXACT_TEST
#define USE_FAST_STREAM

int main( int argc, char* argv[] )
{

    std::vector<std::string> args;

    int indent = 2;

    for(int i=1; (i<argc) && argv[i]; ++i)
    {
        std::string arg = argv[i];
        if (arg=="-h" || arg=="--help")
        {
            return printUsage();
        }
        else
        {
            args.push_back(arg);
        }
    }

    #if defined(USE_EXACT_TEST) && defined(_DEBUG)

        args.clear();
        args.push_back("F:\\_github\\marty-yaml2json\\tests\\test001_004.txt");

    #endif



    
    std::string inputName = "STDIN";
    
    std::istream *pIn = &std::cin;

    std::ifstream inFile;

    if (args.size()>0)
    {
        inFile.open(args[0], std::ios_base::in);
        if (!inFile)
        {
            std::cerr << "Failed to open input file: " << args[0] << std::endl;
            return (1);
        }
    
        inputName = args[0];
        pIn = &inFile;
    }
    
    std::istream &in = *pIn;


    std::string inData;
    {
        char buffer[4096];
        while (in.read(buffer, sizeof(buffer)))
            inData.append(buffer, sizeof(buffer));
        inData.append(buffer, in.gcount());
    }


    nlohmann::json jIn;

    try
    {
        jIn = nlohmann::json::parse( inData
                                   , nullptr        // parser_callback_t
                                   , true           // allow_exceptions
                                   , true           // allowComments  // ignore_comments
                                   );
    }
    catch(const std::exception &e)
    {
        std::cerr << "Failed to parse input file: " << inputName << std::endl;
        // if (args.size()>0)
        //     std::cerr << "Failed to parse input file: " << args[0] << std::endl;
        // else
        //     std::cerr << "Failed to parse STDIN input" << std::endl;

        std::cerr << "Error: " << e.what() << std::endl;

        return 1;
    }


    
    std::ostream *pOut = &std::cout;
    
    std::ofstream outFile;
    if (args.size()>1)
    {
        outFile.open(args[1], std::ios_base::out);
        if (!outFile)
        {
            std::cerr << "Failed to open output file: " << args[1] << std::endl;
            return (1);
        }
    
        pOut = &outFile;
    }
    
    std::ostream &out = *pOut;


    #ifndef USE_FAST_STREAM
    
        marty::json_utils::writeYaml( out, jIn );
    
    #else
    
        marty::yaml2json::FastSimpleStringStream fssm;
        marty::json_utils::writeYaml( fssm, jIn );
        out << fssm.str();
    
    #endif
    


    return 0;
}



