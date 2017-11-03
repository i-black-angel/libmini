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
#ifndef _PGLOBAL_H_
#define _PGLOBAL_H_

#include <punica/pcoredef.h>

PUNICA_BEGIN_NAMESPACE

const char *welcome();
void abort(const char *errmsg);
std::string uuidgen();

PUNICA_END_NAMESPACE

#ifdef P_OS_WIN

void gettimeofday (struct timeval *tv, void *dummy);

#endif /* P_OS_WIN */

// will be re-write in some whare
#ifndef P_OS_WIN

#define posix_assert(x)													\
    do {																\
        if (unlikely (x)) {												\
            const char *errstr = strerror (x);							\
            fprintf (stderr, "%s (%s:%d)\n", errstr, __FILE__, __LINE__); \
            punica::abort (errstr);										\
        }																\
    } while (false)

#endif /* P_OS_WIN */

#endif /* _PGLOBAL_H_ */
