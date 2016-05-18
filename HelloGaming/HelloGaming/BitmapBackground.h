#pragma once

#include "DirectXBase.h"

class BitmapBackground
{
public:
	BitmapBackground(void);
	~BitmapBackground(void);

	void CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context, IWICImagingFactory2 *wicFactory, LPCWSTR filename);

	void CreateWindowSizeDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);

private:
	ID2D1Bitmap *m_pBmp;
	D2D1_RECT_F m_rect;
};

