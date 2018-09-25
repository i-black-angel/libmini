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
#include <mpl/mfileinfo.h>
#include <mpl/mdatetime.h>
#include <mpl/merror.h>
#include <mpl/mlog.h>
#include <mpl/mstring.h>
#include <mpl/mprocess.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

// idea of these function is copied from
// Qt5.9.1/5.9.1/Src/qtbase/src/corelib/io/qdir.cpp
static int mpl_root_length(const std::string &name) 
{
	const int len = name.length();
#ifdef M_OS_WIN
	// Handle a possible drive letter
	if (len >= 2 && isalpha(name.at(0)) && name.at(1) == ':') {
		return (len > 2 && name.at(2) == '/') ? 3 : 2;
	} 
#endif /* M_OS_WIN */
	if (name.at(0) == '/')
		return 1;
	return 0;
}

static std::string mpl_normalize_path_segments(const std::string &name)
{
	const int len = name.length();
	if (len == 0)
		return name;

	int i = len - 1;			// from last character
	std::vector<char> outVector(len);
	int used = len;
	char *out = (char *)&outVector[0];
	const char *p = name.data();
	const char *prefix = p;
	int up = 0;

	const int prefixLength = mpl_root_length(name);
	p += prefixLength;
	i -= prefixLength;
	
	// replicate trailing slash (i > 0 checks for emptiness of input string p)
	if (i > 0 && p[i] == '/') {
		out[--used] = '/';
		--i;
	}

	while (i >= 0) {
		// remove trailing slashes
		if (p[i] == '/') {
			--i;
			continue;
		}

		// remove current directory
		if (p[i] == '.' && (i == 0 || p[i-1] == '/')) {
			--i;
			continue;
		}

		// detect up dir
		if (i >= 1 && p[i] == '.' && p[i-1] == '.'
			&& (i == 1 || (i >= 2 && p[i-2] == '/'))) {
			++up;
			i -= 2;
			continue;
		}

		// prepend a slash before copying when ont empty
		if (!up && used != len && out[used] != '/')
			out[--used] = '/';

		// skip or copy
		while (i >= 0) {
			if (p[i] == '/') {	// do not copy slashes
				--i;
				break;
			}
			// actual copy
			if (!up)
				out[--used] = p[i];
			--i;
		} // while

		// decrement up after copying/skipping
		if (up)
			--up;
	} // while

	// add remaining '..'
	while (up) {
		// is not empty and there isn't already a '/'
		if (used != len && out[used] != '/') 
			out[--used] = '/';
		out[--used] = '.';
		out[--used] = '.';
		--up;
	}

	bool isEmpty = (used == len);
	if (prefixLength) {
		if (!isEmpty && out[used] == '/') {
			// Eventhough there is a prefix the out string is a slash. This
			// happens, if the input string only consists of a prefix followed
			// by one or more slashes.
			++used;
		}
		for (int i = prefixLength - 1; i >= 0; --i) {
			out[--used] = prefix[i];
		}
	} else {
		if (isEmpty) {
			// After resolving the input path, the resulting string is empty (
			// e.g. "foo/.."). Return a dot in that case.
			out[--used] = '.';
		} else if (out[used] == '/') {
			// After parsing the input string, out only contains a slash. That
			// happens whenever all parts are resolved and there is a trailing
			// slash ("./" or "foo/../" for example). Prepend a dot to have the
			// correct return value.
			out[--used] = '.';
		}
	}
	// If path was not modified return the original value
	if (used == 0)
		return name;
	return std::string(out + used, len - used);
}

static std::string mpl_clean_path(const std::string &path)
{
	if (path.empty()) return path;
	mpl::MString name = path;
	char dir_separator = mpl::MFileInfo::separator();
	if (dir_separator != '/') 
		name.replace(dir_separator, '/');

	std::string ret = mpl_normalize_path_segments(name);

	// Strip away last slash except for root directories
	if ((ret.length() > 1) && (ret.at(ret.length() - 1) == '/')) {
#ifdef M_OS_WIN
		if (!(ret.length() == 3 && ret.at(1) == ':'))
#endif /* M_OS_WIN */
			ret.erase(ret.size() - 1, 1);
	}

	return ret;
}

static bool mpl_access(const std::string &__name, int __type)
{
	int err = 0;
	if (__name.empty()) {
		return false;
	}
	if ((err = access(__name.c_str(), __type)) != 0) {
		return false;
	}
	return true;
}

static int mpl_filesystem_prefix_len(const std::string &filename)
{
#ifdef M_OS_WIN
	if (filename.size() < 2) return 0;
	return (isalpha(filename[0]) && filename[1] == ':') ? 2 : 0;
#else	
	return 0;
#endif /* M_OS_WIN */
}

MPL_BEGIN_NAMESPACE

std::string MFileInfo::slash()
{
#ifdef M_OS_WIN
	return "/\\";
#else
	return "/";
#endif /* M_OS_WIN */
}

bool MFileInfo::isslash(char ch)
{
#ifdef M_OS_WIN
	return ((ch == '/') || (ch == '\\'));
#else
	return (ch == '/');
#endif
}

