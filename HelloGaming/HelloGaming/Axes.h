#pragma once

#include "DirectXBase.h"

class Axes
{
	ID2D1SolidColorBrush *m_pBrush;
	float m_lineThickness;
	float m_opacity;
	D2D1::ColorF m_color;

public:
	Axes(D2D1::ColorF cr, float thickness, float opacity):m_color(cr), m_lineThickness(thickness), m_opacity(opacity){};
	~Axes(void);

	void CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);
	void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context, float panX, float panY, float scaleX, float scaleY);
};

