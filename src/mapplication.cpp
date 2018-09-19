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
#include <mpl/mapplication.h>
#include <mpl/merror.h>
#include <mpl/mlog.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

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

MApplication::MApplication(int argc, char *argv[])
{
}

MApplication::~MApplication()
{
}

std::string MApplication::applicationFilePath()
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	char path[MAX_PATH] = { 0x00 };
	GetModuleFileName(0, path, MAX_PATH);

	return path;
#else
	char path[PATH_MAX] = { 0x00 };
	if (readlink("/proc/self/exe", path, sizeof(path)) == -1) {
		log_error("%s", error().c_str());
		return "";
	}

	return path;
#endif
}

std::string MApplication::applicationDirPath()
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	char path[MAX_PATH] = { 0x00 };
	GetModuleFileName(0, path, MAX_PATH);

	*strrchr(path, '\\') = 0;
	return path;
#else
	char path[PATH_MAX] = { 0x00 };
	if (readlink("/proc/self/exe", path, sizeof(path)) == -1) {
		log_error("%s", error().c_str());
		return "";
	}

	*strrchr(path, '/') = 0;
	return path;
#endif
}

std::string MApplication::applicationName()
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	char path[MAX_PATH] = { 0x00 };
	GetModuleFileName(0, path, MAX_PATH);

	char *ptr = strrchr(path, '\\') + 1;
	return ptr;
#else
	char path[PATH_MAX] = { 0x00 };
	if (readlink("/proc/self/exe", path, sizeof(path)) == -1) {
		log_error("%s", error().c_str());
		return "";
	}

	char *ptr = strrchr(path, '/') + 1;
	return ptr;
#endif
}

std::string MApplication::applicationVersion()
{
	return "";
}

uint32_t MApplication::uptime()
{
	return 0;
}

void MApplication::setApplicationName(const std::string &application)
{
	_applicationName = application;
}

void MApplication::setApplicationVersion(const std::string &version)
{
	_applicationVersion = version;
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
