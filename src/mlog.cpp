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
#include <mpl/mlog.h>
#include <mpl/msysinfo.h>
#include <mpl/mdatetime.h>
#include <mpl/mapplication.h>
#include <mpl/mprocess.h>
#include <mpl/mstring.h>
#include <mpl/merror.h>
#include <mpl/mfile.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

MLog *MLog::_ins = NULL;

MLog *MLog::instance()
{
	if (NULL == _ins) {
		_ins = new MLog();
		atexit(desposed);
	}
	return _ins;
}

void MLog::desposed()
{
	if (NULL != _ins) {
		delete _ins; _ins = NULL;
	}
}

MLog::MLog()
	: _priority(kDebug)
	, _performance(kNormal)
	, _pattern(kSyslog)
{
}

MLog::~MLog()
{
}

void MLog::init(const char *logfile, int priority, int performance)
{
	if (logfile != NULL) {
		_logfile = logfile;
		_pattern = kFile;
	}
	_priority = priority;
	_performance = performance;
}

void MLog::initdir(const char *logdir,
				   const char *prefix,
				   int priority,
				   int performance)
{
	if (logdir != NULL) {
		_logdir = logdir;
		_pattern = kDir;
	}
	if (prefix != NULL)
		_prefix = prefix;

	_priority = priority;
	_performance = performance;
}

void MLog::log(const std::string &file, const std::string &func,
			   uint32_t line, int pri,
			   const char *__format, ...)
{
	MScopedLock locker(_mutex);
	
	// according to pri
	if (pri > _priority) return;
	
	// combine log head
	std::string head;

	int len;
	std::string buffer;
	va_list vargs;
	va_start(vargs, __format);
	len = vsnprintf(NULL, 0, __format, vargs);
	buffer.resize(len);
	va_start(vargs, __format);
	vsnprintf(&buffer[0], len + 1, __format, vargs);
	va_end(vargs);

	// DATETIME HOSTNAME APPLICATIONNAME[PID] FILE FUNC[LINE] 
#if defined(_MSC_VER) || defined(M_OS_WIN)
	const char *fmt = "%s %s %s[%ld] %s %s[%u]: <%s> %s\n";
#else
	const char *fmt = "%s %s %s[%lld] %s %s[%u]: <%s> %s\n";
#endif
	std::string logstr = format(fmt,
								now().c_str(), hostname().c_str(),
								applicationName().c_str(), process::pid(),
								file.c_str(), func.c_str(), line,
								strpriority(pri).c_str(),
								buffer.c_str());

	std::cout << logstr;

	switch (_pattern) {
	case kFile:
		logToFile(logstr);
		break;
	case kDir:
		logToDir(logstr);
		break;
	default:
		logToSyslog(logstr);
		break;
	}
}

std::string MLog::strpriority(int pri) const
{
	std::string ret = "debug";
	switch (pri) {
	case kEmerg:
		ret = "emerg"; break;
	case kAlert:
		ret = "alert"; break;
	case kCrit:
		ret = "crit"; break;
	case kError:
		ret = "error"; break;
	case kWarn:
		ret = "warn"; break;
	case kNotice:
		ret = "notice"; break;
	case kInfo:
		ret = "info"; break;
	default:
		break;
	}
	return ret;
}

void MLog::logToFile(const std::string &logstr)
{
	file::appendbuf(_logfile.c_str(), logstr.c_str(), logstr.size());
}

void MLog::logToDir(const std::string &logstr)
{
	std::string logfile = _logdir + DIRECTORY_SEPARATOR
		+ _prefix + MDateTime::currentDateTime().toString("%Y%m%d")
		+ std::string(".log");
	file::appendbuf(logfile.c_str(), logstr.c_str(), logstr.size());
}

void MLog::logToSyslog(const std::string &logstr)
{
#ifdef M_OS_LINUX
		syslog(LOG_DEBUG, "%s", logstr.c_str());
#else
		OutputDebugString(logstr.c_str());
#endif		
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
