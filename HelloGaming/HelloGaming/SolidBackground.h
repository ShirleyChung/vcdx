#pragma once
#include "DirectXBase.h"

class SolidBackground{
private:
	D2D1::ColorF color;

public:
	SolidBackground(D2D1::ColorF color);

	void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context);
};