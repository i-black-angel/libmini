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

#ifdef M_OS_LINUX
/* GNU's uptime.
   Copyright (C) 1992-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
#define BAD_OPEN_MESSAGE											\
	"Error: /proc must be mounted\n"								\
	"  To mount /proc at boot you need an /etc/fstab line like:\n"	\
	"      proc   /proc   proc    defaults\n"						\
	"  In the meantime, run \"mount proc /proc -t proc\"\n"

#define UPTIME_FILE  "/proc/uptime"
static int uptime_fd = -1;
#define MEMINFO_FILE "/proc/meminfo"
static int meminfo_fd = -1;

/* This macro opens filename only if necessary and seeks to 0 so
 * that successive calls to the functions are more efficient.
 * It also reads the current contents of the file into the global buf.
 */
#define FILE_TO_BUF(filename, fd, buf) do{							\
		static int local_n;											\
		if (fd == -1 && (fd = open(filename, O_RDONLY)) == -1) {	\
			fputs(BAD_OPEN_MESSAGE, stderr);						\
			fflush(NULL);											\
			_exit(102);												\
		}															\
		lseek(fd, 0L, SEEK_SET);									\
		if ((local_n = read(fd, buf, sizeof buf - 1)) < 0) {		\
			perror(filename);										\
			fflush(NULL);											\
			_exit(103);												\
		}															\
		buf[local_n] = '\0';										\
	}while(0)

/* evals 'x' twice */
#define SET_IF_DESIRED(x,y) do{  if(x) *(x) = (y); }while(0)

/***********************************************************************/
int uptime(double * uptime_secs, double * idle_secs) {
	// As of 2.6.24 /proc/meminfo seems to need 888 on 64-bit,
	// and would need 1258 if the obsolete fields were there.
	char buf[2048] = {0x00};
	double up=0, idle=0;
	char *savelocale;

	FILE_TO_BUF(UPTIME_FILE,uptime_fd, buf);
	savelocale = strdup(setlocale(LC_NUMERIC, NULL));
	setlocale(LC_NUMERIC,"C");
	if (sscanf(buf, "%lf %lf", &up, &idle) < 2) {
		setlocale(LC_NUMERIC,savelocale);
		free(savelocale);
		fputs("bad data in " UPTIME_FILE "\n", stderr);
		return 0;
	}
	setlocale(LC_NUMERIC,savelocale);
	free(savelocale);
	SET_IF_DESIRED(uptime_secs, up);
	SET_IF_DESIRED(idle_secs, idle);
	return up;	/* assume never be zero seconds in practice */
}
#endif

MPL_BEGIN_NAMESPACE

std::string hostname()
{
	char buf[1024] = {0x00};
	if (gethostname(buf, sizeof(buf)) == -1) {
		log_error("gethostname: %s", error().c_str());
		return std::string();
	}
	return buf;
}
	
std::string buildCpuArchitecture()
{
}

std::string currentCpuArchitecture()
{
}

std::string buildAbi()
{
}

std::string kernelType()
{
}

std::string kernelVersion()
{
}

std::string productType()
{
}

std::string productVersion()
{
}

std::string prettyProductName()
{
}

uint64_t memoryAvailSize()
{
}

uint64_t memoryTotalSize()
{
}

uint64_t memoryUsedSize()
{
}

double memoryPercent()
{
}

double cpuPercent()
{
}

uint32_t uptimelong()
{
}

std::string uptime()
{
}

std::string since()
{
}

MPL_END_NAMESPACE
