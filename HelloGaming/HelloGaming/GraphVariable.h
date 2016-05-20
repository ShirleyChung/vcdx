#pragma once

#include "DirectXBase.h"

class GraphVariable abstract
{
protected:
	D2D1_POINT_2F *m_points;
	int m_nodeCount;
	float m_minx, m_miny;

public:
	float GetMinX(){ return m_minx; }
	float GetMinY(){ return m_miny; }

	GraphVariable(float* x, float* y, int count);
	virtual ~GraphVariable(void);

	virtual void CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context) = 0;
	virtual void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context) = 0;
};

