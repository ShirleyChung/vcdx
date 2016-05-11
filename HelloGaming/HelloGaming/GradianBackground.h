#pragma once

#include "DirectXBase.h"

class GradianBackground
{
public:
	GradianBackground(D2D1_COLOR_F colors[], float stops[], int count);
	~GradianBackground(void);

	void CreateWindowSizeDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);

	void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);

private:
	int	m_count;
	float* m_stops;
	D2D1_COLOR_F *m_colors;
	D2D1_RECT_F m_rect;
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_linearGDBrush;
};

