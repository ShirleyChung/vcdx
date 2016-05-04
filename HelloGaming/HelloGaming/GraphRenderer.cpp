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

	// 在這裡加入程式碼，建立與視窗大小相依的物件。
}

void GraphRenderer::Update(float timeTotal, float timeDelta)
{
}

void GraphRenderer::Render()
{
}

void GraphRenderer::PointerMoved(Windows::Foundation::Point point)
{
	m_pan.X +=point.X;
	m_pan.Y +=point.Y;
}