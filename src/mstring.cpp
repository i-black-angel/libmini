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
#include <mpl/mstring.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

std::string toXString(const char *fmt, ...)
{
	std::string buffer;
	va_list vargs;
	va_start(vargs, fmt);
	int len = vsnprintf(NULL, 0, fmt, vargs);
	buffer.resize(len);
	va_start(vargs, fmt);
	vsnprintf(&buffer[0], len + 1, fmt, vargs);
	va_end(vargs);

	return buffer;
}

std::string hex2str(const uint8_t *data, size_t len)
{
	std::string buffer;
	char buf[3] = {0x00};
	for (size_t m = 0; m < len; ++m) {
		snprintf (buf, sizeof(buf), "%02X", data[m]);
		buf[2] = '\0';
		buffer += buf;
		buffer += " ";
	}

	if (buffer.size() > 0) {
		buffer.erase(buffer.begin() + (buffer.size() - 1));
	}
	return buffer;
}

std::string toString(int val)
{
	return toXString("%d", val);
}

std::string toString(unsigned val)
{
	return toXString("%u", val);
}

std::string toString(long val)
{
	return toXString("%ld", val);
}

std::string toString(unsigned long val)
{
	return toXString("%lu", val);
}

std::string toString(long long val)
{
	return toXString("%lld", val);
}

std::string toString(unsigned long long val)
{
	return toXString("%llu", val);
}

std::string toString(float val)
{
	return toXString("%f", val);
}
	
std::string toString(double val)
{
	return toXString("%f", val);
}

std::string toString(long double val)
{
	return toXString("%Lf", val);
}

std::string toUTF8(const std::string &str)
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	// convert GBK to Unicode
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t *strUnicode = new wchar_t[len];
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, strUnicode, len);

	// convert Unicode to UTF-8
	len = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	char * strUtf8 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, strUtf8, len, NULL, NULL);

	std::string strOut(strUtf8); // strTemp is encoding UTF-8
	delete[]strUnicode;
	delete[]strUtf8;
	strUnicode = NULL;
	strUtf8 = NULL;
	return strOut;
#elif defined(HAVE_ICONV_H)
	char *gbk = new char[str.size() + 1];
	size_t inbytes = str.size();
	strcpy(gbk, str.c_str());

	char *utf8 = new char[str.size() * 2 + 1]; // twice of gbk
	size_t outbytes = str.size() * 2;
		
	char *inptr = gbk;
	char *outptr = utf8;

	// iconv_open
	iconv_t cd = iconv_open("UTF-8", "GB18030");
	if (cd == (iconv_t)-1) {
		log_error("iconv_open failed: %s", error().c_str());
		delete gbk;
		delete utf8;
		return std::string();
	}

	// iconv
	size_t res = iconv(cd, &inptr, &inbytes, &outptr, &outbytes);
	if (res == -1) {
		log_error("iconv failed: %s", error().c_str());
		iconv_close(cd);
		delete gbk;
		delete utf8;
		return std::string();		
	}

	// iconv_close
	std::string outstr = utf8;

	iconv_close(cd);
	delete gbk;
	delete utf8;
	return outstr;
#else
	return str;
#endif
}

std::string toGBK(const std::string &str)
{
#if defined(_MSC_VER) || defined(M_OS_WIN)
	// convert UTF-8 to Unicode
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	unsigned short *wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)str.c_str(), -1, (LPWSTR)wszGBK, len);

	// convert UTF-8 to Unicode
	len = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
#elif defined(HAVE_ICONV_H)
	char *utf8 = new char[str.size() + 1];
	size_t inbytes = str.size();
	strcpy(utf8, str.c_str());

	char *gbk = new char[str.size() * 2 + 1]; // twice size
	size_t outbytes = str.size() * 2;
		
	char *inptr = utf8;
	char *outptr = gbk;

	// iconv_open
	iconv_t cd = iconv_open("GB18030", "UTF-8");
	if (cd == (iconv_t)-1) {
		log_error("iconv_open failed: %s", error().c_str());
		delete gbk;
		delete utf8;
		return std::string();
	}

	// iconv
	size_t res = iconv(cd, &inptr, &inbytes, &outptr, &outbytes);
	if (res == -1) {
		log_error("iconv failed: %s", error().c_str());
		iconv_close(cd);
		delete gbk;
		delete utf8;
		return std::string();		
	}

	// iconv_close
	std::string outstr = gbk;

	iconv_close(cd);
	delete gbk;
	delete utf8;
	return outstr;
#else
	return str;
#endif
}

MString::MString()
{
}

MString::MString(const std::string &str)
	: std::string(str)
{
}

MString::MString(const char *s)
	: std::string(s)
{
}

MString::~MString()
{
}

std::string& MString::replace(char before, char after)
{
	size_type pos = rfind(before);
	while (pos != npos) {
		std::string::replace(pos, 1, 1, after);
		pos = rfind(before);
	}
	return *this;
}

std::string& MString::replace(const std::string &before, const std::string &after)
{
	size_type pos = rfind(before);
	while (pos != npos) {
		std::string::replace(pos, before.size(), after);
		pos = rfind(before);
	}
	return *this;
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
