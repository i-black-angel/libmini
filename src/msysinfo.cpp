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
#include <mpl/msysinfo.h>
#include <mpl/mlog.h>
#include <mpl/merror.h>
#include <mpl/mfile.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

std::string hostname()
{
#ifdef _MSC_VER
	WSADATA wsaData;
	int err = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (0 != err) {
		fprintf(stderr, "WSAStartup failure!\n");
		return "";
	}
#endif
	char buf[1024] = { 0x00 };
	if (gethostname(buf, sizeof(buf)) == -1) {
		log_error("gethostname: %s", error().c_str());
#ifdef _MSC_VER
		WSACleanup();
#endif
		return "";
	}
#ifdef _MSC_VER
	WSACleanup();
#endif
	return buf;		
}

#ifdef M_OS_LINUX
MSysinfo::MSysinfo()
{
	uname(&_utsname);
	sysinfo(&_sysinfo);
	statfs("/", &_statfs);
}

MSysinfo::~MSysinfo()
{
}

std::string MSysinfo::kernelName() const
{
	return _utsname.sysname;
}

std::string MSysinfo::nodeName() const
{
	return _utsname.nodename;
}

std::string MSysinfo::kernelRelease() const
{
	return _utsname.release;
}

std::string MSysinfo::kernelVersion() const
{
	return _utsname.version;
}

std::string MSysinfo::machine() const
{
	return _utsname.machine;
}

uint64_t MSysinfo::freeMem() const
{
	return _sysinfo.freeram * _sysinfo.mem_unit;
}

uint64_t MSysinfo::totalMem() const
{
	return _sysinfo.totalram * _sysinfo.mem_unit;
}

uint64_t MSysinfo::usedMem() const
{
	return (_sysinfo.totalram - _sysinfo.freeram) * _sysinfo.mem_unit;
}

uint64_t MSysinfo::sharedMem() const
{
	return _sysinfo.sharedram * _sysinfo.mem_unit;
}

uint64_t MSysinfo::bufferMem() const
{
	return _sysinfo.bufferram * _sysinfo.mem_unit;
}

uint64_t MSysinfo::totalSwap() const
{
	return _sysinfo.totalswap * _sysinfo.mem_unit;
}

uint64_t MSysinfo::freeSwap() const
{
	return _sysinfo.freeswap * _sysinfo.mem_unit;
}

uint64_t MSysinfo::usedSwap() const
{
	return (_sysinfo.totalswap - _sysinfo.freeswap) * _sysinfo.mem_unit;
}

static uint64_t mpl_meminfo(const char *name)
{
	uint64_t slot = 0;
	char buf[2048] = { 0x00 };
	char *head, *tail;
	MFile file = "/proc/meminfo";
	if (file.readbuf(buf, sizeof(buf)) <= 0)
		return 0;

	head = buf;
	for (; ;) {
		tail = strchr(head, ':');
		if (!tail) break;
		*tail = '\0';
		if (0 == strcmp(head, name)) { // goto next line
			head = tail + 1;
			slot = strtoull(head, &tail, 10);
			break;
		}
		head = tail + 1;
		tail = strchr(head, '\n');
		if (!tail) break;
		head = tail + 1;
	}
	
	return slot << 10;
}

uint64_t MSysinfo::cached() const
{
	return mpl_meminfo("Cached");
}

uint64_t MSysinfo::active() const
{
	return mpl_meminfo("Active");
}

uint64_t MSysinfo::inactive() const
{
	return mpl_meminfo("Inactive");
}

uint64_t MSysinfo::totalDisk() const
{
	return _statfs.f_blocks * _statfs.f_bsize;
}

uint64_t MSysinfo::freeDisk() const
{
	return _statfs.f_bfree * _statfs.f_bsize;
}

uint64_t MSysinfo::usedDisk() const
{
	return (_statfs.f_blocks - _statfs.f_bfree) * _statfs.f_bsize;
}

uint64_t MSysinfo::availDisk() const
{
	return _statfs.f_bavail * _statfs.f_bsize;
}

uint16_t MSysinfo::procs() const
{
	return _sysinfo.procs;
}

long MSysinfo::uptime() const
{
	return _sysinfo.uptime;
}

std::string MSysinfo::struptime(long uptime_secs) const
{
	int upminutes, uphours, updays, upweeks, upyears, updecades;
	int pos;
	int comma;
	char buf[1024] = { 0x00 };

	/* read and calculate the amount of uptime */
	updecades = uptime_secs / (60 * 60 * 24 * 365 * 10);
	upyears = (uptime_secs / (60 * 60 * 24 * 365)) % 10;
	upweeks = (uptime_secs / (60 * 60 * 24 * 7)) % 52;
	updays = (uptime_secs / (60 * 60 * 24)) % 7;

	strcat(buf, "up ");
	pos += 3;

	upminutes = uptime_secs / 60;
	uphours = upminutes / 60;
	uphours = uphours % 24;
	upminutes = upminutes % 60;

	comma = 0;

	if (updecades) {
		pos += sprintf(buf + pos, "%d %s", updecades,
					   updecades > 1 ? "decades" : "decade");
		comma += 1;
	}

	if (upyears) {
		pos += sprintf(buf + pos, "%s%d %s", comma > 0 ? ", " : "", upyears,
					   upyears > 1 ? "years" : "year");
		comma += 1;
	}

	if (upweeks) {
		pos += sprintf(buf + pos, "%s%d %s", comma > 0 ? ", " : "", upweeks,
					   upweeks > 1 ? "weeks" : "week");
		comma += 1;
	}

	if (updays) {
		pos += sprintf(buf + pos, "%s%d %s", comma > 0 ? ", " : "", updays,
					   updays > 1 ? "days" : "day");
		comma += 1;
	}

	if (uphours) {
		pos += sprintf(buf + pos, "%s%d %s", comma > 0 ? ", " : "", uphours,
					   uphours > 1 ? "hours" : "hour");
		comma += 1;
	}

	if (upminutes) {
		pos += sprintf(buf + pos, "%s%d %s", comma > 0 ? ", " : "", upminutes,
					   upminutes > 1 ? "minutes" : "minute");
		comma += 1;
	}

	return buf;
}

std::string MSysinfo::since() const
{
	double now, uptimeSecs;
	time_t upSinceSecs;
	struct tm *upSince;
	struct timeval tim;
	char buf[64] = { 0x00 };

	// Get the current time and convert it to a double
	gettimeofday(&tim, NULL);
	now = tim.tv_sec + (tim.tv_usec / 1000000.0);

	// Get the uptime and calculate when that was
	uptimeSecs = uptime();
	upSinceSecs = (time_t)((now - uptimeSecs) + 0.5);

	// Show this
	upSince = localtime(&upSinceSecs);
	snprintf(buf, sizeof(buf) - 1,
			 "%04d-%02d-%02d %02d:%02d:%02d",
			 upSince->tm_year + 1900, upSince->tm_mon + 1, upSince->tm_mday,
			 upSince->tm_hour, upSince->tm_min, upSince->tm_sec);
	return std::string(buf);
}
#endif

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
