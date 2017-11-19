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
#ifndef _MSTRING_H_
#define _MSTRING_H_

#include <minion/mcoredef.h>

MINION_BEGIN_NAMESPACE

class MString : public std::string
{
public:
    explicit MString();
    virtual ~MString();

	static std::string format(const char *__format, ...);
};

MINION_END_NAMESPACE

#endif /* _MSTRING_H_ */
