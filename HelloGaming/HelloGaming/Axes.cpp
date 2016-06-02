#include "pch.h"
#include "Axes.h"


Axes::~Axes(void)
{
}

void Axes::CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	DX::ThrowIfFailed(
		context->CreateSolidColorBrush(m_color, D2D1::BrushProperties(m_opacity), &m_pBrush)
		);
}

void Axes::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context, float panX, float panY, float scaleX, float scaleY)
{
	// y axis
	context->DrawLine( D2D1::Point2F(0.0f, (context->GetSize().height - panY)/scaleY), D2D1::Point2F(0.0f, -panY/scaleY), m_pBrush, m_lineThickness/scaleX);

	// x axis
	context->DrawLine( D2D1::Point2F( -panX/scaleX, 0.0f), D2D1::Point2F( (context->GetSize().width - panX)/scaleX, 0.0f), m_pBrush, m_lineThickness/scaleY);

}

