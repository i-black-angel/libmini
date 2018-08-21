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
#include <mpl/mdir.h>
#include <mpl/mfileinfo.h>
#include <mpl/merror.h>
#include <mpl/mlog.h>
#include <mpl/mprocess.h>
#include <dirent.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

static bool mpl_rm_file(const char *path, bool is_dir = false)
{
	if (path == NULL) return false;

	int flag = is_dir ? AT_REMOVEDIR : 0;
	if (unlinkat(AT_FDCWD, path, flag) == 0)
		return true;

	log_error("cannot remove '%s': %s", path, error().c_str());
	return false;
}
	
MDir::MDir()
{
}

MDir::MDir(const char *s)
{
	setPath(s);
}

MDir::MDir(const std::string &str)
{
	setPath(str);
}

MDir::MDir(const MDir &other)
{
	inner_copy(other);
}

MDir::~MDir()
{
}

void MDir::setPath(const std::string &name)
{
	_dir = MFileInfo::cleanPath(name);
}

bool MDir::isDir() const
{
	return MFileInfo(_dir).isDir();
}

bool MDir::isRoot() const
{
	return _dir == rootPath();
}

bool MDir::exists() const
{
	return exists(_dir);
}

bool MDir::isReadable() const
{
	return MFileInfo(_dir).isReadable();
}

bool MDir::cd(const std::string &name)
{
	if (name.empty() || name == ".")
		return true;

	std::string dirpath = MFileInfo::cleanPath(name);
	if (!isAbsolutePath(dirpath))
		dirpath = MFileInfo(dirpath).absoluteFilePath();

    if (chdir(dirpath.c_str()) == 0) {
		setPath(dirpath);
		return true;
	}
	return false;
}

bool MDir::cdup()
{
	return cd("..");
}

bool MDir::exists(const std::string &name)
{
	return MFileInfo::exists(name);
}

bool MDir::mkpath(const std::string &path)
{
	int err = 0;
	mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
	std::string dirpath = MFileInfo(path).filePath();
	char *str = new char[dirpath.size() + 1];
	int len = dirpath.size();
	strncpy(str, dirpath.data(), len);
	str[len] = '\0';
	for (int i = 0; i < len; ++i) {
		if ( str[i] == separator() ) {
			str[i] = '\0';
			if (strlen(str) > 0 && !MFileInfo::exists(str)) {
				if ((err = mkdir(str, mode)) != 0) {
					log_error("mkdir %s failed: %s", str, error().c_str());
					delete[] str;
					return false;
				}
			}
			str[i] = separator();
		}
	}
	
	if (strlen(str) > 0 && !MFileInfo::exists(str)) {
		if ((err = mkdir(str, mode)) != 0) {
			log_error("mkdir %s failed: %s", str, error().c_str());
			delete[] str;
			return false;
		}
	}
	
	delete[] str;
	return true;
}

bool MDir::rmpath(const std::string &entry)
{
	if (entry.empty()) return true;
	
	std::stack<std::string> entries;
	std::string fullpath;
	struct dirent *dirp;

	std::string path = MFileInfo(entry).absoluteFilePath();
	DIR *dp = opendir(path.c_str());
	if (NULL == dp) {
		log_error("cannot open '%s'", path.c_str());
		return false;
	}
	closedir(dp);

	entries.push(path);
	while (!entries.empty()) {
		path = entries.top();
		dp = opendir(path.c_str());
		if (NULL == dp) continue;

		int subdir = 0;
		while ((dirp = readdir(dp)) != NULL) {
			if (strcmp(dirp->d_name, ".") == 0
				|| strcmp(dirp->d_name, "..") == 0)
				continue;
			fullpath = path + separator() + std::string(dirp->d_name);
			if (dirp->d_type == DT_DIR) {
				DIR *subdp = opendir(fullpath.c_str());
				if (NULL != subdp) {
					entries.push(fullpath);
					closedir(subdp);
					++subdir;
				} else {
					log_error("cannot remove '%s': %s", fullpath.c_str(), error().c_str());
				}
			} else {
				mpl_rm_file(fullpath.c_str());
			}
		} // while readdir()
		closedir(dp);
		if (subdir == 0) {
			mpl_rm_file(path.c_str(), true);
			entries.pop();
		}
	} // while entries.empty()
	return true;
}

std::string MDir::absolutePath() const
{
	return MFileInfo(_dir).absoluteFilePath();
}

std::string MDir::canonicalPath() const
{
	return MFileInfo(_dir).canonicalFilePath();
}

std::string MDir::currentPath()
{
	return process::pwd();
}

std::string MDir::homePath()
{
	char *val = getenv("HOME");
	std::string home = (val == NULL) ? "" : val;
	if (home.empty())
		home = rootPath();
	return MFileInfo::cleanPath(home);
}

std::string MDir::tempPath()
{
	char *val = getenv("TMPDIR");
	std::string temp = (val == NULL) ? "" : val;
	if (temp.empty()) {
		temp = "/tmp";
	}
	return MFileInfo::cleanPath(temp);
}

std::string MDir::rootPath()
{
	return "/";
}

char MDir::separator()
{
	return MFileInfo::separator();
}

bool MDir::isAbsolutePath(const std::string &path)
{
	return MFileInfo::isAbsoluteFileName(path);
}

MDir &MDir::operator=(const MDir &other)
{
	inner_copy(other);
	return *this;
}

MDir &MDir::operator=(const char *s)
{
	setPath(s);
	return *this;
}

MDir &MDir::operator=(const std::string &str)
{
	setPath(str);
	return *this;
}

std::ostream &operator<<(std::ostream &out, const MDir &dir)
{
	out << dir.dir();
	return out;
}

std::istream &operator>>(std::istream &in, MDir &dir)
{
	std::string str;
	in >> str;
	dir = str;
	return in;
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
