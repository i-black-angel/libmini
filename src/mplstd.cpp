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
#include <mpl/mplstd.h>

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

// Begin mpl namespace
MPL_BEGIN_NAMESPACE

const char *welcome()
{
	return "Hi, welcome to Mini Public Library.";
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

double power(unsigned int base, unsigned int expo)
{
	return (expo == 0) ? 1 : base * power(base, expo - 1);
}

/* idea of this function is copied from top size scaling */
std::string scaleSize(unsigned long size)
{
	static char nextup[] = { 'B', 'K', 'M', 'G', 'T', 0 };
	char buf[BUFSIZ];
	int i;
	char *up;
	float base;

	base = 1024.0;

	/* human readable output */
	up = nextup;
	for (i = 1; up[0] != '0'; i++, up++) {
		switch (i) {
		// case 1:
		// 	if (4 >= snprintf(buf, sizeof(buf), "%ld%c", (long)size * 1024, *up))
		// 		return buf;
		// 	break;
		// case 2:
		// {
		// 	if (4 >= snprintf(buf, sizeof(buf), "%ld%c", size, *up)) 
		// 		return buf;
		// }
		// break;
		case 1:
			if (4 > snprintf(buf, sizeof(buf), "%ld", size))
				return buf;
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			if (4 >=
				snprintf(buf, sizeof(buf), "%.1f%c",
						 (float)(size / power(base, i - 1)), *up))
				return buf;
			if (4 >=
				snprintf(buf, sizeof(buf), "%ld%c",
						 (long)(size / power(base, i - 1)), *up))
				return buf;
			break;
		case 6:
			break;
		}
	}
	/*
	 * On system where there is more than petabyte of memory or swap the
	 * output does not fit to column. For incoming few years this should
	 * not be a big problem (wrote at Apr, 2011).
	 */
	return std::string(buf);
}

MPL_END_NAMESPACE

