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
#include <mpl/msettings.h>
#include <mpl/mlog.h>
#include <mpl/msettings_p.h>
#include <mpl/merror.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

#define SI_ERROR_STR_OK          "No error"
#define SI_ERROR_STR_UPDATED     "An existing value was updated"
#define SI_ERROR_STR_INSERTED    "a new value was inserted"
#define SI_ERROR_STR_FAIL        "Generic failure"
#define SI_ERROR_STR_NOMEM       "Out of memory error"
#define SI_ERROR_STR_FILE        "File error"

MSettings::MSettings(bool isUtf8)
{
	_simpleini = new CSimpleIni(isUtf8);
}

MSettings::MSettings(const char *path, bool isUtf8)
{
	_simpleini = new CSimpleIni(isUtf8);
	load(path);
}

MSettings::~MSettings()
{
	SAFE_DELETE(_simpleini);
}

std::string MSettings::sierror(int err) const
{
	switch (err) {
	case SI_OK:
		return SI_ERROR_STR_OK;
	case SI_UPDATED:
		return SI_ERROR_STR_UPDATED;
	case SI_INSERTED:
		return SI_ERROR_STR_INSERTED;
	case SI_FAIL:
		return SI_ERROR_STR_FAIL;
	case SI_NOMEM:
		return SI_ERROR_STR_NOMEM;
	// case SI_FILE:
	// 	return error();
	}
	return error();
}

void MSettings::load(const char *path)
{
	if (path != NULL) {
		SI_Error err = _simpleini->LoadFile(path);
		if (SI_OK != err) {
			log_error("LoadFile %s failed: %s", path, sierror(err).c_str());
		} else {
			log_debug("LoadFile %s success", path);
		}
	} else {
		log_error("path is nullptr");
	}
}

bool MSettings::save()
{
	return save(_path.c_str());
}

bool MSettings::save(const char *path)
{
	SI_Error err = _simpleini->SaveFile(path);
	if (SI_OK != err) {
		log_error("SaveFile %s failed: %s", path, sierror(err).c_str());
		return false;
	}
	log_debug("SaveFile %s success");
	return true;
}
	
int MSettings::value(const char *section, const char *key, int def) const
{
	return (int)_simpleini->GetLongValue(section, key, (long)def);
}

double MSettings::value(const char *section, const char *key, double def) const
{
	return _simpleini->GetDoubleValue(section, key, def);
}

std::string MSettings::value(const char *section, const char *key, const char *def) const
{
	return _simpleini->GetValue(section, key, def);
}

bool MSettings::value(const char *section, const char *key, bool def) const
{
	return _simpleini->GetBoolValue(section, key, def);
}

bool MSettings::setValue(const char *section, const char *key, const char *value)
{
	SI_Error err = _simpleini->SetValue(section, key, value);
	if (err != SI_OK) {
		log_error("SetValue %s/%s failed: %s", section, key, sierror(err).c_str());
		return false;
	}
	return true;
}

bool MSettings::setValue(const char *section, const char *key, int value)
{
	SI_Error err = _simpleini->SetLongValue(section, key, (long)value);
	if (err != SI_OK) {
		log_error("SetValue %s/%s failed: %s", section, key, sierror(err).c_str());
		return false;
	}
	return true;
}

bool MSettings::setValue(const char *section, const char *key, double value)
{
	SI_Error err = _simpleini->SetDoubleValue(section, key, value);
	if (err != SI_OK) {
		log_error("SetValue %s/%s failed: %s", section, key, sierror(err).c_str());
		return false;
	}
	return true;
}

bool MSettings::setValue(const char *section, const char *key, bool value)
{
	SI_Error err = _simpleini->SetBoolValue(section, key, value);
	if (err != SI_OK) {
		log_error("SetValue %s/%s failed: %s", section, key, sierror(err).c_str());
		return false;
	}
	return true;
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
