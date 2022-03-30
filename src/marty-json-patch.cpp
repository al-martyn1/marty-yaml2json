/*! \file
    \brief marty-json-patch utility
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


#include "umba/simple_formatter.h"
#include "json_utils.h"
#include "test_utils.h"

#include <yaml-cpp/node/emit.h>

using umba::lout;
using namespace umba::omanip;
using std::cout; 
using std::cerr; 



inline
int printUsage( const char *msg = 0 )
{
    if (msg)
        lout << msg << endl;
    cerr << "Usage: marty-json-patch [OPTIONS] original diff [new]" << std::endl;
    cerr << "If the optional [new] file name not taken, the STDOUT is used" << std::endl;
    cerr << "OPTIONS can be one of the:" << std::endl;
    cerr << "  -y          force output in the YAML format" << std::endl;
    cerr << "  -j          force output in the JSON format" << std::endl;
    cerr << "  -d=N        set the output indent to N (-1 for the single line output)";
    cerr << "              ':' also can be used as a separator" << std::endl;
    cerr << "  -h,--help   print this help and exit" << std::endl;
    return 1;
}
 



int main( int argc, char* argv[] )
{
    #if 0
    argc = 4;
    argv[1] = (char*)"-d=4";

    {
      static const char* nameOrg = "F:\\_github\\marty-yaml2json\\tests\\marty-json-diff-01.txt";
      static const char* nameNew = "F:\\_github\\marty-yaml2json\\tests\\marty-json-diff-res.dif";
      argv[2] = (char*)nameOrg;
      argv[3] = (char*)nameNew;
    }
    #endif

    std::vector<std::string> args;

    int indent = 2;


    marty::json_utils::FileFormat ffOutput = marty::json_utils::FileFormat::unknown;

    for(int i=1; (i<argc) && argv[i]; ++i)
    {
        std::string arg = argv[i];
        if (arg=="-y")
        {
            ffOutput = marty::json_utils::FileFormat::yaml;
        }
        else if (arg=="-j")
        {
            ffOutput = marty::json_utils::FileFormat::json;
        }
        else if (arg.compare(0,3,"-d=")==0 || arg.compare(0,3,"-d:")==0)
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
            args.push_back(argv[i]);
        }
    }


    #if defined(_DEBUG)
        if (args.size()==0)
        {
            // static const char* nameOrg = "F:\\_github\\marty-yaml2json\\tests\\marty-json-diff-01.txt";
            // static const char* nameNew = "F:\\_github\\marty-yaml2json\\tests\\marty-json-diff-02.txt";

            // static const char* nameOrg = "F:\\_github\\marty-yaml2json\\tests\\marty-yaml-diff-01.txt";
            // static const char* nameNew = "F:\\_github\\marty-yaml2json\\tests\\marty-yaml-diff-02.txt";
            // args.push_back(nameOrg);
            // args.push_back(nameNew);
        }
    #endif

    // std::cout<<"indent: " << indent << std::endl;


    if (args.size()<1)
        return printUsage("Missing input 'original' file name");

    if (args.size()<2)
        return printUsage("Missing input 'diff' file name");

    std::ifstream orgFile(args[0], std::ios_base::in);
    if (!orgFile)
    {
        std::cerr << "Failed to open input file: " << args[0] << std::endl;
        return (1);
    }

    std::string errMsg;
    std::string tmpJson;

    marty::json_utils::FileFormat ffOrg = marty::json_utils::FileFormat::unknown;
    nlohmann::json jOrg = marty::json_utils::parseJsonOrYaml( orgFile, true // allowComments
                                                            , &errMsg, &tmpJson
                                                            , &ffOrg
                                                            );
    if (ffOrg==marty::json_utils::FileFormat::unknown)
    {
        std::cerr << "Failed to parse input file: " << args[0] << std::endl;
        std::cerr << "Error: " << errMsg << std::endl;
        std::cerr << "JSON :" << std::endl << tmpJson << std::endl;
        return (1);
    }


    std::ifstream diffFile(args[1], std::ios_base::in);
    if (!diffFile)
    {
        std::cerr << "Failed to open input file: " << args[1] << std::endl;
        return (1);
    }

    marty::json_utils::FileFormat ffDiff = marty::json_utils::FileFormat::unknown;
    nlohmann::json jDiff = marty::json_utils::parseJsonOrYaml( diffFile, true // allowComments
                                                            , &errMsg, &tmpJson
                                                            , &ffDiff
                                                            );
    if (ffDiff==marty::json_utils::FileFormat::unknown)
    {
        std::cerr << "Failed to parse input file: " << args[1] << std::endl;
        std::cerr << "Error: " << errMsg << std::endl;
        std::cerr << "JSON :" << std::endl << tmpJson << std::endl;
        return (1);
    }


    //!!!
    auto jNew = jOrg.patch(jDiff);


    std::ostream *pOut = &std::cout;

    std::ofstream outFile;
    if (args.size()>2)
    {
        outFile.open(args[2], std::ios_base::out);
        if (!outFile)
        {
            std::cerr << "Failed to open output file: " << args[2] << std::endl;
            return (1);
        }

        pOut = &outFile;
    }

    std::ostream &out = *pOut;


    std::string serializedJsonNew = jNew.dump(indent);

    // Если выходной формат не задан, и один из входных файлов - в формате YAML, то задаём формат YAML
    if (ffOutput == marty::json_utils::FileFormat::unknown)
    {
        if (ffOrg==marty::json_utils::FileFormat::yaml || ffDiff==marty::json_utils::FileFormat::yaml)
            ffOutput = marty::json_utils::FileFormat::yaml;
        else
            ffOutput = marty::json_utils::FileFormat::json;
    }


    if (ffOutput == marty::json_utils::FileFormat::json)
    {
        out << serializedJsonNew;
        return 0;
    }
    else if (ffOutput == marty::json_utils::FileFormat::yaml)
    {
        try
        {
            YAML::Node newNode = YAML::Load(serializedJsonNew);
            // YAML::Emitter emitter;
            // emitter << "Hello world!";
            // std::ofstream fout("file.yaml");
            // out << diffNode;
            out << YAML::Dump(newNode);

        }
        catch(const std::exception &e)
        {
            std::cerr << "Processing YAML output: failed to parse JSON diff: " << e.what() << endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Unknown output format" << std::endl;
        return (1);
    }

    return 0;
}

