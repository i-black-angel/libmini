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
#include <mpl.h>
#include "servicemanager.h"

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

ServiceManager::ServiceManager()
{
}

ServiceManager::~ServiceManager()
{
}

int ServiceManager::parseArgs(int argc, char *argv[])
{
	mpl::MOptions opt(VERSION, "Framework program description");
	opt.insert('n', 'name', "title for initial program", true, "NAME");
	opt.parse(argc, argv);

	if (!opt.find('v')) {
		if (0 != daemon(0, 0)) {
			fprintf(stderr, "daemon failed: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
	}

	if (mpl::process::alreadyRunning(_PATH_PID_FILE)) {
		log_error("another daemon is already running");
		return EXIT_FAILURE;
	}

#ifdef M_OS_WIN
	std::string path = mpl::applicationDirPath() + "\\" + mpl::applicationName() + ".conf";
#else
	std::string path = _PATH_CONF_FILE;	
#endif /* M_OS_WIN */
	if (opt.find('c')) {
		path = opt.getstr('c');
	}
	Configuration::instance()->load(path);
}

int ServiceManager::exec(int argc, char *argv[])
{
	// step:
	// 1. parse program options
	// 2. signal block
	if (parseArgs(argc, argv) != 0)
		return EXIT_FAILURE;

	return 0;
}

#ifdef _MSC_VER
# pragma warning (pop)
#endif
