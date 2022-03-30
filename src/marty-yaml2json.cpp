/*! \file
    \brief marty-yaml2json - конвертер YAML->JSON
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
    cerr << "Usage: marty-yaml2json [OPTIONS] yaml [json]" << std::endl;
    cerr << "If the optional [json] file name not taken, the STDOUT is used" << std::endl;
    cerr << "OPTIONS can be one of the:" << std::endl;
    cerr << "  -d=N        set the output indent to N (-1 for the single line output)";
    cerr << "              ':' also can be used as a separator" << std::endl;
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
        if (arg.compare(0,3,"-d=")==0 || arg.compare(0,3,"-d:")==0)
        {
            try
            {
                indent = std::stoi(arg.substr(3));
            }
            catch (const std::exception& e)
            {
                std::cerr << "Parsing indent value failed: " << e.what() << endl;
                return 2;
            }
            catch (...)
            {
                std::cerr << "Parsing indent value failed: " << "unknown error" << endl;
                return 2;
            }
        }
        else if (arg=="-h" || arg=="--help")
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
        args.push_back("F:\\_github\\marty-yaml2json\\tests\\test001_001.txt");

    #endif



    if (args.size()<1)
    {
        return printUsage("No input file taken");
    }


    try
    {
        std::ifstream in;
        in.open(args[0], std::ios_base::in);
        if (!in)
        {
            std::cerr << "Failed to open input file: " << args[0] << std::endl;
            return 1;
        }
       
       
        YAML::Node rootNode = YAML::Load(in);
       
        marty::yaml2json::FastSimpleStringStream fssm;


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
        
            marty::yaml2json::writeJson(out, rootNode, indent);
       
        #else
       
            marty::yaml2json::writeJson(fssm, rootNode, indent);
            out << fssm.str();
       
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



