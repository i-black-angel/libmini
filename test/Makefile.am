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

hostaddress_CPPFLAGS = -I$(top_srcdir)/include
hostaddress_LDADD = ../src/libminion.la
hostaddress_SOURCES = hostaddress.cpp

welcome_CPPFLAGS = -I$(top_srcdir)/include
welcome_LDADD = ../src/libminion.la
welcome_SOURCES = welcome.cpp

assert_CPPFLAGS = -I$(top_srcdir)/include
assert_LDADD = ../src/libminion.la
assert_SOURCES = assert.cpp

mutex_CPPFLAGS = -I$(top_srcdir)/include
mutex_LDADD = ../src/libminion.la -lpthread
mutex_SOURCES = mutex.cpp

condition_CPPFLAGS = -I$(top_srcdir)/include
condition_LDADD = ../src/libminion.la -lpthread
condition_SOURCES = condition.cpp

udpsocket_CPPFLAGS = -I$(top_srcdir)/include
udpsocket_LDADD = ../src/libminion.la -lpthread
udpsocket_SOURCES = udpsocket.cpp

thread_CPPFLAGS = -I$(top_srcdir)/include
thread_LDADD = ../src/libminion.la -lpthread
thread_SOURCES = thread.cpp

udpclient_CPPFLAGS = -I$(top_srcdir)/include
udpclient_LDADD = ../src/libminion.la
udpclient_SOURCES = udpclient.cpp

error_CPPFLAGS = -I$(top_srcdir)/include
error_LDADD = ../src/libminion.la
error_SOURCES = error.cpp

uuid_CPPFLAGS = -I$(top_srcdir)/include
uuid_LDADD = ../src/libminion.la
uuid_SOURCES = uuid.cpp

fileinfo_CPPFLAGS = -I$(top_srcdir)/include
fileinfo_LDADD = ../src/libminion.la
fileinfo_SOURCES = fileinfo.cpp

md5sum_CPPFLAGS = -I$(top_srcdir)/include
md5sum_LDADD = ../src/libminion.la
md5sum_SOURCES = md5sum.cpp

datetime_CPPFLAGS = -I$(top_srcdir)/include
datetime_LDADD = ../src/libminion.la
datetime_SOURCES = datetime.cpp
