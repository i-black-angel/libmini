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
#include <mini/mthread.h>

#ifdef M_OS_WIN

#ifdef __cplusplus
extern "C" {
#endif

	static unsigned int __stdcall thread_routine(void *arg)
	{
		mini::MThread *self = (mini::MThread *) arg;
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
		mini::MThread *self = (mini::MThread *) arg;
		if (NULL != self) {
			self->exec();
		}
		return NULL;
	}

#ifdef __cplusplus
}
#endif

#endif /* M_OS_WIN */

MINI_BEGIN_NAMESPACE

MThread::MThread()
{
	_self = 0;
}

MThread::~MThread()
{
}

bool MThread::start()
{
	int res = 0;
	res = pthread_create(&_self, NULL, thread_routine, this);
	if (0 != res)
		perror("create thread");
	return res == 0;
}

void MThread::stop()
{
}

int MThread::join()
{
}

int MThread::detach()
{
}

int MThread::cancel()
{
}
	
int64_t MThread::currentId()
{
	return pthread_self();
}

int64_t MThread::id()
{
	return _self;
}
	
void MThread::setPriority(Priority priority)
{
	_priority = priority;
}

MThread::Priority MThread::priority() const
{
	return _priority;
}

void MThread::exec()
{
	run();
}

void MThread::run()
{
}

MINI_END_NAMESPACE