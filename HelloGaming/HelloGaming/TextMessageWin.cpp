#include "pch.h"
#include "TextMessageWin.h"


TextMessageWin::TextMessageWin(void)
	:m_fontFace(L"·s²Ó©úÅé")
	,m_locale(L"zh-TW")
	,m_fontSize(14.0f)
	,m_lineRect(D2D1::RectF(0,30,600,32))
	,m_textColor(D2D1::ColorF::White)
{
	SetRect(m_lineRect);
}


TextMessageWin::~TextMessageWin(void)
{
}

void TextMessageWin::CreateDeviceIndependentResources(Microsoft::WRL::ComPtr<IDWriteFactory1> dwFactory)
{
	DX::ThrowIfFailed( 
		dwFactory->CreateTextFormat(m_fontFace.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL, m_fontSize, m_locale.c_str(), &m_textFormat)
		);
}

void TextMessageWin::CreateWindowSizeDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	DX::ThrowIfFailed( context->CreateSolidColorBrush(m_textColor, &m_textBrush)	 );
}

void TextMessageWin::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	Pane::Render(context);

	context->DrawText( m_messages.back().c_str(), m_messages.back().length(), m_textFormat.Get(), m_lineRect, m_textBrush.Get());
}
