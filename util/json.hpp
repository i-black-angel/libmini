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
#ifndef _JSON_H_
#define _JSON_H_

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

namespace json {
	typedef rapidjson::Document Document;
	typedef rapidjson::ParseResult ParseResult;
	typedef rapidjson::Value Value;
	
	int getInt(const Value &genericValue, const std::string &name);
	unsigned getUint(const Value &genericValue, const std::string &name);
	int64_t getInt64(const Value &genericValue, const std::string &name);
	uint64_t getUint64(const Value &genericValue, const std::string &name);
	double getDouble(const Value &genericValue, const std::string &name);
	float getFloat(const Value &genericValue, const std::string &name);
	std::string getString(const Value &genericValue, const std::string &name);
	bool getBool(const Value &genericValue, const std::string &name);
	Value &getObject(Value &genericValue, const std::string &name);
	const Value &getObject(const Value &genericValue, const std::string &name);
	std::string writer(const Value &doc);
	std::string prettyWriter(const Value &doc);
}

#endif /* _JSON_H_ */
