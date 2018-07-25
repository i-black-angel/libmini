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
#include <mpl/mstring.h>

MPL_BEGIN_NAMESPACE

std::string toXString(const char *fmt, ...)
{
	std::string buffer;
	va_list vargs;
	va_start(vargs, fmt);
	int len = vsnprintf(NULL, 0, fmt, vargs);
	buffer.resize(len + 2);
	va_start(vargs, fmt);
	vsnprintf(&buffer[0], len + 1, fmt, vargs);
	va_end(vargs);

	return buffer;
}

std::string hex2str(const uint8_t *data, size_t len)
{
	std::string buffer;
	char buf[3] = {0x00};
	for (size_t m = 0; m < len; ++m) {
		snprintf (buf, sizeof(buf), "%02X", data[m]);
		buf[2] = '\0';
		buffer += buf;
		buffer += " ";
	}

	if (buffer.size() > 0) {
		buffer.erase(buffer.begin() + (buffer.size() - 1));
	}
	return buffer;
}

std::string toString(int val)
{
	return toXString("%d", val);
}

std::string toString(unsigned val)
{
	return toXString("%u", val);
}

std::string toString(long val)
{
	return toXString("%ld", val);
}

std::string toString(unsigned long val)
{
	return toXString("%lu", val);
}

std::string toString(long long val)
{
	return toXString("%lld", val);
}

std::string toString(unsigned long long val)
{
	return toXString("%llu", val);
}

std::string toString(float val)
{
	return toXString("%f", val);
}
	
std::string toString(double val)
{
	return toXString("%f", val);
}

std::string toString(long double val)
{
	return toXString("%Lf", val);
}

MString::MString()
{
}

MString::~MString()
{
}

MPL_END_NAMESPACE
