#pragma once

// 使 Win32 應用程式開發介面運作效能卓越的協助程式公用程式。
namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// 在這一行設定中斷點，以擷取 Win32 應用程式開發介面錯誤。
			throw Platform::Exception::CreateException(hr);
		}
	}
}
