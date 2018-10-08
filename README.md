# libmpl - C/C++ Mini Public Library

![](https://img.shields.io/badge/license-Apache2-green.svg "License")

libmpl is a simple and easy use C++ public library. 

## Compiling

The simplest way to compile this package is:

1. Type `./autogen.sh` to create *configure* file.
2. Type `./configure` to configure the package for you system.
3. Type `make -j4` to compile the package.

## Usage at a glance

This simple example demonstrate how to use libmpl.

~~~~~~~~~~cpp
#include <mpl.h>

int main(int argc, char *argv[])
{
    std::cout << mpl::welcome() << std::endl;
    return 0;
}
~~~~~~~~~~

## Licence

Apache v2.0
