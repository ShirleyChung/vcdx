#pragma once

#include "DirectXBase.h"

class Pane
{
protected:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_bkBrush;
	D2D1::ColorF m_bkcolor;
	ID2D1Bitmap *m_pBmp;
	D2D1_RECT_F m_rect;

public:
	Pane(void);
	~Pane(void);

	virtual void SetRect(D2D1_RECT_F rt);

	virtual void CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context, IWICImagingFactory2 *wicFactory, LPCWSTR filename);
	virtual void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);
};

