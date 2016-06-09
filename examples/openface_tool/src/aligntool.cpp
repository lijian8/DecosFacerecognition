#include "aligntool.hpp"

void AlignTool::initialize(int argc, char *argv[]) {
    namespace po = boost::program_options;
    OptionDescription desc("Operations");

    std::string image_in;

    desc.add_options()
        ("image_in", po::value<std::string>(&image_in)->required(), "Path to input image");

    PositionalArguments operations;
    operations.add("image_in", 1);

    VariablesMap vm;
    po::store(po::command_line_parser(argc, argv).options(desc)
            .positional(operations).run(),vm);

    align(image_in);
}

void AlignTool::align(std::string path) {
    std::cout << path << std::endl;
}
