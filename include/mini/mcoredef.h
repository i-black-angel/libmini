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
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <pthread.h>
# include <getopt.h>
# include <syslog.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/file.h>
# include <fcntl.h>
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
# define    mkdir           _mkdir
# define    rmdir           _rmdir
# define    snprintf        _snprintf
# define    useconds_t      DWORD
# define    sleep(n)        Sleep((n) * 1000)
# define    usleep(n)       Sleep((n) / 1000)
# define    close           _close
# define    DIRECTORY_SEPARATOR   "\\"
#else  /* non-windows */
# ifndef __socket_t_defined
#  define __socket_t_defined
typedef int socket_t;
# endif
# define    closesocket     close
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

# define MINI_BEGIN_NAMESPACE namespace mini {
# define MINI_END_NAMESPACE }

#else /* __cplusplus */

# define MINI_BEGIN_NAMESPACE 
# define MINI_END_NAMESPACE

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

#endif /* _MCOREDEF_H_ */