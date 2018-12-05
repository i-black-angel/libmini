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
#ifndef _MFILE_H_
#define _MFILE_H_

#include <mpl/mcoredef.h>

/* Get the definition of O_*, F_*, FD_*: all the
   numbers and flag bits for `open', `fcntl', et al. */
/* /usr/include/bits/fcntl-linux.h */

MPL_BEGIN_NAMESPACE

namespace file
{
	int appendLine(const std::string &file, const std::string &line);
	int readbuf(const std::string &file, char *buf, size_t bytes);
	int writebuf(const std::string &file, const char *buf, size_t bytes);
	int appendbuf(const std::string &file, const char *buf, size_t bytes);
}

class MFile
{
public:
    MFile();
	MFile(const char *s);
    MFile(const std::string &str);
	MFile(const MFile &other);
    virtual ~MFile();

	std::string name() const { return _fname; }
	
	int readbuf(char *buf, size_t bytes) const;
	int writebuf(const char *buf, size_t bytes) const;
	
	MFile &operator=(const MFile &other);
	MFile &operator=(const char *s);
	MFile &operator=(const std::string &str);

	bool operator==(const MFile &f) const;
	bool operator!=(const MFile &f) const
		{ return !operator==(f); }
private:
	inline void innerCopy(const MFile &o) {
		_fname = o._fname;
	}
	std::string _fname;
};

MPL_END_NAMESPACE

#endif /* _MFILE_H_ */
