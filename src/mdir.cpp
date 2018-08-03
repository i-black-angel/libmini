/**
 * Copyright 2017 Shusheng Shao <iblackangel@163.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <mpl/mdir.h>
#include <mpl/mfileinfo.h>
#include <mpl/merror.h>
#include <mpl/mlog.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

MDir::MDir()
{
}

MDir::MDir(const char *s)
{
	_dir = s;
}

MDir::MDir(const std::string &str)
{
	_dir = str;
}

MDir::MDir(const MDir &other)
{
	inner_copy(other);
}

MDir::~MDir()
{
}

bool MDir::isDir() const
{
	return MFileInfo(_dir).isDir();
}

bool MDir::mkpath(const std::string &path)
{
	int err = 0;
	mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
	std::string dirpath = MFileInfo(path).filePath();
	char *str = new char[dirpath.size() + 1];
	int len = dirpath.size();
	strncpy(str, dirpath.data(), len);
	str[len] = '\0';
	for (int i = 0; i < len; ++i) {
		if ( str[i] == MFileInfo::separator() ) {
			str[i] = '\0';
			if (strlen(str) > 0 && !MFileInfo::exists(str)) {
				if ((err = mkdir(str, mode)) != 0) {
					log_error("mkdir %s failed: %s", str, error().c_str());
					delete[] str;
					return false;
				}
			}
			str[i] = MFileInfo::separator();
		}
	}
	
	if (strlen(str) > 0 && !MFileInfo::exists(str)) {
		if ((err = mkdir(str, mode)) != 0) {
			log_error("mkdir %s failed: %s", str, error().c_str());
			delete[] str;
			return false;
		}
	}
	
	delete[] str;
	return true;
}

MDir &MDir::operator=(const MDir &other)
{
	inner_copy(other);
	return *this;
}

MDir &MDir::operator=(const char *s)
{
	_dir = s;
	return *this;
}

MDir &MDir::operator=(const std::string &str)
{
	_dir = str;
	return *this;
}

std::ostream &operator<<(std::ostream &out, const MDir &dir)
{
	out << dir.dir();
	return out;
}

std::istream &operator>>(std::istream &in, MDir &dir)
{
	// TODO::
	std::string str;
	in >> str;
	dir = str;
	return in;
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