bool MFileInfo::isAbsoluteFileName(const std::string &filename)
{
	if (filename.empty()) return false;
	return isslash(filename[0]) || (mpl_filesystem_prefix_len(filename) != 0);
}

MFileInfo::MFileInfo() : _stat_ok(false)
{
}

MFileInfo::MFileInfo(const char *s) : _stat_ok(false)
{
	setFile(s);
}

MFileInfo::MFileInfo(const std::string &file) : _stat_ok(false)
{
	setFile(file);
}

MFileInfo::MFileInfo(const MFileInfo &fileinfo)
{
	inner_copy(fileinfo);
}

MFileInfo::~MFileInfo()
{
}

MFileInfo& MFileInfo::operator=(const MFileInfo &fileinfo)
{
	inner_copy(fileinfo);
	return *this;
}

MFileInfo& MFileInfo::operator=(const std::string &str)
{
	setFile(str);
	return *this;
}

MFileInfo& MFileInfo::operator=(const char *s)
{
	setFile(s);
	return *this;
}

bool MFileInfo::operator==(const MFileInfo &fileinfo) const
{
	return _file == fileinfo._file;
}

void MFileInfo::setFile(const std::string &file)
{
	_origin = file;
	_file = cleanPath(file);

#if defined(_MSC_VER) || defined(M_OS_WIN)
	if (stat(_file.c_str(), &_stat) < 0) 
#else
	if (lstat(_file.c_str(), &_stat) < 0) 
#endif
		_stat_ok = false;
	else
		_stat_ok = true;
}

bool MFileInfo::exists() const
{
	return exists(_file);
}

bool MFileInfo::exists(const std::string &file)
{
	return mpl_access(cleanPath(file), F_OK);
}

std::string MFileInfo::filePath() const
{
	return _file;
}

std::string MFileInfo::absoluteFilePath() const
{
	if (isAbsolute()) 
		return filePath();

	return cleanPath(process::pwd() + "/" + _file);
}

std::string MFileInfo::canonicalFilePath() const
{
	// If the FILE dose not exist, just return an empty string.
	if (!isAbsolute()) {
		std::string file = cleanPath(process::pwd() + "/" + _file);
		if (!exists(file))
			return std::string();
		return file;
	}

	return filePath();
}

std::string MFileInfo::filename() const
{
	std::string file = basename();
	size_t idx = file.find_last_of(".");
	if (idx == std::string::npos) { return file; }
    return file.substr(0, idx);	
}

std::string MFileInfo::basename() const
{
	std::string file = _file;
	size_t idx = file.find_last_of(slash().c_str());
	if (idx == std::string::npos) { return file; }
	return file.substr(idx + 1);
}

std::string MFileInfo::dirname() const
{
	std::string file = _file;
	size_t idx = file.find_last_of(slash().c_str());
	if (idx == std::string::npos) { return std::string("."); }
	if (idx == 0) idx = 1;
    return file.substr(0, idx);
}

std::string MFileInfo::suffix() const
{
	std::string file = basename();
	size_t idx = file.find_last_of(".");
	if (idx == std::string::npos) { return std::string(""); }
    return file.substr(idx + 1);
}

std::string MFileInfo::path() const
{
	return dirname();
}

std::string MFileInfo::absolutePath() const
{
	if (isAbsolute())
		return path();

	return cleanPath(process::pwd() + "/" + path());
}

std::string MFileInfo::canonicalPath() const
{
	// If the FILE dose not exist, just return an empty string.
	if (!isAbsolute()) {
		std::string p = cleanPath(process::pwd() + "/" + path());
		if (!exists(p))
			return std::string();
		return p;
	}

	return path();
}

bool MFileInfo::isReadable() const
{
	return mpl_access(_file.c_str(), R_OK | F_OK);
}

bool MFileInfo::isWritable() const
{
	return mpl_access(_file.c_str(), W_OK | F_OK);
}


bool MFileInfo::isExecutable() const
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return true;
#else
	return mpl_access(_file.c_str(), X_OK | F_OK);
#endif
}


bool MFileInfo::isFile() const
{
	if (!_stat_ok) return false;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return (FILE_ATTRIBUTE_NORMAL & GetFileAttributes(_file.c_str())) ? true : false;
#else	
	return S_ISREG(_stat.st_mode);
#endif
}

bool MFileInfo::isDir() const
{
	if (!_stat_ok) return false;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return (FILE_ATTRIBUTE_DIRECTORY & GetFileAttributes(_file.c_str())) ? true : false;
#else	
	return S_ISDIR(_stat.st_mode);
#endif
}

bool MFileInfo::isSymLink() const
{
	if (!_stat_ok) return false;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return false;
#else	
	return S_ISLNK(_stat.st_mode);
#endif
}

bool MFileInfo::isBlock() const
{
	if (!_stat_ok) return false;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return false;
#else	
	return S_ISBLK(_stat.st_mode);
#endif
}

bool MFileInfo::isCharDev() const
{
	if (!_stat_ok) return false;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return false;
#else
	return S_ISCHR(_stat.st_mode);
#endif
}

