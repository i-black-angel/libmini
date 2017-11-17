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
#ifndef _MPROCESS_H_
#define _MPROCESS_H_

#include <mini/mcoredef.h>

MINI_BEGIN_NAMESPACE

class MProcess
{
public:
    explicit MProcess();
    virtual ~MProcess();

	int64_t pid() const;
	std::string program() const;
	std::string workingDirectory() const;
	static int execute(const std::string &program, const std::vector<std::string> &arguments);
	static int execute(const std::string &command);
	static std::vector<std::string> systemEnvironment();
};

MINI_END_NAMESPACE

#endif /* _MPROCESS_H_ */
