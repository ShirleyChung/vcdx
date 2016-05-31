#include "pch.h"
#include "GraphRenderer.h"

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
	m_solidBackground = new SolidBackground(D2D1::ColorF::Green);

//	D2D1_COLOR_F colors[] = { D2D1::ColorF(ColorF::PaleGoldenrod), D2D1::ColorF(ColorF::PaleTurquoise), D2D1::ColorF(0.7f, 0.7f, 1.0f)  };
//	float stops[] = { 0.0f, 0.5f, 1.0f };

	const int count = 4, nodeCount = 500;

	D2D1_COLOR_F *colors = new D2D1_COLOR_F[count];
	float *stops = new float[count];

	for(int i=0; i<count; ++i)
	{
		colors[i] = D2D1::ColorF( 0.75f + (float)(rand()%192)/192.0f, 0.75f + (float)(rand()%192)/192.0f, 0.75f + (float)(rand()%192)/192.0f);
		stops[i] = (float)i/count;
	}

	m_graBackground = new GradianBackground(colors, stops, count);

	delete[] colors;
	delete[] stops;

	m_bmpBackground = new BitmapBackground();

	float *x = new float[nodeCount];
	float *y = new float[nodeCount];

	for(int i=0; i<nodeCount; ++i)
	{
		x[i] = (float)(rand()%200);
		y[i] = (float)(rand()%200);
	}

	m_graphVar = new ScatterPlot(x, y, 2.0f, D2D1::ColorF::BlueViolet, NodeShape::Circle, nodeCount);

	delete[] x;
	delete[] y;

	m_pAxes = new Axes(D2D1::ColorF::Black, 1.0, 1.0);
}

GraphRenderer::~GraphRenderer()
{
	delete m_solidBackground;
	delete m_graBackground;
	delete m_bmpBackground;
	delete m_graphVar;
	delete m_pAxes;
}

void GraphRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();
}

void GraphRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	m_bmpBackground->CreateDeviceDependentResources( m_d2dContext, m_wicFactory.Get(), L"space.jpg" );
	m_graphVar->CreateDeviceDependentResources(m_d2dContext);
}

void GraphRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();

	m_graBackground->CreateWindowSizeDependentResources(m_d2dContext);
	m_bmpBackground->CreateWindowSizeDependentResources(m_d2dContext);

	m_pan.X = 20.0f - m_graphVar->GetMinX();
	m_pan.Y =  m_d2dContext->GetSize().height/2 - m_graphVar->GetMinY();
}

void GraphRenderer::Update(float timeTotal, float timeDelta)
{
}

void GraphRenderer::Render()
{
	m_d2dContext->BeginDraw();

	/* 行動裝置螢幕方向 */
	m_d2dContext->SetTransform(m_orientationTransform2D);

	/* 背景 */
//	m_solidBackground->Render(m_d2dContext);
//	m_graBackground->Render(m_d2dContext);
	m_bmpBackground->Render(m_d2dContext);

	/* 圖表位置及方位 */
	Matrix3x2F scale = Matrix3x2F::Scale(1.0f, -1.0f, D2D1::Point2F(0.0f, 0.0f));
	Matrix3x2F translation = Matrix3x2F::Translation(m_pan.X, m_pan.Y);
	m_d2dContext->SetTransform(scale * translation* m_orientationTransform2D);

	/* 圖表 */
	m_graphVar->Render(m_d2dContext);

	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}
}

void GraphRenderer::PointerMoved(Windows::Foundation::Point point)
{
	m_pan.X +=point.X;
	m_pan.Y +=point.Y;
}