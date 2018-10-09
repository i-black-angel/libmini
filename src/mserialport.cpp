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
#include <mpl/mserialport.h>
#include <termios.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400,B1200, B300 };
uint32_t name_arr[] = { 115200, 57600, 38400, 19200, 9600, 4800, 2400,1200, 300 };

MSerialPort::MSerialPort()
	: _fd(-1)
{
}

MSerialPort::~MSerialPort()
{
	closeSerial();
}

ssize_t MSerialPort::readData(void *buf, size_t nbytes)
{
	return read(_fd, buf, nbytes);
}

ssize_t MSerialPort::writeData(const void *buf, size_t nbytes)
{
	tcflush(_fd, TCOFLUSH);
	return write(_fd, buf, nbytes);
}

int MSerialPort::openSerial(const char *file, uint32_t nbaudrate, 
				   uint32_t ndatabites, char nparity, uint32_t nstopbits)
{
	_fd = open(file, O_RDWR | O_NOCTTY);
	if (_fd > 0) {
		if ((baudrate(nbaudrate) != 0) ||
			(parity(ndatabites, nparity, nstopbits) != 0)) {
			closeSerial();
		}
	}
	return _fd;
}

void MSerialPort::closeSerial()
{
	if (_fd > 0) {
		close(_fd);
		_fd = -1;
	}
}

int MSerialPort::baudrate(uint32_t nbaudrate)
{
	int result = -1;
	struct termios opt;
	tcgetattr(_fd, &opt);
	for (int i = 0; i < 8; ++i) {
		if (nbaudrate == name_arr[i]) {
			tcflush(_fd, TCIOFLUSH);
			cfsetispeed(&opt, speed_arr[i]);
			cfsetospeed(&opt, speed_arr[i]);
			result = tcsetattr(_fd, TCSANOW, &opt);
			break;
		}
	}
	tcflush(_fd, TCIOFLUSH);
	return result;
}

int MSerialPort::parity(uint32_t databits, char nparity,  uint32_t stopbits)
{
	struct termios term;
	if (tcgetattr(_fd, &term) < 0) {
		return -1;
	}
	term.c_cflag &= ~(CSIZE);
	switch (databits) {
	case 8:
		term.c_cflag |= CS8;
		break;
	case 7:
		term.c_cflag |= CS7;
		break;
	case 6:
		term.c_cflag |= CS6;
		break;
	case 5:
		term.c_cflag |= CS5;
		break;
	default:
		term.c_cflag |= CS8;
		break;
	}

	switch (stopbits) {
	case 2:
		term.c_cflag |= CSTOPB;
		break;
	default:
		term.c_cflag &= ~CSTOPB;
		break;
	}

	switch (nparity) {
	case 'o':
	case 'O':
		term.c_cflag |= (PARODD | PARENB);
		term.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E':
		term.c_cflag |= PARENB;
		term.c_cflag &= ~PARODD;
		term.c_iflag |= INPCK;
		break;
	case 's':
	case 'S':
		term.c_cflag &= ~PARENB;
		term.c_cflag &= ~CSTOPB;
		term.c_iflag |= INPCK;
		break;
	case 'n':
	case 'N':
		term.c_cflag &= ~PARENB;
		term.c_iflag &= ~INPCK;
		break;
	default:
		term.c_cflag &= ~PARENB;
		term.c_iflag &= ~INPCK;
		break;

	}
	
	term.c_iflag &= ~(INLCR | ICRNL | BRKINT | IXON | IXOFF | IXANY);
	term.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	term.c_oflag &= ~OPOST;
	term.c_cflag |= (CLOCAL | CREAD);

	tcflush(_fd, TCIFLUSH);

	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 30;

	if (tcsetattr(_fd, TCSANOW, &term) < 0) {
		return -1;
	}
	return 0;
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
