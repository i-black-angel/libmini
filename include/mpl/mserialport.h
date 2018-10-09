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
#ifndef _MSERIALPORT_H_
#define _MSERIALPORT_H_

#include <mpl/mcoredef.h>

MPL_BEGIN_NAMESPACE

class MSerialPort
{
public:
    explicit MSerialPort();
    virtual ~MSerialPort();

	int openSerial(const char *name, uint32_t baudrate,
				   uint32_t databites, char parity, uint32_t stopbits);
	ssize_t readData(void *buf, size_t nbytes);
	ssize_t writeData(const void *buf, size_t nbytes);
	void closeSerial();

	int baudrate(uint32_t val);
	int parity(uint32_t databits, char nparity,  uint32_t stopbits);

	inline int fd() const { return _fd; }
private:
	int _fd;
};

MPL_END_NAMESPACE

#endif /* _MSERIALPORT_H_ */
