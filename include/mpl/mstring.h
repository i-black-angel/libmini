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
#ifndef _MSTRING_H_
#define _MSTRING_H_

#include <mpl/mcoredef.h>

MPL_BEGIN_NAMESPACE

std::string toString(int val);
std::string toString(unsigned val);
std::string toString(long val);
std::string toString(unsigned long val);
std::string toString(long long val);
std::string toString(unsigned long long val);
std::string toString(float val);
std::string toString(double val);
std::string toString(long double val);
std::string toXString(const char *fmt, ...);
std::string hex2str(const uint8_t *data, size_t len);
#define format toXString

class MString : public std::string
{
public:
    MString();
	MString(const char *s);
	MString(const std::string &str);
    virtual ~MString();

	std::string& replace(char before, char after);
	std::string& replace(const std::string &before, const std::string &after);
};

MPL_END_NAMESPACE

#endif /* _MSTRING_H_ */
