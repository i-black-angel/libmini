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
#include <pwd.h>
#include <grp.h>

MPL_BEGIN_NAMESPACE

static bool __access(const std::string &__name, int __type)
{
	int err = 0;
	if (__name.empty()) {
		// log_error("filename is empty");
		return false;
	}
	if ((err = access(__name.c_str(), __type)) != 0) {
		// log_error("access %s failed: %s", __name.c_str(), error().c_str());
		return false;
	}
	return true;
}

static std::string slash()
{
#ifdef M_OS_WIN
	return "/\\";
#else
	return "/";
#endif /* M_OS_WIN */
}

MFileInfo::MFileInfo() : _stat_ok(false)
{
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

bool MFileInfo::operator==(const MFileInfo &fileinfo) const
{
	return _file == fileinfo._file;
}

void MFileInfo::setFile(const std::string &file)
{
	_file = file;

	if (lstat(_file.c_str(), &_stat) < 0) {
		_stat_ok = false;
		log_error("lstat %s failed: %s", _file.c_str(), error().c_str());
	} else {
		_stat_ok = true;
	}
}

bool MFileInfo::exists() const
{
	return exists(_file);
}

bool MFileInfo::exists(const std::string &file)
{
	return __access(file, F_OK);
}

std::string MFileInfo::filePath() const
{
	return dirname();
}

std::string MFileInfo::absoluteFilePath() const
{
}

std::string MFileInfo::canonicalFilePath() const
{
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

std::string MFileInfo::completeBaseName() const
{
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

std::string MFileInfo::bundleName() const
{
}

std::string MFileInfo::completeSuffix() const
{
}


std::string MFileInfo::path() const
{
	return _file;
}

std::string MFileInfo::absolutePath() const
{
}

std::string MFileInfo::canonicalPath() const
{
}

bool MFileInfo::isReadable() const
{
	return __access(_file.c_str(), R_OK | F_OK);
}

bool MFileInfo::isWritable() const
{
	return __access(_file.c_str(), W_OK | F_OK);
}

bool MFileInfo::isExecutable() const
{
	return __access(_file.c_str(), X_OK | F_OK);
}

bool MFileInfo::isHidden() const
{
}

bool MFileInfo::isNativePath() const
{
}

bool MFileInfo::isRelative() const
{
}

bool MFileInfo::makeAbsolute()
{
}

bool MFileInfo::isFile() const
{
	if (!_stat_ok) return false;
	return S_ISREG(_stat.st_mode);
}

bool MFileInfo::isDir() const
{
	if (!_stat_ok) return false;
	return S_ISDIR(_stat.st_mode);
}

bool MFileInfo::isSymLink() const
{
	if (!_stat_ok) return false;
	return S_ISLNK(_stat.st_mode);
}

bool MFileInfo::isBlock() const
{
	if (!_stat_ok) return false;
	return S_ISBLK(_stat.st_mode);
}

bool MFileInfo::isCharDev() const
{
	if (!_stat_ok) return false;
	return S_ISCHR(_stat.st_mode);
}

bool MFileInfo::isFIFO() const
{
	if (!_stat_ok) return false;
	return S_ISFIFO(_stat.st_mode);
}

bool MFileInfo::isSock() const
{
	if (!_stat_ok) return false;
	return S_ISSOCK(_stat.st_mode);
}
// bool MFileInfo::isRoot() const
// {
// }

// bool MFileInfo::isBundle() const
// {
// }

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

std::string MFileInfo::owner() const
{
	if (!_stat_ok) return std::string();
	struct passwd *pwd = getpwuid(_stat.st_uid);
	// log_debug("passwd: %s", pwd->pw_passwd);
	return std::string(pwd->pw_name);
}

uint32_t MFileInfo::ownerId() const
{
	if (!_stat_ok) return -1;
	return _stat.st_uid;
}

std::string MFileInfo::group() const
{
	if (!_stat_ok) return std::string();
	struct group *grp = getgrgid(_stat.st_gid);
	return std::string(grp->gr_name);
}

uint32_t MFileInfo::groupId() const
{
	if (!_stat_ok) return -1;
	return _stat.st_gid;
}

// bool permission(MFile::Permissions permissions) const
// MFile::Permissions permissions() const

size_t MFileInfo::size() const
{
	if (!_stat_ok) return 0;
	return _stat.st_size;
}

void MFileInfo::refresh()
{
	setFile(_file);
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

MPL_END_NAMESPACE
