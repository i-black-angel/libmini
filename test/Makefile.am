# Process this file with automake to produce Makefile.in (in this,
# and all subdirectories).
#
# Copyright 2017 Shusheng Shao <iblackangel@163.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

noinst_PROGRAMS = hostaddress welcome assert mutex condition udpsocket
noinst_PROGRAMS += thread udpclient error uuid fileinfo md5sum datetime
noinst_PROGRAMS += application tcpclient tcpserver datahandler threadpool
noinst_PROGRAMS += settings options dirt tstring file process
noinst_PROGRAMS += signal sysinfo

hostaddress_CPPFLAGS = -I$(top_srcdir)/include
hostaddress_LDADD = ../src/libmpl.la
hostaddress_SOURCES = hostaddress.cpp

welcome_CPPFLAGS = -I$(top_srcdir)/include
welcome_LDADD = ../src/libmpl.la
welcome_SOURCES = welcome.cpp

assert_CPPFLAGS = -I$(top_srcdir)/include
assert_LDADD = ../src/libmpl.la
assert_SOURCES = assert.cpp

mutex_CPPFLAGS = -I$(top_srcdir)/include
mutex_LDADD = ../src/libmpl.la -lpthread
mutex_SOURCES = mutex.cpp

condition_CPPFLAGS = -I$(top_srcdir)/include
condition_LDADD = ../src/libmpl.la -lpthread
condition_SOURCES = condition.cpp

udpsocket_CPPFLAGS = -I$(top_srcdir)/include
udpsocket_LDADD = ../src/libmpl.la -lpthread
udpsocket_SOURCES = udpsocket.cpp

thread_CPPFLAGS = -I$(top_srcdir)/include
thread_LDADD = ../src/libmpl.la -lpthread
thread_SOURCES = thread.cpp

udpclient_CPPFLAGS = -I$(top_srcdir)/include
udpclient_LDADD = ../src/libmpl.la
udpclient_SOURCES = udpclient.cpp

error_CPPFLAGS = -I$(top_srcdir)/include
error_LDADD = ../src/libmpl.la
error_SOURCES = error.cpp

uuid_CPPFLAGS = -I$(top_srcdir)/include
uuid_LDADD = ../src/libmpl.la
uuid_SOURCES = uuid.cpp

fileinfo_CPPFLAGS = -I$(top_srcdir)/include
fileinfo_LDADD = ../src/libmpl.la
fileinfo_SOURCES = fileinfo.cpp

md5sum_CPPFLAGS = -I$(top_srcdir)/include
md5sum_LDADD = ../src/libmpl.la
md5sum_SOURCES = md5sum.cpp

datetime_CPPFLAGS = -I$(top_srcdir)/include
datetime_LDADD = ../src/libmpl.la
datetime_SOURCES = datetime.cpp

application_CPPFLAGS = -I$(top_srcdir)/include
application_LDADD = ../src/libmpl.la
application_SOURCES = application.cpp

tcpclient_CPPFLAGS = -I$(top_srcdir)/include
tcpclient_LDADD = ../src/libmpl.la
tcpclient_SOURCES = tcpclient.cpp

tcpserver_CPPFLAGS = -I$(top_srcdir)/include
tcpserver_LDADD = ../src/libmpl.la
tcpserver_SOURCES = tcpserver.cpp

datahandler_CPPFLAGS = -I$(top_srcdir)/include
datahandler_LDADD = ../src/libmpl.la
datahandler_SOURCES = datahandler.cpp

threadpool_CPPFLAGS = -I$(top_srcdir)/include
threadpool_LDADD = ../src/libmpl.la
threadpool_SOURCES = threadpool.cpp

settings_CPPFLAGS = -I$(top_srcdir)/include
settings_LDADD = ../src/libmpl.la
settings_SOURCES = settings.cpp

options_CPPFLAGS = -I$(top_srcdir)/include
options_LDADD = ../src/libmpl.la
options_SOURCES = options.cpp

dirt_CPPFLAGS = -I$(top_srcdir)/include
dirt_LDADD = ../src/libmpl.la
dirt_SOURCES = dir.cpp

tstring_CPPFLAGS = -I$(top_srcdir)/include
tstring_LDADD = ../src/libmpl.la
tstring_SOURCES = tstring.cpp

file_CPPFLAGS = -I$(top_srcdir)/include
file_LDADD = ../src/libmpl.la
file_SOURCES = file.cpp

process_CPPFLAGS = -I$(top_srcdir)/include
process_LDADD = ../src/libmpl.la
process_SOURCES = process.cpp

signal_CPPFLAGS = -I$(top_srcdir)/include
signal_LDADD = ../src/libmpl.la
signal_SOURCES = signal.cpp

sysinfo_CPPFLAGS = -I$(top_srcdir)/include
sysinfo_LDADD = ../src/libmpl.la
sysinfo_SOURCES = sysinfo.cpp
