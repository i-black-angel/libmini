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
#include <punica/pthread.h>

#ifdef P_OS_WIN

#ifdef __cplusplus
extern "C" {
#endif

	static unsigned int __stdcall thread_routine(void *arg)
	{
		punica::PThread *self = (punica::PThread *) arg;
		if (NULL != self) {
			self->exec();
		}
		return 0;
	}
	
#ifdef __cplusplus
}
#endif

#else  /* LINUX */

#ifdef __cplusplus
extern "C" {
#endif

	static void *thread_routine(void *arg)
	{
		punica::PThread *self = (punica::PThread *) arg;
		if (NULL != self) {
			self->exec();
		}
		return NULL;
	}

#ifdef __cplusplus
}
#endif

#endif /* P_OS_WIN */


PUNICA_BEGIN_NAMESPACE

PThread::PThread()
{
	_self = 0;
}

PThread::~PThread()
{
}

bool PThread::start()
{
	int res = 0;
	res = pthread_create(&_self, NULL, thread_routine, this);
	if (0 != res)
		perror("create thread");
	return res == 0;
}

void PThread::stop()
{
}

int PThread::join()
{
}

int PThread::detach()
{
}

int PThread::cancel()
{
}
	
int64_t PThread::currentId()
{
	return pthread_self();
}

int64_t PThread::id()
{
	return _self;
}
	
void PThread::setPriority(Priority priority)
{
	_priority = priority;
}

PThread::Priority PThread::priority() const
{
	return _priority;
}

void PThread::exec()
{
	run();
}

void PThread::run()
{
}

PUNICA_END_NAMESPACE
