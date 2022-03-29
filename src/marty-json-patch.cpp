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


using umba::lout;
using namespace umba::omanip;
using std::cout; 
using std::cerr; 



inline
int printUsage( const char *msg = 0)
{
    if (msg)
        lout << msg << endl;
    cerr << "Usage: marty-json-patch original diff [new]" << std::endl;
    cerr << "If optional [new] file name not taken, STDOUT is used" << std::endl;
    return 1;
}
 



int main( int argc, char* argv[] )
{

    #if 0 // defined(_DEBUG)
        argc = 3;
        // Может упасть
        argv[1] = (char*)"F:\\_github\\marty-yaml2json\\tests\\marty-json-diff-01.txt";
        argv[2] = (char*)"F:\\_github\\marty-yaml2json\\tests\\marty-json-diff-02.txt";
    #endif


    if (argc<2)
        return printUsage("Missing input 'original' file name");

    if (argc<3)
        return printUsage("Missing input 'diff' file name");

    std::ifstream orgFile(argv[1], std::ios_base::in);
    if (!orgFile)
    {
        std::cerr << "Failed to open input file: " << argv[1] << std::endl;
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
        std::cerr << "Failed to parse input file: " << argv[1] << std::endl;
        std::cerr << "Error: " << errMsg << std::endl;
        std::cerr << "JSON :" << std::endl << tmpJson << std::endl;
        return (1);
    }


    std::ifstream diffFile(argv[2], std::ios_base::in);
    if (!diffFile)
    {
        std::cerr << "Failed to open input file: " << argv[2] << std::endl;
        return (1);
    }

    marty::json_utils::FileFormat ffDiff = marty::json_utils::FileFormat::unknown;
    nlohmann::json jDiff = marty::json_utils::parseJsonOrYaml( diffFile, true // allowComments
                                                            , &errMsg, &tmpJson
                                                            , &ffDiff
                                                            );
    if (ffDiff==marty::json_utils::FileFormat::unknown)
    {
        std::cerr << "Failed to parse input file: " << argv[2] << std::endl;
        std::cerr << "Error: " << errMsg << std::endl;
        std::cerr << "JSON :" << std::endl << tmpJson << std::endl;
        return (1);
    }


/*
    auto jDiff = nlohmann::json::diff( jOrg, jNew );


    std::ostream *pOut = &std::cout;

    std::ofstream outFile;
    if (argc>3)
    {
        outFile.open(argv[3], std::ios_base::out);
        if (!outFile)
        {
            std::cerr << "Failed to open output file: " << argv[3] << std::endl;
            return (1);
        }

        pOut = &outFile;
    }

    std::ostream &out = *pOut;

    if (ffOrg==marty::json_utils::FileFormat::yaml || ffNew==marty::json_utils::FileFormat::yaml)
    {
    }
    else
    {
    }

    out << jDiff.dump(2); // << std::end

*/

    return 0;
}



