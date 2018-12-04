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

#include <mpl/mcoredef.h>
#include <mpl/mmutex.h>

MPL_BEGIN_NAMESPACE

class MLog
{
public:
	static MLog *instance();
	virtual ~MLog();
public:
	enum Priority {
		kEmerg = 0,
		kAlert,
		kCrit,
		kError,
		kWarn,
		kNotice,
		kInfo,
		kDebug
	};

	enum Performance {
		kNormal = 0,
		kHigh
	};

	void init(const char *logfile = NULL,
			  int priority = mpl::MLog::kDebug,
			  int performance = mpl::MLog::kNormal);
	void initdir(const char *logdir,
				 const char *prefix = NULL,
				 int priority = mpl::MLog::kDebug,
				 int performance = mpl::MLog::kNormal);
	void log(const std::string &srcfile, const std::string &srcfunc,
			  uint32_t srcline, int priority,
			  const char *__format, ...);
	std::string strpriority(int pri) const;
protected:
	static void desposed();
	MLog();
	MLog(const MLog &rhs);
	MLog &operator=(const MLog &rhs);

	enum Pattern {
		kSyslog = 0,		/**< just log to standard output */
		kFile = 1,			/**< log to single file */
		kDir = 2			/**< log to directory, with multi files */
	};
	void logToFile(const std::string &logstr);
	void logToDir(const std::string &logstr);
	void logToSyslog(const std::string &logstr);
private:
	std::string _logfile;
	std::string _logdir;
	std::string _prefix;
	int _priority;
	int _performance;
	Pattern _pattern;
	
	MMutex _mutex;
	
	static MLog *_ins;
};

MPL_END_NAMESPACE

#define log_init                  mpl::MLog::instance()->init
#define log_initdir               mpl::MLog::instance()->initdir
#define log_emerg(format, ...)  mpl::MLog::instance()->log(__FILE__, __FUNCTION__, __LINE__, mpl::MLog::kEmerg, format, ##__VA_ARGS__)
#define log_alert(format, ...)  mpl::MLog::instance()->log(__FILE__, __FUNCTION__, __LINE__, mpl::MLog::kAlert, format, ##__VA_ARGS__)
#define log_crit(format, ...)   mpl::MLog::instance()->log(__FILE__, __FUNCTION__, __LINE__, mpl::MLog::kCrit, format, ##__VA_ARGS__)
#define log_error(format, ...)  mpl::MLog::instance()->log(__FILE__, __FUNCTION__, __LINE__, mpl::MLog::kError, format, ##__VA_ARGS__)
#define log_warn(format, ...)   mpl::MLog::instance()->log(__FILE__, __FUNCTION__, __LINE__, mpl::MLog::kWarn, format, ##__VA_ARGS__)
#define log_notice(format, ...) mpl::MLog::instance()->log(__FILE__, __FUNCTION__, __LINE__, mpl::MLog::kNotice, format, ##__VA_ARGS__)
#define log_info(format, ...)   mpl::MLog::instance()->log(__FILE__, __FUNCTION__, __LINE__, mpl::MLog::kInfo, format, ##__VA_ARGS__)
#define log_debug(format, ...)  mpl::MLog::instance()->log(__FILE__, __FUNCTION__, __LINE__, mpl::MLog::kDebug, format, ##__VA_ARGS__)

#endif /* _MLOG_H_ */
