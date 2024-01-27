#include <vector>
#include <cstdint>
#include <cassert>
#include <string>
#include <fstream>
#include <string.h>
#include <stack>
#include <algorithm>
#include <span>
#include <filesystem>

#include "cc_algorithm.hpp"
#include "cc_file.hpp"
#include "helper.hpp"

int do_extract(const std::string cc_filepath, const bool extract)
{
    try
    {
        const std::vector<uint8_t> content = read_binary_file(cc_filepath);

        int res = 0;
#if 1
        printf("try extracting as CC0\n");
        res = cc0_uncompress(cc_filepath, content, extract);
        if (res == 0)
        {
            printf("CC0 successfull uncompressed\n");
            return 0;
        }
        printf("not a CC0!\n");
#endif

        printf("try extracting as CC1\n");
        res = cc1_uncompress(cc_filepath, content, extract);
        if (res == 0)
        {
            printf("CC1 successfull uncompressed\n");
            return 0;
        }
        printf("not a CC1!\n");

        printf("unknown file format!\n");

        return 1;
    }
    catch (const int& e_)
    {
        return e_;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    if ((argc < 2) || (argc > 3))
    {
        printf("uncompress_cc1 CC-FILE [e]\n");
        return -1;
    }

    const std::string cc_filepath = argv[1];
    printf("%s\n", cc_filepath.c_str());

    bool extract = false;
    if (argc == 3)
    {
        const std::string arg2 = argv[2];
        if (arg2 != "e")
        {
            printf("optional second argument needs to be 'e'\n");
            return -1;
        }
        extract = true;
    }

    return do_extract(cc_filepath, extract);
}
