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
#ifndef _MPROCESS_H_
#define _MPROCESS_H_

#include <mpl/mcoredef.h>

MPL_BEGIN_NAMESPACE

namespace process {

	// Get the process ID of the calling process.
	pid_t pid();
	// Get the process ID of the calling process's parent.
	pid_t ppid();
	// Get the process group ID of the calling process.
	pid_t pgrp();
	
	uid_t uid();
	uid_t euid();
	gid_t gid();
	gid_t egid();
	std::string pwd();
	std::string user();
	std::string group();
	std::string login();
	std::string program();
	int execute(const std::string &program, const std::vector<std::string> &arguments);
	int execute(const std::string &command);
	std::vector<std::string> systemEnvironment();
	bool alreadyRunning(const std::string &lockfile);

}

MPL_END_NAMESPACE

#endif /* _MPROCESS_H_ */
