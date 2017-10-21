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
#ifndef _PMUTEX_H_
#define _PMUTEX_H_

#include <punica/pcoredef.h>
#include <punica/pglobal.h>

PUNICA_BEGIN_NAMESPACE

#ifdef P_OS_WIN /* P_OS_WIN */

class PMutex
{
public:
    PMutex()
		{
			InitializeCriticalSection (&cs);
		}

    ~PMutex()
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

	P_DISABLE_COPY(PMutex)

	CRITICAL_SECTION cs;
};

#else  /* P_OS_LINUX */

class PMutex
{
public:
    PMutex()
		{
			int rc = pthread_mutex_init (&_mutex, NULL);
			posix_assert (rc);
		}

    ~PMutex()
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

	P_DISABLE_COPY(PMutex)

	pthread_mutex_t _mutex;
};

#endif	/* P_OS_LINUX */


class PMutexLocker
{
public:
    explicit PMutexLocker(PMutex &mutex)
		: _mutex(mutex)
		{
			_mutex.lock();
		}
	
    ~PMutexLocker()
		{
			_mutex.unlock();
		}
	
private:
	P_DISABLE_COPY(PMutexLocker)
	
	PMutex &_mutex;
};

PUNICA_END_NAMESPACE

#endif /* _PMUTEX_H_ */
