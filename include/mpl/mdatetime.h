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
#ifndef _MDATETIME_H_
#define _MDATETIME_H_

#include <mpl/mcoredef.h>

MPL_BEGIN_NAMESPACE

std::string now();

class MTime
{
public:
    MTime();
	// MTime(int h, int m, int s = 0, int ms = 0);
    virtual ~MTime();

	// int hour() const;
	// int minute() const;
	// int second() const;
	// int msec() const;
	// std::string toString(const std::string &format = "") const;
	// bool setHMS(int h, int m, int s, int ms = 0);

	void start();
	int restart();
	int elapsed() const;
private:
#ifdef M_OS_WIN
	clock_t _start;		
#else
	struct timeval _start;
#endif /* M_OS_WIN */
};

class MDateTime
{
public:
    MDateTime();
	MDateTime(time_t val);
	explicit MDateTime(int y, int m, int d, int H = 0, int M = 0, int S = 0);
    MDateTime(const MDateTime &other);
    virtual ~MDateTime();

	MDateTime &operator=(const MDateTime &other);
	MDateTime &operator=(time_t val);
	
	int year() const;
	int month() const;
	int day() const;
	int hour() const;
	int minute() const;
	int second() const;
	int msec() const;
	int dayOfWeek() const;
	int dayOfYear() const;
	
	time_t data() const { return _d; }
	std::string toString(const std::string &format = "") const;
	
    bool operator==(const MDateTime &other) const { return _d == other._d; }
    bool operator!=(const MDateTime &other) const { return _d != other._d; }
    bool operator< (const MDateTime &other) const { return _d <  other._d; }
    bool operator<=(const MDateTime &other) const { return _d <= other._d; }
    bool operator> (const MDateTime &other) const { return _d >  other._d; }
    bool operator>=(const MDateTime &other) const { return _d >= other._d; }

	static std::string now(const std::string &format = "");
	static MDateTime currentDateTime();
private:
	time_t _d;
};

std::ostream &operator<<(std::ostream &, const MDateTime &);
// std::istream &operator>>(std::istream &, MHostAddress &);

MPL_END_NAMESPACE

#endif /* _MDATETIME_H_ */
