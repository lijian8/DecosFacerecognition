#include "tool.hpp"

OpenfaceTool::OpenfaceTool(int argc, char *argv[]) {
    namespace po = boost::program_options;
    OptionDescription desc("Operations");

    desc.add_options()
        ("help", "Print this help message.")
        ("align", "Align face or directory of faces.")
        ("train", "Train a classifier.")
        ("query", "Query and image to recognize.");

    PositionalArguments operations;
    operations.add("help", 1);
    operations.add("align", 1);
    operations.add("train", 1);
    operations.add("query", 1);

    VariablesMap vm;
    po::store(po::command_line_parser(argc, argv).options(desc)
            .positional(operations).run(),vm);

    if( vm.count("help") ) {
        usage(desc, operations);
        return;
    }

    if( vm.count("align") ) {
        aligner.initialize()
    }

}

void OpenfaceTool::usage(OptionDescription& desc, PositionalArguments& operations) {
    std::cout << "Openface Tool\n\n" << "Operations:" << std::endl;
    for (size_t i = 0; i < operations.max_total_count(); i++) {
        std::cout << "\t" << operations.name_for_position(i) << std::endl;
    }
}
