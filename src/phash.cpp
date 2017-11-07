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
#include <punica/phash.h>
#include <punica/plog.h>
#include "phash_md5.h"

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */
static inline void *
ptr_align (void const *ptr, size_t alignment)
{
	char const *p0 = (char const *)ptr;
	char const *p1 = p0 + alignment - 1;
	return (void *) (p1 - (size_t) p1 % alignment);
}

PUNICA_BEGIN_NAMESPACE

std::string md5sum(const std::string &file)
{
	int err = 0;
	FILE *fp = NULL;

	static const int digest_bits = 128;
	static const int digest_align = 4;
	static const int digest_hex_bytes = digest_bits / 4;
	static const int digest_bin_bytes = digest_bits / 8;

	uint8_t bin_buffer_unaligned[digest_bin_bytes + digest_align];
	/* Make sure bin_buffer is properly aligned. */
	uint8_t *bin_buffer = (uint8_t *)ptr_align (bin_buffer_unaligned, digest_align);

#ifdef P_OS_WIN
	fopen_s(&fp, file.c_str(), "rb");
#else
	fp = fopen(file.c_str(), "r");
#endif /* P_OS_WIN */
	if (NULL == fp) {
		log_error("%s", error().c_str());
		return std::string();
	}

	err = md5_stream(fp, bin_buffer);
	if (err) {
		log_error("%s", error().c_str());
		fclose(fp);
		return std::string();
	}

	std::string res;
	char buf[3] = { 0x00 };
	for (size_t i = 0; i < (digest_hex_bytes / 2); ++i) {
#ifdef P_OS_WIN
		sprintf_s(buf, "%02x", bin_buffer[i]);
#else
		sprintf(buf, "%02x", bin_buffer[i]);
#endif
		res += buf;
	}

	fclose(fp);
	return res;
}

std::string sha1sum(const std::string &file)
{
}

std::string sha224sum(const std::string &file)
{
}

std::string sha256sum(const std::string &file)
{
}

std::string sha384sum(const std::string &file)
{
}

std::string sha512sum(const std::string &file)
{
}

PUNICA_END_NAMESPACE
