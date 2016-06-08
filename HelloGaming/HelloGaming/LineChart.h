#pragma once

#include "graphvariable.h"
class LineChart :	public GraphVariable
{
	ID2D1SolidColorBrush* m_solidBrush;
	float m_lineThickness;
	D2D1::ColorF m_color;
	
	void sortData();

public:
	LineChart(float* x, float* y, int count, D2D1::ColorF cr, float thickness);
	~LineChart(void);

	void SetLineThickness(float thick){ m_lineThickness = thick; };
	float GetLineThickness(){ return m_lineThickness; };

	virtual void CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context) override;
	virtual void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context) override;
};

