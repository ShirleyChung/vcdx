#pragma once

#include "DirectXBase.h"

// 這個類別會轉譯有彩色背景的簡單文字。
ref class GraphRenderer sealed : public DirectXBase
{
public:
	GraphRenderer();

	// DirectXBase 方法。
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	// 用於更新時間相依物件的方法。
	void Update(float timeTotal, float timeDelta);

	// 用於根據輸入事件變更文字位置的方法。
	void UpdateTextPosition(Windows::Foundation::Point deltaTextPosition);

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_blackBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> m_textLayout;
	DWRITE_TEXT_METRICS m_textMetrics;
	Windows::Foundation::Point m_textPosition;
	bool m_renderNeeded;
	int m_backgroundColorIndex;
};
