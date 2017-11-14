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
#include <mini/mdatetime.h>

MINI_BEGIN_NAMESPACE

std::string now()
{
	return MDateTime::now();
}

MDateTime::MDateTime()
{
}

MDateTime::MDateTime(int y, int m, int d, int H, int M, int S)
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

MDateTime::MDateTime(const MDateTime &other)
{
	_d = other._d;
}

MDateTime::~MDateTime()
{
}

MDateTime &MDateTime::operator=(const MDateTime &other)
{
	_d = other._d;
	return *this;
}

int MDateTime::year() const
{
	return localtime(&_d)->tm_year + 1900;
}

int MDateTime::month() const
{
	return localtime(&_d)->tm_mon + 1;
}

int MDateTime::day() const
{
	return localtime(&_d)->tm_mday;
}

int MDateTime::hour() const
{
	return localtime(&_d)->tm_hour;
}

int MDateTime::minute() const
{
	return localtime(&_d)->tm_min;
}

int MDateTime::second() const
{
	return localtime(&_d)->tm_sec;
}

// int MDateTime::msec() const
// {
// }

int MDateTime::dayOfWeek() const
{
	return localtime(&_d)->tm_wday;
}

int MDateTime::dayOfYear() const
{
	return localtime(&_d)->tm_yday;
}

// int MDateTime::daysInMonth() const
// {
// }

// int MDateTime::daysInYear() const
// {
// }

// bool MDateTime::isNull() const
// {
// }

// bool MDateTime::isValid() const
// {
// }
	
std::string MDateTime::toString(const std::string &format) const
{
	std::string local_format = format;
	if (format.empty()) {
		local_format = "%Y-%m-%d %H:%M:%S";
	}
	char buffer[64] = {0};
	strftime(buffer, sizeof(buffer), local_format.c_str(), localtime(&_d));
	return std::string(buffer);
}

std::string MDateTime::now(const std::string &format)
{
	MDateTime datetime = currentDateTime();
	std::string local_format = format;
	if (format.empty()) {
		local_format = "%Y-%m-%d %H:%M:%S";
	}
	char buffer[64] = {0};
	strftime(buffer, sizeof(buffer), local_format.c_str(), localtime(&(datetime._d)));
	return std::string(buffer);
}

MDateTime MDateTime::currentDateTime()
{
	MDateTime datetime;
	datetime._d = time(NULL);
	return datetime;
}

std::ostream &operator<<(std::ostream &out, const MDateTime &datetime)
{
	out << datetime.toString();
	return out;
}

MINI_END_NAMESPACE
