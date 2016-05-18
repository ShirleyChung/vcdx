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

	D2D1_COLOR_F colors[] = { D2D1::ColorF(ColorF::PaleGoldenrod),
		D2D1::ColorF(ColorF::PaleTurquoise), 
		D2D1::ColorF(0.7f, 0.7f, 1.0f)  };

	float stops[] = { 0.0f, 0.5f, 1.0f };

	m_graBackground = new GradianBackground(colors, stops, 3);
}

GraphRenderer::~GraphRenderer()
{
	delete m_solidBackground;
	delete m_graBackground;
}

void GraphRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();
}

void GraphRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();
}

void GraphRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();

	m_graBackground->CreateWindowSizeDependentResources(m_d2dContext);
}

void GraphRenderer::Update(float timeTotal, float timeDelta)
{
}

void GraphRenderer::Render()
{
	m_d2dContext->BeginDraw();

	m_d2dContext->SetTransform(m_orientationTransform2D);

//	m_solidBackground->Render(m_d2dContext);
	m_graBackground->Render(m_d2dContext);

	Matrix3x2F translation = Matrix3x2F::Translation(m_pan.X, m_pan.Y);
	m_d2dContext->SetTransform(translation* m_orientationTransform2D);

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