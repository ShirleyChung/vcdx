#include "pch.h"
#include "LineChart.h"


LineChart::LineChart(float* x, float* y, int count, D2D1::ColorF cr, float thickness)
	:GraphVariable(x,y,count), m_color(cr), m_lineThickness(thickness)
{
	sortData();
}


LineChart::~LineChart(void)
{
}

void LineChart::sortData()
{

}

void LineChart::CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{

}

void LineChart::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{

}


