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
#ifndef _MHASH_H_
#define _MHASH_H_

#include <mpl/mcoredef.h>

MPL_BEGIN_NAMESPACE

std::string md5sum(const std::string &file);
std::string sha1sum(const std::string &file);
std::string sha224sum(const std::string &file);
std::string sha256sum(const std::string &file);
std::string sha384sum(const std::string &file);
std::string sha512sum(const std::string &file);

MPL_END_NAMESPACE

#endif /* _MHASH_H_ */
