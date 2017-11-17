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
#include <minion/mfileinfo.h>

MINION_BEGIN_NAMESPACE

static bool __access(const std::string &__name, int __type)
{
	int err = 0;
	if (__name.empty()) {
		// log_error("filename is empty");
		return false;
	}
	if ((err = access(__name.c_str(), __type)) != 0) {
		// errno
		// log_error(strerror(errno));
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

MFileInfo::MFileInfo()
{
}

MFileInfo::MFileInfo(const std::string &file)
{
	setFile(file);
}

// MFileInfo(const MFile &file);
// MFileInfo(const MDir &dir, const std::string &file);

MFileInfo::MFileInfo(const MFileInfo &fileinfo)
{
	_file = fileinfo._file;
}

MFileInfo::~MFileInfo()
{
}

MFileInfo& MFileInfo::operator=(const MFileInfo &fileinfo)
{
	_file = fileinfo._file;
	return *this;
}

bool MFileInfo::operator==(const MFileInfo &fileinfo) const
{
	return _file == fileinfo._file;
}

void MFileInfo::setFile(const std::string &file)
{
	_file = file;
}

// void setFile(const MFile &file);
// void setFile(const MDir &dir, const std::string &file);

bool MFileInfo::exists() const
{
	return exists(_file);
}

bool MFileInfo::exists(const std::string &file)
{
	return __access(file, F_OK);
}

// void MFileInfo::refresh()
// {
// }

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

// Dir dir() const
// Dir absoluteDir() const

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
}

bool MFileInfo::isDir() const
{
}

bool MFileInfo::isSymLink() const
{
}

bool MFileInfo::isRoot() const
{
}

bool MFileInfo::isBundle() const
{
}

std::string MFileInfo::readLink() const
{
	struct stat sb;
	ssize_t r = 0;
	char *linkname = NULL;

	if (lstat(_file.c_str(), &sb) == -1) {
		perror("lstat");
		return std::string();
	}
	linkname = (char *) ::malloc(sb.st_size + 1);
	if (linkname == NULL) {
		fprintf(stderr, "insufficient memory\n");
		return std::string();
	}

	r = ::readlink(_file.c_str(), linkname, sb.st_size + 1);

	if (r == -1) {
		perror("lstat");
		return std::string();
	}

	if (r > sb.st_size) {
		fprintf(stderr, "symlink increased in size "
			"between lstat() and readlink()\n");
		return std::string();
	}

	linkname[r] = '\0';

	std::string res = linkname;
	::free(linkname);
	
	return res;
}

std::string MFileInfo::owner() const
{
}

uint32_t MFileInfo::ownerId() const
{
}

std::string MFileInfo::group() const
{
}

uint32_t MFileInfo::groupId() const
{
}

// bool permission(MFile::Permissions permissions) const
// MFile::Permissions permissions() const

int64_t MFileInfo::size() const
{
}

MDateTime MFileInfo::created() const
{
}

MDateTime MFileInfo::lastModified() const
{
}

MDateTime MFileInfo::lastRead() const
{
}

MINION_END_NAMESPACE
