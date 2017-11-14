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
#ifndef _MCONDITION_H_
#define _MCONDITION_H_

#include <mini/mcoredef.h>
#include <mini/mmutex.h>
#include <limits.h>

MINI_BEGIN_NAMESPACE

class MCondition
{
public:
    /*explicit */MCondition();
    virtual ~MCondition();

	bool wait(MMutex &mutex, unsigned long timeout = ULONG_MAX);

	void wake();
	void wakeAll();

private:
	M_DISABLE_COPY(MCondition)

#ifdef M_OS_WIN
	HANDLE _cond;
#else
	pthread_cond_t _cond;
#endif /* M_OS_WIN */
};

MINI_END_NAMESPACE

#endif /* _MCONDITION_H_ */
