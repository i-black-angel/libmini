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
#ifndef _MSETTINGS_H_
#define _MSETTINGS_H_

#include <minion/mcoredef.h>
#include <minion/msettings_p.h>

MINION_BEGIN_NAMESPACE

class MSettings
{
public:
	MSettings(bool isUtf8 = false);
    explicit MSettings(const char *path, bool isUtf8 = false);
    virtual ~MSettings();

	void load(const char *path);
	std::string path() const { return _path; }
	bool save();
	bool save(const char *path);
	
	int value(const char *section, const char *key, int def) const;
	double value(const char *section, const char *key, double def) const;
	std::string value(const char *section, const char *key, const char *def) const;
	bool value(const char *section, const char *key, bool def) const;

	bool setValue(const char *section, const char *key, const char *value);
	bool setValue(const char *section, const char *key, int value);
	bool setValue(const char *section, const char *key, double value);
	bool setValue(const char *section, const char *key, bool value); 
protected:
	std::string sierror(int err) const;
private:
	std::string _path;
	CSimpleIni *_simpleini;
};

MINION_END_NAMESPACE

#endif /* _MSETTINGS_H_ */
