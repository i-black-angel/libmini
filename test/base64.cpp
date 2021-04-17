#include <iostream>
#include "mpl/mbase64.h"

int main(int argc, char const *argv[])
{
    std::string str = "https://github.com//fengbingchun";
    int outlen = (str.size() + 2) / 3 * 4 + ((str.size() + 2) / 3 * 4 + 63) / 64;
    std::string out;
    out.resize(outlen);
    mpl::base64Encode((const uint8_t *)str.c_str(), str.size(), &out[0], &outlen);
    std::cout << outlen << std::endl;
    std::cout << out << std::endl;
    std::string dst;
    dst.resize(out.size());
    int outlen1 = 0;
    mpl::base64Decode(out.c_str(), out.size(), (uint8_t *)&dst[0], &outlen1);
    std::cout << dst << std::endl;
    return 0;
}
