#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main() {
    std::cout << fs::current_path() << std::endl;
    return 0;
}

