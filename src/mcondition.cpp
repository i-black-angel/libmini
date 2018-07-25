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
#include <mpl/mcondition.h>

MPL_BEGIN_NAMESPACE

MCondition::MCondition()
{
	pthread_cond_init(&_cond, NULL);
}

MCondition::~MCondition()
{
	pthread_cond_destroy(&_cond);
}

bool MCondition::wait(MMutex &mutex, unsigned long timeout)
{
	int code = 0;
	if (timeout != ULONG_MAX) {
		struct timespec ts;
		struct timeval tv;
		gettimeofday(&tv, NULL);
		ts.tv_sec = tv.tv_sec;
		ts.tv_nsec = tv.tv_usec * 1000;  /* usec to nsec */
		// add the offset to get timeout value
		ts.tv_sec += timeout / 1000;
		ts.tv_nsec += timeout % 1000 * 1000000;
		code = pthread_cond_timedwait(&_cond, mutex.mutex(), &ts);
	} else {
		code = pthread_cond_wait(&_cond, mutex.mutex());
	}
	return (code == 0);
}

void MCondition::wake()
{
	pthread_cond_signal(&_cond);
}

void MCondition::wakeAll()
{
	pthread_cond_broadcast(&_cond);
}

MPL_END_NAMESPACE
