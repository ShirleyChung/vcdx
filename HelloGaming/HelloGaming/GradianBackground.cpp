#include "pch.h"
#include "GradianBackground.h"


GradianBackground::GradianBackground(D2D1_COLOR_F colors[], float stops[], int count)
{
	m_count = count;
	m_colors = new D2D1_COLOR_F[count];
	m_stops = new float[count];
	while(count-- > 0)
	{
		m_colors[count] = colors[count];
		m_stops[count] = stops[count];
	}
}


GradianBackground::~GradianBackground(void)
{
	delete[] m_colors;
	delete[] m_stops;
}

void GradianBackground::CreateWindowSizeDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	D2D1_GRADIENT_STOP *gdStops = new D2D1_GRADIENT_STOP[m_count];
	for(int i=0; i<m_count; ++i)
	{
		gdStops[i].color = m_colors[i];
		gdStops[i].position = m_stops[i];
	}
	ID2D1GradientStopCollection *gdStopsColletion;
	DX::ThrowIfFailed(
		context->CreateGradientStopCollection(gdStops, m_count, &gdStopsColletion)
		);

	m_rect = D2D1::RectF(0, 0, context->GetSize().width, context->GetSize().height);

	DX::ThrowIfFailed(
		context->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(D2D1::Point2F(0,0), D2D1::Point2F(m_rect.right, m_rect.bottom)),
			gdStopsColletion,
			&m_linearGDBrush
			)
		);
}

void GradianBackground::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	context->FillRectangle(m_rect, m_linearGDBrush.Get());
}
