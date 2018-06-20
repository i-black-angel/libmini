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
#include <minion/mstring.h>

MINION_BEGIN_NAMESPACE

MString::MString()
{
}

MString::~MString()
{
}

std::string MString::format(const char *__format, ...)
{
	int len;
	std::string buffer;
	va_list vargs;
	va_start(vargs, __format);
	len = vsnprintf(NULL, 0, __format, vargs);
	buffer.resize(len + 2);
	va_start(vargs, __format);
	vsnprintf(&buffer[0], len + 1, __format, vargs);
	va_end(vargs);

	return buffer;
}

std::string MString::hex2str(const uint8_t *data, size_t len)
{
	std::string buffer;
	char buf[3] = {0x00};
	for (size_t m = 0; m < len; ++m) {
		snprintf (buf, sizeof(buf) - 1, "%02X", data[m]);
		buf[2] = '\0';
		buffer += buf;
		buffer += " ";
	}

	if (buffer.size() > 0) {
		buffer.erase(buffer.begin() + (buffer.size() - 1));
	}
	return buffer;
}

MINION_END_NAMESPACE
