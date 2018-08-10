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
#ifndef _MDATAHANDLER_H_
#define _MDATAHANDLER_H_

#include <mpl/mqueue.h>
#include <mpl/mthread.h>
#include <mpl/mevent.h>

MPL_BEGIN_NAMESPACE

template <typename T>
class MDataHandler : public MThread
{
public:
    explicit MDataHandler() { }
    virtual ~MDataHandler() { stop(); }

	// push data
	void push(T &t) {
		_queue.push(t);
		_event.signal();
	}

	void push(const T& t) {
		_queue.push(t);
		_event.signal();
	}

	void stop() {
		interrupt();
		_event.wakeAll();
		MThread::stop();
	}
protected:
	virtual void handle(T &t) = 0;

	virtual void run() {
		while (!isInterrupted()) {
			_event.wait();
			while (!_queue.empty() && !isInterrupted()) {
				T t;
				if (_queue.get(t)) {
					handle(t);
				}
			} // while (!_queue.empty())
		} // while (!isInterrupted())
	} // run
private:
	MQueue<T> _queue;
	MEvent _event;
};

MPL_END_NAMESPACE

#endif /* _MDATAHANDLER_H_ */
