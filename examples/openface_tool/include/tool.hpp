#ifndef TOOL_HPP
#define TOOL_HPP

#include <iostream>
#include "aligntool.hpp"

class OpenfaceTool {
public:
    OpenfaceTool(int argc, char *argv[]);
    ~OpenfaceTool() {}

private:
    AlignTool at;
    void usage(OptionDescription&, PositionalArguments&);
};

#endif
