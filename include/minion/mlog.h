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

#include <minion/mcoredef.h>

MINION_BEGIN_NAMESPACE

class MLog
{
	M_SINGLETON_DEFINED(MLog)
public:
	enum Category {
		kEmerg = 0,
		kAlert,
		kCrit,
		kError,
		kWarn,
		kNotice,
		kInfo,
		kDebug
	};

	void log(const std::string &file, const std::string &func,
			  uint32_t line, int level,
			  const char *__format, ...) const;
	std::string strlog(int level) const;
};

MINION_END_NAMESPACE

#define log_init(level)
#define log_emerg(__format, args...) minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kEmerg, __format, ##args)
#define log_alert(__format, args...) minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kAlert, __format, ##args)
#define log_crit(__format, args...) minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kCrit, __format, ##args)
#define log_error(__format, args...) minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kError, __format, ##args)
#define log_warn(__format, args...) minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kWarn, __format, ##args)
#define log_notice(__format, args...) minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kNotice, __format, ##args)
#define log_info(__format, args...) minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kInfo, __format, ##args)
#define log_debug(__format, args...) minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kDebug, __format, ##args)

#endif /* _MLOG_H_ */
