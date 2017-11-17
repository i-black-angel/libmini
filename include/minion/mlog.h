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
#ifndef _MLOG_H_
#define _MLOG_H_

#include <mini/mcoredef.h>

MINI_BEGIN_NAMESPACE

class MLog
{
public:
    explicit MLog();
    virtual ~MLog();
};

MINI_END_NAMESPACE

#define log_init(level)
#define log_emerg(format, args...)
#define log_alert(format, args...)
#define log_crit(format, args...)
#define log_error(format, args...)
#define log_warn(format, args...)
#define log_notice(format, args...)
#define log_info(format, args...)
#define log_debug(format, args...)

#endif /* _MLOG_H_ */
