#!/bin/sh
# Run this to generate all the initial makefiles, etc.
#
# Copyright 2017 Shusheng Shao <iblackangel@163.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e

DIE=0
_pwd="$(pwd)"
as_me="$(basename $0)"
srcdir="$(dirname $0)"
test -z "$srcdir" && srcdir=.

cd $srcdir

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "You must have autoconf installed to run $as_me."
    echo "Download the appropriate package for your distribution,"
    echo "or see http://www.gnu.org/software/autoconf"
    DIE=1
}

(libtoolize --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "You must have libtool installed to run $as_me."
	echo "Download the appropriate package for your distribution,"
	echo "or see http://www.gnu.org/software/libtool"
	DIE=1
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "You must have automake installed to run $as_me."
    echo "Download the appropriate package for your distribution,"
    echo "or see http://www.gnu.org/software/automake"
    DIE=1
}

if [ "$DIE" -eq 1 ]; then
    exit 1
fi 

mkdir -p m4

autoreconf --force --install --verbose || exit $?

echo "You can now run \`./configure'."

exit 0

# cd "$_pwd"
# "$srcdir/configure" "$@"

### autogen.sh ends here
