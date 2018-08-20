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
#include <mpl/mlockfile.h>
#include <mpl/mlog.h>
#include <mpl/merror.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE
MLockFile::MLockFile()
{
}

MLockFile::MLockFile(const char *s)
	: _fname(s),
	  _fd(-1)
{
}

MLockFile::MLockFile(const std::string &str)
	: _fname(str),
	  _fd(-1)
{
}

MLockFile::MLockFile(const MLockFile &other)
{
	inner_copy(other);
}

MLockFile::~MLockFile()
{
}

static int mpl_lockfile(int fd)
{
	struct flock lock;
	lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;  
    lock.l_len = 0;  
    return (fcntl(fd, F_SETLK, &lock));
}

int MLockFile::lock()
{
	static const int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	static const int flag = O_CREAT | O_RDWR;

	char buf[16] = { 0x00 };

	_fd = open(_fname.c_str(), flag, mode);
	if(_fd == -1) {
		return -1;
	}

	if (mpl_lockfile(_fd) == -1) {
		if (EACCES == errno || EAGAIN == errno) {
			close(_fd); _fd = -1;
			return 1;
		}
		return -1;
	}
	ftruncate(_fd, 0);
	sprintf(buf, "%ld", getpid());
	write(_fd, buf, strlen(buf));
	return 0;
}

void MLockFile::unlock()
{
	if (-1 == _fd) return;

	struct flock lock;
	lock.l_type = F_UNLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	fcntl(_fd, F_SETLK, &lock);

	close(_fd);
	_fd = -1;
}

MLockFile &MLockFile::operator=(const MLockFile &other)
{
	inner_copy(other);
	return *this;
}

MLockFile &MLockFile::operator=(const char *s)
{
	_fname = s;
	_fd = -1;
	return *this;
}

MLockFile &MLockFile::operator=(const std::string &str)
{
	_fname = str;
	_fd = -1;
	return *this;
}

bool MLockFile::operator==(const MLockFile &f) const
{
	return (_fname == f._fname) && (_fd == f._fd);
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
