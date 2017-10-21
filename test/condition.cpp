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
#include <punica.h>

punica::PCondition cond_;
punica::PMutex mutex_;

void *threadCallback(void *pvoid)
{
	punica::PMutexLocker locker(mutex_);
	std::cout << pthread_self() << std::endl; 
	cond_.wait(mutex_);
	std::cout << "wakeup " << pthread_self() << std::endl;
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t self;
	for (int i = 0; i < 5; ++i) {
		pthread_create(&self, NULL, threadCallback, NULL);
	}
	sleep(1);

	cond_.wake();

	sleep(2);

	cond_.wakeAll();

	sleep(2);
    return 0;
}
