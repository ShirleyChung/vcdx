﻿#include "pch.h"
#include "SimpleTextRenderer.h"

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

static const ColorF BackgroundColors[] = 
{
	ColorF::CornflowerBlue,
	ColorF::Orchid,
	ColorF::DeepPink,
	ColorF::Red,
	ColorF::DarkOrange,
	ColorF::Gold,
	ColorF::GreenYellow,
	ColorF::Aqua
};

SimpleTextRenderer::SimpleTextRenderer() :
	m_renderNeeded(true), //重繪旗標
	m_backgroundColorIndex(0), // 背景顏色
	m_textPosition(0.0f, 0.0f) // 文字位置
{
}

void SimpleTextRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			42.0f,
			L"en-US",
			&m_textFormat
			)
		);

	DX::ThrowIfFailed(
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)
		);
}

void SimpleTextRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			ColorF(ColorF::Black),
			&m_blackBrush
			)
		);

	Platform::String^ text = "Hello, DirectX!";

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextLayout(
			text->Data(),
			text->Length(),
			m_textFormat.Get(),
			700, // maxWidth。
			1000, // maxHeight。
			&m_textLayout
			)
		);

	DX::ThrowIfFailed(
		m_textLayout->GetMetrics(&m_textMetrics)
		);
}

void SimpleTextRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();

	// 在這裡加入程式碼，建立與視窗大小相依的物件。
}

void SimpleTextRenderer::Update(float timeTotal, float timeDelta)
{
	(void) timeTotal; // 未使用的參數。
	(void) timeDelta; // 未使用的參數。

	// 在這裡加入程式碼，更新與時間相依的物件。
}

void SimpleTextRenderer::Render()
{
	m_d2dContext->BeginDraw();

	m_d2dContext->Clear(ColorF(BackgroundColors[m_backgroundColorIndex]));

	// 放置轉譯的文字。
	Matrix3x2F translation = Matrix3x2F::Translation(
		m_windowBounds.Width / 2.0f - m_textMetrics.widthIncludingTrailingWhitespace / 2.0f + m_textPosition.X,
		m_windowBounds.Height / 2.0f - m_textMetrics.height / 2.0f + m_textPosition.Y
		);

	// 注意，此處會對 m_orientationTransform2D 矩陣執行後乘運算，
	// 以便能夠正確設定文字方向，以符合顯示方向。
	// 這個後乘運算步驟必須用於針對交換鏈結的
	// 目標點陣圖執行的任何繪製呼叫。若是其他目標的繪製呼叫，
	// 則不應該套用這個轉換。
	Matrix3x2F move = Matrix3x2F::Translation(m_pan.X, m_pan.Y);

	m_d2dContext->SetTransform(translation * m_orientationTransform2D*move);

	m_d2dContext->DrawTextLayout(
		Point2F(0.0f, 0.0f),
		m_textLayout.Get(),
		m_blackBrush.Get(),
		D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
		);

	// 忽略 D2DERR_RECREATE_TARGET。這個錯誤表示裝置
	// 已遺失。將在下次呼叫 Present 時處理。
	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	m_renderNeeded = false;
}

void SimpleTextRenderer::UpdateTextPosition(Point deltaTextPosition)
{
	m_textPosition.X += deltaTextPosition.X;
	m_textPosition.Y += deltaTextPosition.Y;
}

void SimpleTextRenderer::BackgroundColorNext()
{
	m_backgroundColorIndex++;
	if (m_backgroundColorIndex >= ARRAYSIZE(BackgroundColors))
	{
		m_backgroundColorIndex = 0;
	}
}

void SimpleTextRenderer::BackgroundColorPrevious()
{
	m_backgroundColorIndex--;
	if (m_backgroundColorIndex < 0)
	{
		m_backgroundColorIndex = ARRAYSIZE(BackgroundColors) - 1;
	}
}

void SimpleTextRenderer::SaveInternalState(IPropertySet^ state)
{
	if (state->HasKey("m_backgroundColorIndex"))
	{
		state->Remove("m_backgroundColorIndex");
	}
	if (state->HasKey("m_textPosition"))
	{
		state->Remove("m_textPosition");
	}
	state->Insert("m_backgroundColorIndex", PropertyValue::CreateInt32(m_backgroundColorIndex));
	state->Insert("m_textPosition", PropertyValue::CreatePoint(m_textPosition));
}

void SimpleTextRenderer::LoadInternalState(IPropertySet^ state)
{
	if (state->HasKey("m_backgroundColorIndex") && state->HasKey("m_textPosition"))
	{
		m_backgroundColorIndex = safe_cast<IPropertyValue^>(state->Lookup("m_backgroundColorIndex"))->GetInt32();
		m_textPosition = safe_cast<IPropertyValue^>(state->Lookup("m_textPosition"))->GetPoint();
	}
}

void SimpleTextRenderer::PointerMoved(Windows::Foundation::Point point)
{
	m_pan.X +=point.X;
	m_pan.Y +=point.Y;
}