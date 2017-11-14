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
#include <mini.h>
#include "../src/muuid_p.h"

int main(int argc, char *argv[])
{
	for (size_t i = 0; i < 10; ++i) {
		std::cout << mini::uuidgen() << std::endl;
		usleep(30000);
	}

	uuid_t uu1;
	uuid_t uu2;
	uuid_clear(uu1);
	uuid_compare(uu1, uu2);
	uuid_copy(uu1, uu2);
	uuid_generate(uu1);
	uuid_generate_random(uu1);
	uuid_generate_time(uu2);
	std::cout << uuid_generate_time_safe(uu2) << std::endl;
	std::cout << uuid_is_null(uu2) << std::endl;
	std::cout << uuid_is_null(uu1) << std::endl;

	char buf[256] = {0};
	uuid_unparse(uu1, buf); std::cout << buf << std::endl;
	uuid_unparse(uu2, buf); std::cout << buf << std::endl;
	std::cout << uuid_parse(buf, uu1) << std::endl;
	uuid_unparse_lower(uu1, buf); std::cout << buf << std::endl;
	uuid_unparse_upper(uu1, buf); std::cout << buf << std::endl;

	struct timeval tv;
	time_t time = uuid_time(uu1, &tv);
	std::cout << time << std::endl;
	std::cout << uuid_type(uu1) << std::endl;
	std::cout << uuid_variant(uu1) << std::endl;
    return 0;
}
