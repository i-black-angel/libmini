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
#include <mpl/merror.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

int errnum()
{
#ifdef M_OS_WIN
	return GetLastError();
#else	
	return errno;
#endif /* M_OS_WIN */
}

std::string error(int code)
{
#ifdef M_OS_WIN
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM|
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, code, 0/*Default language*/,
		(LPTSTR) &lpMsgBuf, 0, NULL);
	if (NULL != lpMsgBuf) {
		std::string errstr = (const char *)lpMsgBuf;
		LocalFree(lpMsgBuf);
		return errstr;
	}
	return std::string();
#else
	return strerror(code);
#endif /* M_OS_WIN */
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
