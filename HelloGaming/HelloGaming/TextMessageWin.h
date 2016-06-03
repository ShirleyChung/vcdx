#pragma once

#include "DirectXBase.h"
#include <list>

class TextMessageWin
{
	std::wstring m_fontFace, m_locale;
	float m_fontSize;
	D2D1_RECT_F m_lineRect;
	D2D1::ColorF m_textColor;

	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_textBrush;

	std::list<std::wstring> m_messages;
public:
	TextMessageWin(void);
	~TextMessageWin(void);

	void CreateDeviceIndependentResources(Microsoft::WRL::ComPtr<IDWriteFactory1> dwFactory);
	void CreateWindowSizeDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);

	void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);

	void AddMessage(const std::wstring& s){ m_messages.push_back(s); }
};

