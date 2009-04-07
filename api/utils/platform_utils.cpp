/**
 * Appcelerator Kroll - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

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
#include "utils.h"

namespace kroll
{
	std::string PlatformUtils::GetFirstMACAddress()
	{
		NodeId id;
		try
		{
			PlatformUtils::GetNodeId(id);
		}
		catch(std::string e)
		{
			return e;
		}
		catch(...)
		{
			return "NoMACAddressFound";
		}
		char result[18];
		std::sprintf(result, "%02x:%02x:%02x:%02x:%02x:%02x",
			id[0],
			id[1],
			id[2],
			id[3],
			id[4],
			id[5]);
		return std::string(result);
	}

	std::string PlatformUtils::GetMachineId()
	{
		std::string id = GetOldStyleMachineId();
		if (!id.empty())
		{
			return id;
		}
		else
		{
			std::string MACAddress = PlatformUtils::GetFirstMACAddress();
			return DataUtils::HexMD5(MACAddress);
		}
	}

	std::string PlatformUtils::GetOldStyleMachineId()
	{
		std::string runtimeHome = FileUtils::GetDefaultRuntimeHomeDirectory();
		if (EnvironmentUtils::Has("KR_RUNTIME_HOME"))
			runtimeHome = EnvironmentUtils::Get("KR_RUNTIME_HOME");

		std::string product = std::string(".") + PRODUCT_NAME;
		std::transform(product.begin(), product.end(), product.begin(), tolower);
		std::string path = FileUtils::Join(runtimeHome.c_str(), product.c_str(), NULL);

		if (FileUtils::IsFile(path))
		{
			std::ifstream file(path.c_str());
			if (!file.bad() && !file.fail() && ! file.eof())
			{
				std::string line;
				std::getline(file, line);
				FileUtils::Trim(line);
				return line;
			}
		}
		return std::string();
	}
}
