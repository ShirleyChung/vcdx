#pragma once

#include "DirectXBase.h"

#include "SolidBackground.h"
#include "GradianBackground.h"
#include "BitmapBackground.h"
#include "ScatterPlot.h"
#include "Axes.h"
#include "TextMessageWin.h"
#include "LineChart.h"

// 這個類別會轉譯有彩色背景的簡單文字。
ref class GraphRenderer sealed : public DirectXBase
{
public:
	GraphRenderer();
	virtual ~GraphRenderer();

	// DirectXBase 方法。
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	// 用於更新時間相依物件的方法。
	void Update(float timeTotal, float timeDelta);

	// 滑鼠移上去的事件
	void PointerMoved(Windows::Foundation::Point point);

private:
	Windows::Foundation::Point m_pan, m_mousePan;
	std::shared_ptr<SolidBackground> m_solidBackground;
	std::shared_ptr<GradianBackground> m_graBackground;
	std::shared_ptr<BitmapBackground> m_bmpBackground;
	std::shared_ptr<TextMessageWin> m_msgWin;
	std::shared_ptr<Axes> m_pAxes;

	std::shared_ptr<GraphVariable> m_graphVar;
	std::shared_ptr<LineChart> m_lineChart;

	Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> m_rectangle;
	Microsoft::WRL::ComPtr<ID2D1EllipseGeometry> m_ellipse;
	Microsoft::WRL::ComPtr<ID2D1RoundedRectangleGeometry> m_roundRect;

	// 顯示FPS
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_textBrush;

	float m_timeDelta, m_timeTotal;

internal:
	std::shared_ptr<LineChart> GetLineChart(){ return m_lineChart; }
};
