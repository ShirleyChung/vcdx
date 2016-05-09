#include "pch.h"
#include "SolidBackground.h"

SolidBackground::SolidBackground(D2D1::ColorF color):color(color){}

void SolidBackground::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	context->Clear(color);
}