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
#include <minion/mlog.h>

MINION_BEGIN_NAMESPACE

M_SINGLETON_IMPLEMENT(MLog)

void MLog::log(const std::string &file, const std::string &func,
			   uint32_t line, int level,
			   const char *format, ...)
{
	int len;
	std::string buffer;

	// combine log head
	std::string head;
	
	va_list vargs;
	va_start(vargs, format);
	len = vsnprintf(NULL, 0, format, vargs);
	std::cout << len << std::endl;
	buffer.resize(len + 2);
	vsnprintf(&buffer[0], len + 1, format, vargs);
	va_end(vargs);

	std::cout << file << "[" << line << "]" << ": " << buffer << std::endl;

	va_list vargs;
	char buffer[1024] = {0};
	va_start(vargs, format);
	vsnprintf(buffer, sizeof(buffer) - 1, format, vargs);
	va_end(vargs);
}

MINION_END_NAMESPACE
