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
#ifndef _MOPTIONS_H_
#define _MOPTIONS_H_

#include <minion/mcoredef.h>

MINION_BEGIN_NAMESPACE

class MOptions
{
public:
	struct option_t
	{
		int key;
		std::string longopt;
		std::string desc;
		bool req_arg;

		bool operator==(int x) const
			{ return (key == x); }
		bool operator!=(int x) const
			{ return !operator==(x); }
	};
	
    explicit MOptions(const char *version, const char *description);
    virtual ~MOptions();

	void insert(int key, const std::string &longopt, const std::string &desc, bool reqArg = true);
	bool find(int key) const;
	int getint(int key) const;
	std::string getstring(int key) const;
	std::string value(int key) const;
	void parse(int argc, char *argv[]);
protected:
	void help() const;
	void version() const;
private:
	// Types
	typedef	std::vector<struct option_t> options_type;
	typedef	std::vector<struct option_t>::const_iterator options_const_iterator;
	typedef std::map<int, std::string> values_type;
	typedef std::map<int, std::string>::const_iterator values_const_iterator;

	options_type _options;
	values_type _values;
	std::string _version;
	std::string _desc;
};

MINION_END_NAMESPACE

#endif /* _MOPTIONS_H_ */
