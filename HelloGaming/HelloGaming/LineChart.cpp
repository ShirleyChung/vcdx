#include "pch.h"
#include "LineChart.h"
#include <algorithm>

bool compare_points(D2D1_POINT_2F a, D2D1_POINT_2F b)
{
	return a.x < b.x;
}

bool compare_y(D2D1_POINT_2F a, D2D1_POINT_2F b)
{
	return a.y < b.y;
}

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
	std::vector<D2D1_POINT_2F> sortedNodes(m_points, m_points + m_nodeCount);
	std::stable_sort(sortedNodes.begin(), sortedNodes.end(), compare_points);

	int c = 0;
	for(std::vector<D2D1_POINT_2F>::iterator itor = sortedNodes.begin(); itor != sortedNodes.end(); itor++, c++)
	{
		m_points[c].x = (*itor).x;
		m_points[c].y = (*itor).y;
	}
	std::stable_sort(sortedNodes.begin(), sortedNodes.end(), compare_y);
	std::vector<D2D1_POINT_2F>::iterator itor = sortedNodes.begin();
	for(int i=0; i<m_nodeCount; ++i, ++itor)
	{
		m_points[i].y = (*itor).y;
	}
}

void LineChart::CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	DX::ThrowIfFailed( context->CreateSolidColorBrush(m_color, D2D1::BrushProperties(1.0f), &m_solidBrush) );
}

void LineChart::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	for( int i=0; i<m_nodeCount; ++i)
	{
		context->FillRectangle(
			D2D1::RectF(m_points[i].x - m_lineThickness, m_points[i].y, m_points[i].x + m_lineThickness, 0)
			, m_solidBrush);	
	}
}


