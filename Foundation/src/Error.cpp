//
// Error.cpp
//
// $Id: //poco/1.4/Foundation/src/Error.cpp#3 $
//
// Library: Foundation
// Package: Core
// Module:  Error
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Foundation.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/Error.h"
#include <string>
#include <string.h>


namespace Poco {


#ifdef POCO_OS_FAMILY_WINDOWS

	std::string Error::getMessage(DWORD errorCode)
	{
		std::string errMsg;
		DWORD dwFlg = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	#if defined(POCO_WIN32_UTF8) && !defined(POCO_NO_WSTRING)
		LPWSTR lpMsgBuf = 0;
		if (FormatMessageW(dwFlg, 0, errorCode, 0, (LPWSTR) & lpMsgBuf, 0, NULL))
			UnicodeConverter::toUTF8(lpMsgBuf, errMsg);
	#else
		LPTSTR lpMsgBuf = 0;
		if (FormatMessageA(dwFlg, 0, errorCode, 0, (LPTSTR) & lpMsgBuf, 0, NULL))
			errMsg = lpMsgBuf;
	#endif
		LocalFree(lpMsgBuf);
		return errMsg;
	}

#else

	std::string Error::getMessage(int errorCode)
	{
		/* Reentrant version of `strerror'.
		   There are 2 flavors of `strerror_r', GNU which returns the string
		   and may or may not use the supplied temporary buffer and POSIX one
		   which fills the string into the buffer.
		   To use the POSIX version, -D_XOPEN_SOURCE=600 or -D_POSIX_C_SOURCE=200112L
		   without -D_GNU_SOURCE is needed, otherwise the GNU version is
		   preferred.
		*/
#if defined _GNU_SOURCE && !POCO_ANDROID
		char errmsg[256] = "";
		return std::string(strerror_r(errorCode, errmsg, 256));
#elif (_XOPEN_SOURCE >= 600) || POCO_ANDROID
		char errmsg[256] = "";
		strerror_r(errorCode, errmsg, 256);
		return errmsg;
#else
		return std::string(strerror(errorCode));
#endif
	}

#endif


} // namespace Poco
