#pragma once

#include "graphvariable.h"

enum NodeShape{ Circle, Square};

class ScatterPlot :	public GraphVariable
{
	float m_halfNodeSize;
	D2D1::ColorF m_nodeColor;
	ID2D1SolidColorBrush* m_brush;
	NodeShape m_nodeShape;

public:
	ScatterPlot(float *x, float *y, float nodeSize, D2D1::ColorF nodeColor, NodeShape nodeShape, int count);
	~ScatterPlot(void);

	virtual void CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context) override;
	virtual void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context) override;
};

