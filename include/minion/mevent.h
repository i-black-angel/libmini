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
#ifndef _MEVENT_H_
#define _MEVENT_H_

#include <minion/mcoredef.h>
#include <minion/mcondition.h>
#include <minion/mmutex.h>

MINION_BEGIN_NAMESPACE

class MEvent
{
public:
    explicit MEvent();
    virtual ~MEvent();

	// timeout millseconds
	inline void wait(unsigned long timeout = ULONG_MAX)
		{
			MMutexLocker lock(_mutex);
			_condition.wait(_mutex, timeout);
		}

	inline void signal()
		{
			_condition.wake();
		}

	inline void wakeAll()
		{
			_condition.wakeAll();
		}
private:
	M_DISABLE_COPY(MEvent)

	MMutex _mutex;
	MCondition _condition;
};


MINION_END_NAMESPACE

#endif /* _MEVENT_H_ */
