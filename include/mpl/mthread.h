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

#include <mpl/mcoredef.h>
#include <mpl/mmutex.h>

MPL_BEGIN_NAMESPACE

class MThread
{
public:
    MThread();
    virtual ~MThread();

	bool start();
	void stop();

	int join();
	int detach();
	int cancel();
	
	int64_t id();
	
	void interrupt();
	bool isInterrupted() const;

protected:
	virtual void run() = 0;

private:
#ifdef M_OS_WIN
	static unsigned int threadRoutine(void *arg);
	HANDLE _handle;
	unsigned int _id;
#else
	static void *threadRoutine(void *arg);
	pthread_t _self;
#endif /* M_OS_WIN */

	MMutex _mutex;
	bool _interrupt;
	
	M_DISABLE_COPY(MThread)
};

// get_thread_id
int64_t threadId();

MPL_END_NAMESPACE

#endif /* _MTHREAD_H_ */
