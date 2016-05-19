#include "pch.h"
#include "ScatterPlot.h"


ScatterPlot::ScatterPlot(float *x, float *y, float nodeSize, D2D1::ColorF nodeColor, NodeShape nodeShape, int count)
	:GraphVariable(x,y,count)
	,m_nodeColor(0)
{

}


ScatterPlot::~ScatterPlot(void)
{
}

void ScatterPlot::CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{

}

void ScatterPlot::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{

}
