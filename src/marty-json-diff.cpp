/*! \file
    \brief marty-json-diff utility
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
    cerr << "Usage: marty-json-diff [OPTIONS] original new [diff]" << std::endl;
    cerr << "If the optional [diff] file name not taken, the STDOUT is used" << std::endl;
    cerr << "OPTIONS can be one of the:" << std::endl;
    cerr << "  -y          force output in the YAML format" << std::endl;
    cerr << "  -j          force output in the JSON format" << std::endl;
    cerr << "  -h,--help   print this help and exit" << std::endl;
    return 1;
}
 



int main( int argc, char* argv[] )
{
    std::vector<std::string> args;


    marty::json_utils::FileFormat ffOutput = marty::json_utils::FileFormat::unknown;

    for(int i=1; (i<argc) && argv[i]; ++i)
    {
        std::string arg = argv[i];
        if (arg=="-y")
            ffOutput = marty::json_utils::FileFormat::yaml;
        else if (arg=="-j")
            ffOutput = marty::json_utils::FileFormat::json;
        else if (arg=="-h" || arg=="--help")
            return printUsage();
        else
            args.push_back(argv[i]);
    }


    #if defined(_DEBUG)
        if (args.size()==0)
        {
            // static const char* nameOrg = "F:\\_github\\marty-yaml2json\\tests\\marty-json-diff-01.txt";
            // static const char* nameNew = "F:\\_github\\marty-yaml2json\\tests\\marty-json-diff-02.txt";
            static const char* nameOrg = "F:\\_github\\marty-yaml2json\\tests\\marty-yaml-diff-01.txt";
            static const char* nameNew = "F:\\_github\\marty-yaml2json\\tests\\marty-yaml-diff-02.txt";
            args.push_back(nameOrg);
            args.push_back(nameNew);
        }
    #endif


    if (args.size()<1)
        return printUsage("Missing input 'original' file name");

    if (args.size()<2)
        return printUsage("Missing input 'new' file name");

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


    std::ifstream newFile(args[1], std::ios_base::in);
    if (!newFile)
    {
        std::cerr << "Failed to open input file: " << args[1] << std::endl;
        return (1);
    }

    marty::json_utils::FileFormat ffNew = marty::json_utils::FileFormat::unknown;
    nlohmann::json jNew = marty::json_utils::parseJsonOrYaml( newFile, true // allowComments
                                                            , &errMsg, &tmpJson
                                                            , &ffNew
                                                            );
    if (ffNew==marty::json_utils::FileFormat::unknown)
    {
        std::cerr << "Failed to parse input file: " << args[1] << std::endl;
        std::cerr << "Error: " << errMsg << std::endl;
        std::cerr << "JSON :" << std::endl << tmpJson << std::endl;
        return (1);
    }


    auto jDiff = nlohmann::json::diff( jOrg, jNew );


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


    std::string serializedJsonDiff = jDiff.dump(2);

    // Если выходной формат не задан, и один из входных файлов - в формате YAML, то задаём формат YAML
    if (ffOutput == marty::json_utils::FileFormat::unknown)
    {
        if (ffOrg==marty::json_utils::FileFormat::yaml || ffNew==marty::json_utils::FileFormat::yaml)
            ffOutput = marty::json_utils::FileFormat::yaml;
        else
            ffOutput = marty::json_utils::FileFormat::json;
    }


    if (ffOutput == marty::json_utils::FileFormat::json)
    {
        out << serializedJsonDiff;
        return 0;
    }
    else if (ffOutput == marty::json_utils::FileFormat::yaml)
    {
        try
        {
            YAML::Node diffNode = YAML::Load(serializedJsonDiff);
            // YAML::Emitter emitter;
            // emitter << "Hello world!";
            // std::ofstream fout("file.yaml");
            // out << diffNode;
            out << YAML::Dump(diffNode);

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

