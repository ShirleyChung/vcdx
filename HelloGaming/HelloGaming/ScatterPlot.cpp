#include "pch.h"
#include "ScatterPlot.h"


ScatterPlot::ScatterPlot(float *x, float *y, float nodeSize, D2D1::ColorF nodeColor, NodeShape nodeShape, int count)
	:GraphVariable(x,y,count)
	,m_nodeColor(nodeColor)
	,m_halfNodeSize(nodeSize/2)
	,m_nodeShape(nodeShape)
{
	if(count){
		m_min_x = m_max_x = x[0];
		m_min_y = m_max_y = y[0];
		for( int i=0; i<count; ++i)
		{
			if (m_min_x > m_points[i].x) m_min_x = m_points[i].x;
			if (m_min_y > m_points[i].y) m_min_y = m_points[i].y;
			if (m_max_x < m_points[i].x) m_max_x = m_points[i].x;
			if (m_max_y < m_points[i].y) m_max_y = m_points[i].y;
		}
	}
}


ScatterPlot::~ScatterPlot(void)
{
}

void ScatterPlot::CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	DX::ThrowIfFailed( context->CreateSolidColorBrush(D2D1::ColorF(m_nodeColor), &m_brush) );
}

void ScatterPlot::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	context->DrawLine(D2D1::Point2F(m_min_x, m_min_y), D2D1::Point2F(m_max_x, m_min_y), m_brush); // x axis
	context->DrawLine(D2D1::Point2F(m_min_x, m_min_y), D2D1::Point2F(m_min_x, m_max_y), m_brush); // y axis

	switch(m_nodeShape){
	case NodeShape::Circle:
		for(int i=0; i<m_nodeCount; ++i)
		{
			context->FillEllipse(D2D1::Ellipse(m_points[i], m_halfNodeSize, m_halfNodeSize), m_brush);
		}
		break;
	case NodeShape::Square:
		for( int i=0; i<m_nodeCount; ++i)
		{
			context->FillRectangle(D2D1::RectF(m_points[i].x - m_halfNodeSize,
				m_points[i].y - m_halfNodeSize,
				m_points[i].x + m_halfNodeSize,
				m_points[i].y + m_halfNodeSize), m_brush);
		}
		break;
	default:
		break;
	}
}
