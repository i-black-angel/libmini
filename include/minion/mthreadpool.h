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
#ifndef _MTHREADPOOL_H_
#define _MTHREADPOOL_H_

#include <minion/mcoredef.h>
#include <minion/mmutex.h>
#include <minion/mthread.h>
#include <minion/mqueue.h>

MINION_BEGIN_NAMESPACE

class MTaskInterface
{
public:
    virtual ~MTaskInterface() { }
	virtual void run() = 0;
};

class MThreadPool
{
public:
	class MWorkThread : public MThread
	{
	public:
		MWorkThread(MThreadPool *pool) : _pool(pool) { }
		virtual ~MWorkThread() { }
	protected:
		virtual void run() {
			while (!isInterrupted()) {
				_pool->_event.wait();
				MTaskInterface *task = NULL;
				while (_pool->_queue.get(task) && !isInterrupted()) {
					task->run();
					if (NULL != task) { delete task; }
				}
			}
		} // void run()

		MThreadPool *_pool;
	}; // class MWorkThread
	
    explicit MThreadPool(int size) : _poolsize(size) {
		for (int i = 0; i < size; ++i) {
			MWorkThread *work = new MWorkThread(this);
			_threads.push_back(work);
		}
	}
    virtual ~MThreadPool() { stop(); }

	bool start() {
		for (int i = 0; i < _poolsize; ++i) {
			_threads[i]->start();
		}
		return true;
	}
	
	void stop() {
		for (int i = 0; i < _poolsize; ++i) {
			_threads[i]->interrupt();
		}
		// wake all threads up
		_event.wakeAll();
		for (int i = 0; i < _poolsize; ++i) {
			_threads[i]->join();
			delete _threads[i];
		}
		_threads.clear();
	}

	void push_back(MTaskInterface *task) {
		_queue.push(task);
		_event.signal();
	}

	friend class MWorkThread;
private:
	std::vector<MThread *> _threads;
	MQueue<MTaskInterface *> _queue;
	MEvent _event;
	int _poolsize;
};

MINION_END_NAMESPACE

#endif /* _MTHREADPOOL_H_ */
