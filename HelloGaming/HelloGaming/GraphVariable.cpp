#include "pch.h"
#include "GraphVariable.h"


GraphVariable::GraphVariable(float* x, float* y, int c)
	:m_nodeCount(c)
	,m_minx(x[0])
	,m_miny(y[0])
{
	m_points = new D2D1_POINT_2F[c];
	while(c--)
	{
		m_points[c] = D2D1::Point2F(x[c], y[c]);
		if (x[c] < m_minx) m_minx = x[c];
		if (y[c] < m_miny) m_miny = y[c];
	}
}


GraphVariable::~GraphVariable(void)
{
	delete[] m_points;
}
