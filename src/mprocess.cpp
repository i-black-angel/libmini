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
#include <minion/mprocess.h>

MINION_BEGIN_NAMESPACE

std::string workingDirectory()
{
	MProcess proc;
	return proc.workingDirectory();
}

MProcess::MProcess()
{
}

MProcess::~MProcess()
{
}

int64_t MProcess::pid() const
{
	return getpid();
}

std::string MProcess::program() const
{
}

std::string MProcess::workingDirectory() const
{
	char buf[1024] = {0x00};
#ifdef M_OS_WIN
	DWORD res = ::GetCurrentDirectory(sizeof(buf) - 1, buf);
#else
	char *cwd = getcwd(buf, sizeof(buf) - 1);
#endif
	return buf;
}

int MProcess::execute(const std::string &program, const std::vector<std::string> &arguments)
{
}

int MProcess::execute(const std::string &command)
{
}

std::vector<std::string> MProcess::systemEnvironment()
{
}

MINION_END_NAMESPACE
