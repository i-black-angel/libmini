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
#ifndef _MTHREAD_H_
#define _MTHREAD_H_

#include <mini/mcoredef.h>

MINI_BEGIN_NAMESPACE

class MThread
{
public:

	enum Priority {
        IdlePriority,

        LowestPriority,
        LowPriority,
        NormalPriority,
        HighPriority,
        HighestPriority,

        TimeCriticalPriority,

        InheritPriority
    };

    explicit MThread();
    virtual ~MThread();

	bool start();
	void stop();

	int join();
	int detach();
	int cancel();
	
	static int64_t currentId();
	int64_t id();
	
    void setPriority(Priority priority);
    Priority priority() const;	

	//  This is internal function. It should be private, however then
	//  it would not be accessible from the main C routine of the thread.
	void exec();

protected:

	virtual void run();

private:

#ifdef M_OS_WIN
	HANDLE _self;
#else
	pthread_t _self;
#endif /* M_OS_WIN */

	Priority _priority;
	
	M_DISABLE_COPY(MThread)
};

MINI_END_NAMESPACE

#endif /* _MTHREAD_H_ */
