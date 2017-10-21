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
#include <punica/pglobal.h>

PUNICA_BEGIN_NAMESPACE

const char *welcome()
{
	return "Hello, welcome to libpunica.";
}

void abort(const char *errmsg)
{
#if defined P_OS_WIN

    //  Raise STATUS_FATAL_APP_EXIT.
    ULONG_PTR extra_info [1];
    extra_info [0] = (ULONG_PTR) errmsg;
    RaiseException (0x40000015, EXCEPTION_NONCONTINUABLE, 1, extra_info);
#else
    (void)errmsg;
    ::abort ();
#endif
}

PUNICA_END_NAMESPACE