bool MFileInfo::isFIFO() const
{
	if (!_stat_ok) return false;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return false;
#else
	return S_ISFIFO(_stat.st_mode);
#endif
}

bool MFileInfo::isSock() const
{
	if (!_stat_ok) return false;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return false;
#else
	return S_ISSOCK(_stat.st_mode);
#endif
}

#ifdef M_OS_LINUX
std::string MFileInfo::readLink() const
{
	struct stat sb;
	ssize_t r = 0;
	char *linkname = NULL;

	if (lstat(_file.c_str(), &sb) == -1) {
		log_error("lstat %s failed: %s", _file.c_str(), error().c_str());
		return std::string();
	}
	linkname = (char *) ::malloc(sb.st_size + 1);
	if (linkname == NULL) {
		log_error("insufficient memory");
		return std::string();
	}

	r = ::readlink(_file.c_str(), linkname, sb.st_size + 1);

	if (r == -1) {
		log_error("readlink %s failed: %s", _file.c_str(), error().c_str());
		::free(linkname);
		return std::string();
	}

	if (r > sb.st_size) {
		log_error("symlink increased in size "
				  "between lstat() and readlink()");
		::free(linkname);
		return std::string();
	}

	linkname[r] = '\0';

	std::string res = linkname;
	::free(linkname);
	
	return res;
}
#endif

std::string MFileInfo::owner() const
{
	if (!_stat_ok) return std::string();
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return "";
#else
	struct passwd *pwd = getpwuid(_stat.st_uid);
	if (pwd == NULL) return std::string();
	// log_debug("passwd: %s", pwd->pw_passwd);
	return std::string(pwd->pw_name);
#endif
}

uint32_t MFileInfo::ownerId() const
{
	if (!_stat_ok) return -1;
	return _stat.st_uid;
}

std::string MFileInfo::group() const
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return "";
#else
	if (!_stat_ok) return std::string();
	struct group *grp = getgrgid(_stat.st_gid);
	if (grp == NULL) return std::string();
	return std::string(grp->gr_name);
#endif
}

uint32_t MFileInfo::groupId() const
{
	if (!_stat_ok) return -1;
	return _stat.st_gid;
}

int MFileInfo::permissions() const
{
	if (!_stat_ok) return 0;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return GetFileAttributes(_file.c_str());
#else
	return _stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

bool MFileInfo::permission(int perm) const
{
	if (!_stat_ok) return false;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	return ((GetFileAttributes(_file.c_str()) & perm) != 0);
#else
	return _stat.st_mode & perm;
#endif
}

size_t MFileInfo::size() const
{
	if (!_stat_ok) return 0;
	return _stat.st_size;
}

void MFileInfo::refresh()
{
	setFile(_origin);
}

MDateTime MFileInfo::lastStatusChanged() const
{
	if (!_stat_ok) return MDateTime(0);
	return MDateTime(_stat.st_ctime);
}

MDateTime MFileInfo::lastModified() const
{
	if (!_stat_ok) return MDateTime(0);
	return MDateTime(_stat.st_mtime);
}

MDateTime MFileInfo::lastRead() const
{
	if (!_stat_ok) return MDateTime(0);
	return MDateTime(_stat.st_atime);
}

MFileInfo::FileType MFileInfo::filetype() const
{
	if (!_stat_ok) return unknown;
	FileType ft = unknown;
#if defined(_MSC_VER) || defined(M_OS_WIN)
	DWORD res = GetFileAttributes(_file.c_str());
	if (res & FILE_ATTRIBUTE_NORMAL) {
		ft = normal;
	} else if (res & FILE_ATTRIBUTE_DIRECTORY) {
		ft = directory;
	}
#else
	if (S_ISREG(_stat.st_mode)) {
		ft = normal;
	} else if (S_ISDIR(_stat.st_mode)) {
		ft = directory;
	} else if (S_ISFIFO(_stat.st_mode)) {
		ft = fifo;
	} else if (S_ISCHR(_stat.st_mode)) {
		ft = chardev;
	} else if (S_ISBLK(_stat.st_mode)) {
		ft = blockdev;
	} else if (S_ISLNK(_stat.st_mode)) {
		ft = symbolic_link;
	} else if (S_ISSOCK(_stat.st_mode)) {
		ft = sock;
	}
#endif
	return ft;
}

std::string MFileInfo::filetypeString() const
{
	switch(filetype()) {
	case MFileInfo::fifo:
		return "fifo";
	case MFileInfo::chardev:
		return "chardev (character special)";
	case MFileInfo::directory:
		return "directory";
	case MFileInfo::blockdev:
		return "blockdev (block special)";
	case MFileInfo::normal:
		return "normal";
	case MFileInfo::symbolic_link:
		return "symbolic link";
	case MFileInfo::sock:
		return "socket";
	}
	return "unknown";
}

char MFileInfo::separator()
{
#ifdef M_OS_WIN
	return '\\';
#else
	return '/';
#endif /* M_OS_WIN */
}

std::string MFileInfo::cleanPath(const std::string &path)
{
	return mpl_clean_path(path);
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
