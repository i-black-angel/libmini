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
#include <mpl/mlockfile.h>
#include <mpl/mlog.h>
#include <mpl/merror.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

pid_t process::pid()
{
	return getpid();
}

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

std::string process::user()
{
	struct passwd *pwd = getpwuid(getuid());
	if (pwd == NULL) return std::string();
	return std::string(pwd->pw_name);
}

std::string process::group()
{
	struct group *grp = getgrgid(getgid());
	if (grp == NULL) return std::string();
	return std::string(grp->gr_name);
}

std::string process::login()
{
	return getlogin();
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
}

int process::execute(const std::string &command)
{
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

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
