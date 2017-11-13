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
#include <punica/pdatetime.h>

PUNICA_BEGIN_NAMESPACE

std::string now()
{
	return PDateTime::now();
}

PDateTime::PDateTime()
{
}

PDateTime::PDateTime(int y, int m, int d, int H, int M, int S)
{
	struct tm stm;
	stm.tm_year = y - 1900;
	stm.tm_mon = m - 1;
	stm.tm_mday = d;
	stm.tm_hour = H;
	stm.tm_min = M;
	stm.tm_sec = S;

	_d = mktime(&stm);
}

PDateTime::PDateTime(const PDateTime &other)
{
	_d = other._d;
}

PDateTime::~PDateTime()
{
}

std::string PDateTime::now(const std::string &format)
{
	PDateTime datetime = currentDateTime();
	std::string local_format = format;
	if (format.empty()) {
		local_format = "%Y-%m-%d %H:%M:%S";
	}
	char buffer[64] = {0};
	strftime(buffer, sizeof(buffer), local_format.c_str(), localtime(&(datetime._d)));
	return std::string(buffer);
}

PDateTime PDateTime::currentDateTime()
{
	PDateTime datetime;
	datetime._d = time(NULL);
	return datetime;
}

PUNICA_END_NAMESPACE
