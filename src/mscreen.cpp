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
#ifdef _MSC_VER
#include <atlimage.h>
#endif
#include <mpl/mscreen.h>

MPL_BEGIN_NAMESPACE

bool grabWindow(const std::string &imageFile)
{
#ifdef M_OS_WIN
	HDC hdcSrc = GetDC(NULL);
	int nBitPerPixel = GetDeviceCaps(hdcSrc, BITSPIXEL);
	int nWidth = GetDeviceCaps(hdcSrc, HORZRES);
	int nHeight = GetDeviceCaps(hdcSrc, VERTRES);
	CImage image;
	image.Create(nWidth, nHeight, nBitPerPixel);
	BitBlt(image.GetDC(), 0, 0, nWidth, nHeight, hdcSrc, 0, 0, SRCCOPY);
	ReleaseDC(NULL, hdcSrc);
	image.ReleaseDC();
	return S_OK == image.Save(imageFile.c_str(), Gdiplus::ImageFormatPNG);
#endif
	return true;
}

bool grab(int x, int y, int nWidth, int nHeight, const std::string &imageFile)
{
#ifdef M_OS_WIN
	HDC hdcSrc = GetDC(NULL);
	int nBitPerPixel = GetDeviceCaps(hdcSrc, BITSPIXEL);
	CImage image;
	image.Create(nWidth, nHeight, nBitPerPixel);
	StretchBlt(image.GetDC(), 0, 0, nWidth, nHeight, hdcSrc, x, y, nWidth, nHeight, SRCCOPY);
	ReleaseDC(NULL, hdcSrc);
	image.ReleaseDC();
	return S_OK == image.Save(imageFile.c_str(), Gdiplus::ImageFormatPNG);  
#endif
	return true;
}

void screenClick(int x, int y)
{
#ifdef M_OS_WIN
	HDC hdcSrc = GetDC(NULL);
	int width = GetDeviceCaps(hdcSrc, HORZRES);
	int height = GetDeviceCaps(hdcSrc, VERTRES);
	ReleaseDC(NULL, hdcSrc);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,
				x * 65536 / width,
				y * 65536 / height, 0, 0);
	Sleep(20);  // click interval, unit: ms
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,
				0, 0, 0, 0);
#endif
}

MPL_END_NAMESPACE