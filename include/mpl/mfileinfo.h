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
#ifndef _MFILEINFO_H_
#define _MFILEINFO_H_

#include <mpl/mcoredef.h>
#include <mpl/mdatetime.h>

MPL_BEGIN_NAMESPACE

class MFileInfo
{
public:
	enum FileType {
		unknown,
		fifo,
		chardev,
		directory,
		blockdev,
		normal,
		symbolic_link,
		sock,
	};

	enum Permission {
		ReadUser				= S_IRUSR,
		WriteUser				= S_IWUSR,
		ExecUser				= S_IXUSR,
		ReadWriteExecUser		= S_IRWXU,
		ReadGroup				= S_IRGRP,
		WriteGroup				= S_IWGRP,
		ExecGroup				= S_IXGRP,
		ReadWriteExecGroup		= S_IRWXG,
		ReadOther				= S_IROTH,
		WriteOther				= S_IWOTH,
		ExecOther				= S_IXOTH,
		ReadWriteExecOther		= S_IRWXO
	};

    MFileInfo();
	MFileInfo(const char *s);
	MFileInfo(const std::string &file);
	MFileInfo(const MFileInfo &fileinfo);
    virtual ~MFileInfo();

	std::string data() const { return _data; }
	std::string filePath() const { return _file; }
	std::string absoluteFilePath() const;
	std::string canonicalFilePath() const;
	std::string dirName() const;
	std::string baseName() const;
	std::string fileName() const;
	std::string suffix() const;

	std::string path() const;
	std::string absolutePath() const;
	std::string canonicalPath() const;

	bool isExists() const;		// F_OK
	bool isReadable() const;	// R_OK
	bool isWritable() const;	// W_OK
	bool isExecutable() const;	// X_OK

	inline bool isRelative() const { return !isAbsolute(); }
	inline bool isAbsolute() const { return isAbsoluteFileName(_file); }

	FileType fileType() const;
	std::string fileTypeString() const;
	
	bool isFile() const;
	bool isDir() const;
	bool isSymLink() const;
	bool isBlock() const;
	bool isCharDev() const;
	bool isFIFO() const;
	bool isSock() const;

#ifdef M_OS_LINUX
	std::string readLink() const;
	inline std::string symLinkTarget() const { return readLink(); }
#endif

	std::string owner() const;
	uint32_t ownerId() const;
	std::string group() const;
	uint32_t groupId() const;

	size_t size() const;
	void refresh();

	int permissions() const;
	bool permission(int perm) const;
	
	MDateTime lastStatusChanged() const;
	MDateTime lastModified() const;
	MDateTime lastRead() const;

	MFileInfo& operator=(const MFileInfo &fileinfo);
	MFileInfo& operator=(const std::string &str);
	MFileInfo& operator=(const char *s);
	
	bool operator==(const MFileInfo &fileinfo) const;
	inline bool operator!=(const MFileInfo &fileinfo) const
		{ return !operator==(fileinfo); }

	static bool exists(const std::string &file);
	static std::string slash();
	static char separator();
	static bool isSlash(char ch);
	static bool isAbsoluteFileName(const std::string &filename);
	static bool isRelativeFileName(const std::string &filename);
	static std::string cleanPath(const std::string &path);

protected:
	void setFile(const std::string &file);

private:
	inline void innerCopy(const MFileInfo &other) {
		_data = other._data;
		_file = other._file;
		_stat = other._stat;
		_stat_ok = other._stat_ok;
	}

	std::string _data;
	std::string _file;
	struct stat _stat;
	bool _stat_ok;
};

MPL_END_NAMESPACE

#endif /* _MFILEINFO_H_ */
