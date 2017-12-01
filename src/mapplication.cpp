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
#include <minion/mapplication.h>
#include <minion/merror.h>

MINION_BEGIN_NAMESPACE

std::string applicationName()
{
	return MApplication::applicationName();
}

std::string applicationDirPath()
{
	return MApplication::applicationDirPath();
}

std::string applicationFilePath()
{
	return MApplication::applicationFilePath();
}

int64_t pid()
{
	return MApplication::pid();
}

MApplication::MApplication(int argc, char *argv[])
{
}

MApplication::~MApplication()
{
}

std::string MApplication::applicationDirPath()
{
	std::string ret;

	char path[PATH_MAX] = { 0x00 };
	if (readlink("/proc/self/exe", path, sizeof(path)) == -1) {
		std::cerr << error() << std::endl;
		return ret;
	}

	std::string file = path;
	size_t idx = file.find_last_of("/");
	if (idx == std::string::npos) { return std::string("."); }
	if (idx == 0) idx = 1;

    return file.substr(0, idx);
}

std::string MApplication::applicationFilePath()
{
	std::string ret;

	char path[PATH_MAX] = { 0x00 };
	if (readlink("/proc/self/exe", path, sizeof(path)) == -1) {
		std::cerr << error() << std::endl;
		return ret;
	}

	return path;
}

std::string MApplication::applicationName()
{
	std::string ret;

	char path[PATH_MAX] = { 0x00 };
	if (readlink("/proc/self/exe", path, sizeof(path)) == -1) {
		std::cerr << error() << std::endl;
		return ret;
	}

	std::string file = path;
	size_t idx = file.find_last_of("/");
	if (idx == std::string::npos) ret = file;
	ret = file.substr(idx + 1);

	return ret;
}

std::string MApplication::applicationVersion()
{
}

int64_t MApplication::pid()
{
    return getpid();
}

uint32_t MApplication::uptime()
{
}

void MApplication::setApplicationName(const std::string &application)
{
	_applicationName = application;
}

void MApplication::setApplicationVersion(const std::string &version)
{
	_applicationVersion = version;
}

int __lockfile(int fd)
{
	struct flock lock;
	lock.l_type = F_WRLCK;
    lock.l_start = 0;  
    lock.l_whence = SEEK_SET;  
    lock.l_len = 0;  
    return (fcntl(fd, F_SETLK, &lock));
}

bool MApplication::alreadyRunning(const std::string &lockfile)
{
	static const int lockmode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	static const int openflag = (O_CREAT | O_RDWR);

	int fd = -1;
	char buf[16] = { 0x00 };

	fd = open(lockfile.c_str(), openflag, lockmode);
	if (fd == -1) {
		log_error("open %s failed: %s", lockfile.c_str(), error().c_str());
		exit(EXIT_FAILURE);
	}

	if (__lockfile(fd) == -1) {
		if (EACCES == errno || EAGAIN == errno) {
			close(fd);
			return true;
		}
		log_error("can't lock %s: %s", lockfile.c_str(), error().c_str());
		exit(EXIT_FAILURE);
	}
	ftruncate(fd, 0);
	sprintf(buf, "%lld", pid());
	write(fd, buf, strlen(buf) + 1);
	return false;
}

MINION_END_NAMESPACE
