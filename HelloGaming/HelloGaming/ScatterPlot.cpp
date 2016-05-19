#include "pch.h"
#include "ScatterPlot.h"


ScatterPlot::ScatterPlot(float *x, float *y, float nodeSize, D2D1::ColorF nodeColor, NodeShape nodeShape, int count)
	:GraphVariable(x,y,count)
	,m_nodeColor(nodeColor)
	,m_halfNodeSize(nodeSize/2)
	,m_nodeShape(nodeShape)
{}


ScatterPlot::~ScatterPlot(void)
{
}

void ScatterPlot::CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	DX::ThrowIfFailed( context->CreateSolidColorBrush(D2D1::ColorF(m_nodeColor), &m_brush) );
}

void ScatterPlot::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
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
