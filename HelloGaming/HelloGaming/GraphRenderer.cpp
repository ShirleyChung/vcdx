#include "pch.h"
#include "GraphRenderer.h"
#include <string>

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

GraphRenderer::GraphRenderer()
{
	m_solidBackground = std::make_shared<SolidBackground>(D2D1::ColorF::Green);

//	D2D1_COLOR_F colors[] = { D2D1::ColorF(ColorF::PaleGoldenrod), D2D1::ColorF(ColorF::PaleTurquoise), D2D1::ColorF(0.7f, 0.7f, 1.0f)  };
//	float stops[] = { 0.0f, 0.5f, 1.0f };

	const int count = 4, nodeCount = 30;

	D2D1_COLOR_F *colors = new D2D1_COLOR_F[count];
	float *stops = new float[count];

	for(int i=0; i<count; ++i)
	{
		colors[i] = D2D1::ColorF( 0.75f + (float)(rand()%192)/192.0f, 0.75f + (float)(rand()%192)/192.0f, 0.75f + (float)(rand()%192)/192.0f);
		stops[i] = (float)i/count;
	}

	m_graBackground = std::make_shared<GradianBackground>(colors, stops, count);

	delete[] colors;
	delete[] stops;

	m_bmpBackground = std::make_shared<BitmapBackground>();

	float *x = new float[nodeCount];
	float *y = new float[nodeCount];

	for(int i=0; i<nodeCount; ++i)
	{
		x[i] = (float)(rand()%200);
		y[i] = (float)(rand()%200);
	}

	m_graphVar = std::make_shared<ScatterPlot>(x, y, 2.0f, D2D1::ColorF::BlueViolet, NodeShape::Circle, nodeCount);

	m_lineChart = std::make_shared<LineChart>(x, y, nodeCount, D2D1::ColorF::Blue, 5.0f);

	delete[] x;
	delete[] y;

	m_pAxes = std::make_shared<Axes>(D2D1::ColorF::Black, 1.0f, 1.0f);
	m_msgWin = std::make_shared<TextMessageWin>();

	m_msgWin->AddMessage(L"Hello! I'm here!");
}

GraphRenderer::~GraphRenderer()
{
//	delete m_solidBackground;
//	delete m_graBackground;
//	delete m_bmpBackground;
//	delete m_graphVar;
//	delete m_pAxes;
//	delete m_lineChart;
}

void GraphRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

	DX::ThrowIfFailed( 
		m_dwriteFactory->CreateTextFormat(L"新細明體", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"zh-TW", &m_textFormat)
		);

	m_msgWin->CreateDeviceIndependentResources(m_dwriteFactory);
}

void GraphRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	m_bmpBackground->CreateDeviceDependentResources( m_d2dContext, m_wicFactory.Get(), L"space.jpg" );
	m_graphVar->CreateDeviceDependentResources(m_d2dContext);
	m_lineChart->CreateDeviceDependentResources(m_d2dContext);
	m_pAxes->CreateDeviceDependentResources(m_d2dContext);

	m_msgWin->CreateDeviceDependentResources(m_d2dContext, m_wicFactory.Get(), L"");
}

void GraphRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(ColorF(ColorF::White), &m_textBrush)
		);

	m_msgWin->CreateWindowSizeDependentResources(m_d2dContext);

	m_graBackground->CreateWindowSizeDependentResources(m_d2dContext);
	m_bmpBackground->CreateWindowSizeDependentResources(m_d2dContext);

	m_pan.X = 20.0f - m_graphVar->GetMinX();
	m_pan.Y =  m_d2dContext->GetSize().height/2 - m_graphVar->GetMinY();
}

void GraphRenderer::Update(float timeTotal, float timeDelta)
{
	static int c = -1;
	if (c++ % 15 == 0) // 16frames更新一次
	{
		m_timeTotal = timeTotal;
		m_timeDelta = timeDelta;
	}
}

void GraphRenderer::Render()
{
	m_d2dContext->BeginDraw();

	Matrix3x2F reset = Matrix3x2F::Identity();
	Matrix3x2F translation = Matrix3x2F::Translation(m_pan.X, m_pan.Y);
	Matrix3x2F mouse_pan = Matrix3x2F::Translation(m_mousePan.X, m_mousePan.Y);
	Matrix3x2F scale = Matrix3x2F::Scale(1.0f, -1.0f, D2D1::Point2F(0.0f, 0.0f));

	/* 行動裝置螢幕方向 */
	m_d2dContext->SetTransform(m_orientationTransform2D);

	/* 背景 */
//	m_solidBackground->Render(m_d2dContext);
//	m_graBackground->Render(m_d2dContext);
	m_bmpBackground->Render(m_d2dContext);

	/* FPS */
	std::wstring s = std::wstring(L"Total Time:") + std::to_wstring(m_timeTotal);
	s += L"s, FPS:" + std::to_wstring( (int)(0.5f + 1.0f/m_timeDelta) ) + L" frames/sec";
	m_d2dContext->DrawText( s.c_str(), s.length(), m_textFormat.Get(), D2D1::RectF(0,0,600,32), m_textBrush.Get());

	/* messages */
	m_d2dContext->SetTransform(mouse_pan* m_orientationTransform2D);
	m_msgWin->Render(m_d2dContext);

	m_d2dContext->SetTransform(scale * translation* m_orientationTransform2D);

	/* 坐標軸 */
	m_pAxes->Render(m_d2dContext, m_pan.X, m_pan.Y, 1.0f, -1.0f);

	/* 圖表 */
	m_graphVar->Render(m_d2dContext);

	m_lineChart->Render(m_d2dContext);

	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}
}

void GraphRenderer::PointerMoved(Windows::Foundation::Point point)
{
	m_mousePan.X +=point.X;
	m_mousePan.Y +=point.Y;
}