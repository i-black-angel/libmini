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
#include <minion.h>

int main(int argc, char *argv[])
{
	// std::string path = minion::applicationDirPath();
	std::string path = minion::workingDirectory();
	path += "/settings.ini";
	std::cout << path << std::endl;
	minion::MSettings settings(path.c_str());
	int value = settings.value("general", "port", 0);
	std::string name = settings.value("general", "name", "");
	int hex = settings.value("general", "hex", 0x30);
	double price = settings.value("general", "price", 0.0);
	std::cout << "port: " << value << std::endl;
	std::cout << "name: " << name << std::endl;
	printf("hex: 0x%x\n", hex);
	std::cout << "price: " << price << std::endl;

	price = settings.value("system", "price", 0.0);
	std::cout << "system/price: " << price << std::endl;

    return 0;
}
