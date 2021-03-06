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
#include "json.hpp"

namespace json {
	
	int getInt(const Value &genericValue, const std::string &name)
	{
		Value::ConstMemberIterator it = genericValue.FindMember(name);
		if (it != genericValue.MemberEnd() && it->value.IsInt())
			return it->value.GetInt();
		return 0;
	}

	unsigned getUint(const Value &genericValue, const std::string &name)
	{
		Value::ConstMemberIterator it = genericValue.FindMember(name);
		if (it != genericValue.MemberEnd() && it->value.IsUint())
			return it->value.GetUint();
		return 0;
	}

	int64_t getInt64(const Value &genericValue, const std::string &name)
	{
		Value::ConstMemberIterator it = genericValue.FindMember(name);
		if (it != genericValue.MemberEnd() && it->value.IsInt64())
			return it->value.GetInt64();
		return 0;
	}

	uint64_t getUint64(const Value &genericValue, const std::string &name)
	{
		Value::ConstMemberIterator it = genericValue.FindMember(name);
		if (it != genericValue.MemberEnd() && it->value.IsUint64())
			return it->value.GetUint64();
		return 0;
	}

	double getDouble(const Value &genericValue, const std::string &name)
	{
		Value::ConstMemberIterator it = genericValue.FindMember(name);
		if (it != genericValue.MemberEnd() && it->value.IsDouble())
			return it->value.GetDouble();
		return 0.0;
	}

	float getFloat(const Value &genericValue, const std::string &name)
	{
		Value::ConstMemberIterator it = genericValue.FindMember(name);
		if (it != genericValue.MemberEnd() && it->value.IsFloat())
			return it->value.GetFloat();
		return 0.0f;
	}

	std::string getString(const Value &genericValue, const std::string &name)
	{
		Value::ConstMemberIterator it = genericValue.FindMember(name);
		if (it != genericValue.MemberEnd() && it->value.IsString())
			return it->value.GetString();
		return "";
	}

	bool getBool(const Value &genericValue, const std::string &name)
	{
		Value::ConstMemberIterator it = genericValue.FindMember(name);
		if (it != genericValue.MemberEnd() && it->value.IsBool())
			return it->value.GetBool();
		return false;
	}

	Value &getObject(Value &genericValue, const std::string &name)
	{
		Value::MemberIterator it = genericValue.FindMember(name);
		if (it != genericValue.MemberEnd() && it->value.IsObject())
			return it->value;
		return *(new Value());
	}

	const Value &getObject(const Value &genericValue, const std::string &name)
	{
		return const_cast<Value &>(getObject(genericValue, name));
	}

	std::string writer(const Value &doc)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> w(buffer);
		doc.Accept(w);
		return buffer.GetString();
	}

	std::string prettyWriter(const Value &doc)
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> w(buffer);
		doc.Accept(w);
		return buffer.GetString();
	}
}
