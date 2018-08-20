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
#ifndef _MLOCKFILE_H_
#define _MLOCKFILE_H_

#include <mpl/mcoredef.h>

MPL_BEGIN_NAMESPACE

class MLockFile
{
public:
    explicit MLockFile();
	MLockFile(const char *s);
	MLockFile(const std::string &str);
	MLockFile(const MLockFile &other);
    virtual ~MLockFile();

	int lock();
	void unlock();

	MLockFile &operator=(const MLockFile &other);
	MLockFile &operator=(const char *s);
	MLockFile &operator=(const std::string &str);

	bool operator==(const MLockFile &f) const;
	bool operator!=(const MLockFile &f) const
		{ return !operator==(f); }
private:
	inline void inner_copy(const MLockFile &o) {
		_fname = o._fname;
		_fd = o._fd;
	}
	std::string _fname;
	int _fd;
};

MPL_END_NAMESPACE

#endif /* _MLOCKFILE_H_ */
