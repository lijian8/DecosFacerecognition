#ifndef ALIGNTOOL_HPP
#define ALIGNTOOL_HPP

#include "openface/facealigner.hpp"
#include <boost/program_options.hpp>

typedef boost::program_options::options_description OptionDescription;
typedef boost::program_options::variables_map VariablesMap;
typedef boost::program_options::positional_options_description PositionalArguments;

class AlignTool {
public:
    AlignTool(FaceAligner aligner) : aligner_(aligner) {};
    ~AlignTool() {};

    void initialize(int argc, char *argv[]);
    void align(std::string);

private:
    FaceAligner aligner_;
};

#endif
