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
#include <mpl/merror.h>

#ifndef M_OS_WIN
#include <termios.h>
#endif

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

#ifdef M_OS_WIN

MSerialPort::MSerialPort()	
{
	_hComm = INVALID_HANDLE_VALUE;
}

MSerialPort::~MSerialPort()
{
	closeSerial();
}

ssize_t MSerialPort::readData(void *buf, size_t nbytes)
{
	DWORD read_bytes = 0;
	if (ReadFile(_hComm, buf, nbytes, &read_bytes, NULL))
		return read_bytes;
	return -1;
}

ssize_t MSerialPort::writeData(const void *buf, size_t nbytes)
{
	DWORD written = 0;
	PurgeComm(_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
	if (WriteFile(_hComm, buf, nbytes, &written, NULL))
		return written;
	return -1;
}

int MSerialPort::openSerial(const char *file, uint32_t nbaudrate, 
	uint32_t ndatabites, char nparity, uint32_t nstopbits)
{
	_hComm = CreateFile(file,            /* Device name, e.g. COM1, COM2... */
		GENERIC_READ | GENERIC_WRITE,    /* Read and write */ 
		0,                               /* Shared mode, 0 means not share */
		NULL,                            /* SecurityAttribute */
		OPEN_EXISTING,                   /* Device must be existing, otherwise failed */
		0,
		0);

	if (_hComm == INVALID_HANDLE_VALUE) {
		printf("CreateFile failed: %s\n", error().c_str());
		return -1; 
	}
	if ((baudrate(nbaudrate) != 0) ||
		(parity(ndatabites, nparity, nstopbits) != 0)) {
		closeSerial();
		return -1;
	}

	PurgeComm(_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	return 0;
}

void MSerialPort::closeSerial()
{
	if (_hComm!= INVALID_HANDLE_VALUE) {
		CloseHandle(_hComm);
		_hComm = INVALID_HANDLE_VALUE;
	}
}

int MSerialPort::baudrate(uint32_t nbaudrate)
{
	DCB dcb;
	if (!GetCommState(_hComm, &dcb)) {
		printf("MSerialPort::baudrate GetCommState failed: %s\n", error().c_str());
		return -1;
	}

	// Setting BaudRate
	dcb.BaudRate = nbaudrate;

	if (!SetCommState(_hComm, &dcb)) {
		printf("MSerialPort::baudrate SetCommState failed: %s\n", error().c_str());
		return -1;
	}		
	return 0;	
}

int MSerialPort::parity(uint32_t databits, char nparity,  uint32_t stopbits)
{
	DCB dcb;
	if (!GetCommState(_hComm, &dcb)) {
		printf("MSerialPort::parity GetCommState failed: %s\n", error().c_str());
		return -1;
	}

	dcb.fBinary = TRUE;			// Binary mode, must be true
	switch (databits)           // DataBit, 4-8
	{
	case 8:
	case 7:
	case 6:
	case 5:
	case 4:
		dcb.ByteSize = databits;
		break;	
	default:
		dcb.ByteSize = 8;
		break;
	}
	
	switch (stopbits)           // StopBit
	{
	case 2:
		dcb.StopBits = TWOSTOPBITS;
		break;
	//case 1:
	//	dcb.StopBits = ONE5STOPBITS;
	//	break;
	default:
		dcb.StopBits = ONESTOPBIT;
		break;
	}

	switch (nparity) {
	case 'o':
	case 'O':
		dcb.fParity = TRUE;
		dcb.Parity = ODDPARITY;
		break;
	case 'e':
	case 'E':
		dcb.fParity = TRUE;
		dcb.Parity = EVENPARITY;
		break;
	case 's':
	case 'S':
		dcb.fParity = TRUE;
		dcb.Parity = SPACEPARITY;
		break;
	case 'n':
	case 'N':
		dcb.fParity = FALSE;
		dcb.Parity = NOPARITY;
		break;
	default:
		dcb.fParity = FALSE;
		dcb.Parity = NOPARITY;
		break;

	}

	dcb.fOutxCtsFlow = FALSE;	// CTS Flow
	dcb.fOutxDsrFlow = FALSE;	// DST Flow
	dcb.fDtrControl = DTR_CONTROL_ENABLE; // DTR Control
	dcb.fDsrSensitivity = FALSE;
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fOutX = FALSE;		
	dcb.fInX = FALSE;		
	dcb.fErrorChar = FALSE;		
	dcb.fNull = FALSE;			
	dcb.fRtsControl = RTS_CONTROL_ENABLE; //
	dcb.fAbortOnError = FALSE;

	if (!SetCommState(_hComm, &dcb)) {
		printf("MSerialPort::parity SetCommState failed: %s\n", error().c_str());
		return -1;
	}		

	// Setup Comm
	SetCommMask(_hComm, EV_RXCHAR);
	SetupComm(_hComm, 16384, 16384);

	// Configure COM Serial timeout
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts(_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout  = MAXDWORD;
	CommTimeOuts.ReadTotalTimeoutMultiplier  = 0;
	CommTimeOuts.ReadTotalTimeoutConstant  = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier  = 10;
	CommTimeOuts.WriteTotalTimeoutConstant  = 1000;

	if (!SetCommTimeouts(_hComm, &CommTimeOuts)) {
		printf("MSerialPort::parity SetCommTimeouts failed: %s\n", error().c_str());
		return -1;
	}
	return 0;
}

#else /* M_OS_LINUX */

int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400,B1200, B300 };
uint32_t name_arr[] = { 115200, 57600, 38400, 19200, 9600, 4800, 2400,1200, 300 };

MSerialPort::MSerialPort()	
{
	_fd = -1;
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
#endif

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
