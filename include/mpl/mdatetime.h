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
    virtual ~MTime();
	
	void start();
	int restart();
	int elapsed() const;
private:
#if defined(M_OS_WIN) || defined(_MSC_VER)
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
	MDateTime(const char *sz);
	MDateTime(const std::string &text);
	MDateTime(int y, int m, int d, int H = 0, int M = 0, int S = 0);
    MDateTime(const MDateTime &other);
    virtual ~MDateTime();

	inline bool isValid() const { return _valid; }
	
	MDateTime addDays(int days) const;
	MDateTime addSecs(int64_t s) const;
	int daysTo(const MDateTime &other) const;
	int64_t secsTo(const MDateTime &other) const;
	
	MDateTime &operator=(const MDateTime &other);
	MDateTime &operator=(time_t val);
	MDateTime &operator=(const std::string &text);
	MDateTime &operator=(const char *sz);
	
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

	// FORMAT controls the output.  Interpreted sequences are:
	// %Y   year
	// %m   month
	// %d   day
	// %H   hour
	// %M   minute
	// %S   second
	std::string toString(const std::string &format = "") const;
	
    bool operator==(const MDateTime &other) const { return _d == other._d; }
    bool operator!=(const MDateTime &other) const { return _d != other._d; }
    bool operator< (const MDateTime &other) const { return _d <  other._d; }
    bool operator<=(const MDateTime &other) const { return _d <= other._d; }
    bool operator> (const MDateTime &other) const { return _d >  other._d; }
    bool operator>=(const MDateTime &other) const { return _d >= other._d; }

	static std::string now(const std::string &format = "");
	static MDateTime currentDateTime();
protected:
	bool setDateTime(const std::string &text);
	void setDateTime(int y, int m, int d, int H = 0, int M = 0, int S = 0);
private:
	time_t _d;
	bool _valid;
};

std::ostream &operator<<(std::ostream &, const MDateTime &);
// std::istream &operator>>(std::istream &, MHostAddress &);

MPL_END_NAMESPACE

#endif /* _MDATETIME_H_ */
