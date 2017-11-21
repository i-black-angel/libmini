#!/bin/sh
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

if [ -f Makefile ]; then
	echo "Making make distclean..."
	make distclean
fi
echo "Removing autogenned files..."
rm -f config.guess config.sub configure install-sh missing mkinstalldirs Makefile.in ltmain.sh stamp-h.in */Makefile.in ltconfig stamp-h config.h.in config.h.in~ aclocal.m4 ar-lib compile depcomp
rm -f m4/libtool.m4 m4/lt~obsolete.m4 m4/ltoptions.m4 m4/ltsugar.m4 m4/ltversion.m4
rm -rf autom4te.cache
echo "Done."
