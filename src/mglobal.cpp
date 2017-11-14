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
#include <mini/mglobal.h>
#include <mini/muuid.h>

MINI_BEGIN_NAMESPACE

const char *welcome()
{
	return "Hello, welcome to libmini.";
}

void abort(const char *errmsg)
{
#if defined M_OS_WIN
    //  Raise STATUS_FATAL_APP_EXIT.
    ULONG_PTR extra_info [1];
    extra_info [0] = (ULONG_PTR) errmsg;
    RaiseException (0x40000015, EXCEPTION_NONCONTINUABLE, 1, extra_info);
#else
    (void)errmsg;
    ::abort ();
#endif
}

MINI_END_NAMESPACE

#ifdef M_OS_WIN

void gettimeofday (struct timeval *tv, void *dummy)
{
	FILETIME	ftime;
	uint64_t	n;

	GetSystemTimeAsFileTime (&ftime);
	n = (((uint64_t) ftime.dwHighDateTime << 32)
	     + (uint64_t) ftime.dwLowDateTime);
	if (n) {
		n /= 10;
		n -= ((369 * 365 + 89) * (uint64_t) 86400) * 1000000;
	}

	tv->tv_sec = n / 1000000;
	tv->tv_usec = n % 1000000;
}

#endif /* M_OS_WIN */
