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
#ifndef _MCOREDEF_H_
#define _MCOREDEF_H_

/* Include standard headers */
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdint.h>
#include <stdarg.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <locale>
#include <deque>
#include <map>
#include <stack>
#include <stdexcept>
#include <sys/stat.h>

#ifdef _MSC_VER
# ifndef M_OS_WIN
#  define M_OS_WIN
# endif
#else
# ifndef M_OS_LINUX
#  define M_OS_LINUX
# endif
#endif

#ifdef M_OS_WIN
# include <ws2tcpip.h>
# include <winsock2.h>
# include <windows.h>
# include <io.h>
# include <process.h>    /* _beginthreadex */
# include <direct.h>     /* _getcwd */
# include <shlwapi.h>	 /* PathRemoveFileSpec */
#else
# include <pwd.h>
# include <grp.h>
# include <unistd.h>
# include <netdb.h>
# include <getopt.h>
# include <syslog.h>
# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/statfs.h>
# include <sys/file.h>
# include <sys/epoll.h>
# include <sys/sysinfo.h>
# include <sys/utsname.h>
#endif

#ifdef max
#undef max
#endif /* max */
#ifdef min
#undef min
#endif /* min */
#include <algorithm>

/* Definitions */
#ifdef M_OS_WIN
#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS	// stupid MSFT "deprecation" warning
#endif
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "shlwapi.lib")
#endif

#ifdef M_OS_WIN
# ifndef __socket_t_defined
#  define __socket_t_defined
typedef SOCKET socket_t;
# endif
typedef     uint32_t       in_addr_t;
typedef     uint16_t       in_port_t;
typedef     size_t         ssize_t;
/* Standard file descriptors.  */
# define	STDIN_FILENO	0	/* Standard input.  */
# define	STDOUT_FILENO	1	/* Standard output.  */
# define	STDERR_FILENO	2	/* Standard error output.  */
# define    getcwd          _getcwd
# define    chdir           _chdir
# define    rmdir           _rmdir
# define    snprintf        _snprintf
# define    useconds_t      DWORD
# define    sleep(n)        Sleep((n) * 1000)
# define    usleep(n)       Sleep((n) / 1000)
# define    DIRECTORY_SEPARATOR   "\\"
# define    R_OK            4
# define    W_OK            2
# define    F_OK            0
#else  /* non-windows */
# ifndef __socket_t_defined
#  define __socket_t_defined
typedef int socket_t;
# endif
# define    DIRECTORY_SEPARATOR   "/"
#endif

#ifndef __byte_defined
# define __byte_defined
typedef unsigned char byte;
#endif

#ifndef _MSC_VER
# define WINAPI
# define CALLBACK
#endif

#define M_OK     0
#define M_ERROR  1

#ifdef __cplusplus

# define MPL_BEGIN_NAMESPACE namespace mpl {
# define MPL_END_NAMESPACE }

#else /* __cplusplus */

# define MPL_BEGIN_NAMESPACE 
# define MPL_END_NAMESPACE

#endif /* __cplusplus */

#if defined __GNUC__
#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

/*
 * Disable copy construction and assignment.
 */
#define M_DISABLE_COPY(Class)				\
    Class(const Class &) ;						\
    Class &operator=(const Class &) ;

#define M_SINGLETON_DEFINED(Class)				\
public:										    \
	static Class *instance()					\
	{											\
		if (NULL == _ins) {						\
			_ins = new Class();					\
			atexit(desposed);					\
		}										\
		return _ins;							\
	}											\
protected:										\
    Class() { }									\
	static void desposed() {					\
		if (NULL != _ins) {						\
			delete _ins; _ins = NULL;			\
		}										\
	}											\
private:										\
    static Class *_ins;							\
public:											\
    virtual ~Class() { }

#define M_SINGLETON_IMPLEMENT(Class) \
	Class *Class::_ins = NULL;

#define SAFE_DELETE(x) do { if (x != NULL) { delete x; x = NULL; } } while(0);
#define ELEMENT(x) (sizeof(x) / sizeof((x)[0])

// from util-linux-2.20.1/hwclock/cmos.c
#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)
#define BIN_TO_BCD(val) ((val)=(((val)/10)<<4) + (val)%10)

#endif /* _MCOREDEF_H_ */
