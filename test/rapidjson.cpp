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
#include <stdio.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

void json_read();
void json_write();

/*
{
    "hello": "world",
    "t": true ,
    "f": false,
    "n": null,
    "i": 123,
    "pi": 3.1416,
    "a": [1, 2, 3, 4]
}
*/
void json_read()
{
		std::string json = "{\
    \"hello\": \"world\",\
    \"t\": true ,\
    \"f\": false,\
    \"n\": null,\
    \"i\": 123,\
    \"pi\": 3.1416,\
    \"a\": [1, 2, 3, 4]\
}";

	rapidjson::Document doc;
	rapidjson::ParseResult result = doc.Parse(json);
	if (!result) {
		printf("JSON parse error: %s (%lu)\n", rapidjson::GetParseError_En(result.Code()), result.Offset());
	}

	// read member
	
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    json_read();
    return 0;
}
