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
#ifndef _MMUTEX_H_
#define _MMUTEX_H_

#include <minion/mcoredef.h>
#include <minion/mglobal.h>

MINION_BEGIN_NAMESPACE

#ifdef M_OS_WIN /* M_OS_WIN */

class MMutex
{
public:
    MMutex()
		{
			InitializeCriticalSection (&cs);
		}

    ~MMutex()
		{
			DeleteCriticalSection (&cs);
		}

	inline void lock()
		{
			EnterCriticalSection (&cs);
		}

	inline bool trylock()
		{
			return (TryEnterCriticalSection (&cs)) ? true : false;
		}

	inline void unlock()
		{
			LeaveCriticalSection (&cs);
		}

private:

	M_DISABLE_COPY(MMutex)

	CRITICAL_SECTION cs;
};

#else  /* M_OS_LINUX */

class MMutex
{
public:
    MMutex()
		{
			int rc = pthread_mutex_init (&_mutex, NULL);
			posix_assert (rc);
		}

    ~MMutex()
		{
            int rc = pthread_mutex_destroy (&_mutex);
            posix_assert (rc);			
		}

	inline void lock()
		{
            int rc = pthread_mutex_lock (&_mutex);
            posix_assert (rc);			
		}

	inline bool trylock()
		{
            int rc = pthread_mutex_trylock (&_mutex);
            if (rc == EBUSY)
                return false;

            posix_assert (rc);
            return true;
		}

	inline void unlock()
		{
            int rc = pthread_mutex_unlock (&_mutex);
            posix_assert (rc);
		}

	inline pthread_mutex_t *mutex()
		{
			return &_mutex;
		}
private:

	M_DISABLE_COPY(MMutex)

	pthread_mutex_t _mutex;
};

#endif	/* M_OS_LINUX */


class MMutexLocker
{
public:
    explicit MMutexLocker(MMutex &mutex)
		: _mutex(mutex)
		{
			_mutex.lock();
		}
	
    ~MMutexLocker()
		{
			_mutex.unlock();
		}
	
private:
	M_DISABLE_COPY(MMutexLocker)
	
	MMutex &_mutex;
};

MINION_END_NAMESPACE

#endif /* _MMUTEX_H_ */
