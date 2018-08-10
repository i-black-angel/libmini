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
#ifndef _MQUEUE_H_
#define _MQUEUE_H_

#include <mpl/mcoredef.h>
#include <mpl/mmutex.h>

MPL_BEGIN_NAMESPACE

template <typename T>
class MQueue : public std::deque<T>
{
public:
    MQueue() { }
    virtual ~MQueue() { }
public:
	void push(T &t) {
		MScopedLock locker(_mutex);
		std::deque<T>::push_back(t);
	}

	void push(const T& t) {
		MScopedLock locker(_mutex);
		std::deque<T>::push_back(t);
	}

	bool get(T &t) {
		MScopedLock locker(_mutex);
		if (!std::deque<T>::empty()) {
			t = std::deque<T>::front();
			std::deque<T>::pop_front();
			return true;
		}
		return false;
	}

	void clear() {
		MScopedLock locker(_mutex);
		std::deque<T>::clear();
	}
private:
	MMutex _mutex;
};

MPL_END_NAMESPACE

#endif /* _MQUEUE_H_ */
