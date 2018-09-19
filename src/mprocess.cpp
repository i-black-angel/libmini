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
#include <mpl/mprocess.h>
#include <mpl/mapplication.h>
#include <mpl/mlog.h>
#include <mpl/merror.h>
#ifdef M_OS_LINUX
#include <mpl/mlockfile.h>
#endif /* M_OS_LINUX */

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

pid_t process::pid()
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return GetCurrentProcessId();
#else
	return getpid();
#endif
}

#ifdef M_OS_LINUX
pid_t process::ppid()
{
	return getppid();
}

pid_t process::pgrp()
{
	return getpgrp();
}

uid_t process::uid()
{
	return getuid();
}

uid_t process::euid()
{
	return geteuid();
}

gid_t process::gid()
{
	return getgid();
}

gid_t process::egid()
{
	return getegid();
}
#endif /* M_OS_LINUX */

std::string process::user()
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	char userName[256] = { 0x00 };
	DWORD nameSize = sizeof(userName);
	GetUserName((LPSTR)userName, &nameSize);
	return userName;
#else
	struct passwd *pwd = getpwuid(getuid());
	if (pwd == NULL) return std::string();
	return std::string(pwd->pw_name);
#endif
}

std::string process::group()
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
#else
	struct group *grp = getgrgid(getgid());
	if (grp == NULL) return std::string();
	return std::string(grp->gr_name);
#endif
}

std::string process::login()
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return user();
#else
	return getlogin();
#endif
}

std::string process::program()
{
	return applicationName();
}

std::string process::pwd()
{
	char buf[1024] = {0x00};
#ifdef M_OS_WIN
	DWORD res = ::GetCurrentDirectory(sizeof(buf) - 1, buf);
#else
	char *cwd = getcwd(buf, sizeof(buf) - 1);
#endif
	return buf;
}

int process::execute(const std::string &program, const std::vector<std::string> &arguments)
{
	return 0;
}

int process::execute(const std::string &command)
{
	return 0;
}

std::vector<std::string> process::systemEnvironment()
{
	std::vector<std::string> out;
	for (int i = 0; environ[i] != NULL; ++i) {
		std::string val = environ[i];
		out.push_back(val);
	}
	return out;
}

#ifdef M_OS_LINUX
bool process::alreadyRunning(const std::string &lockfile)
{
	MLockFile l = lockfile;
	int res = l.lock();
	if (res == -1) {
		log_error("lock '%s' failed: %s", lockfile.c_str(), error().c_str());
		exit(EXIT_FAILURE);
	}
	return (res == 1);
}
#endif

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
