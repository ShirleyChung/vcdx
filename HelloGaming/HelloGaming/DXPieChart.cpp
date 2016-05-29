#include "pch.h"
#include "DXPieChart.h"
#include <math.h>

#define M_PI acos(-1.0f)

#define LIGHT_COLOR RGB(210, 210, 210)
#define DARK_COLOR RGB(10, 10, 10)

D2D1_POINT_2F CenterPoint(const D2D1_RECT_F& rt)
{
	return D2D1::Point2F( (rt.left+rt.right)/2, (rt.top+rt.bottom)/2 );
}

void OffsetRect(Rect& rt, float x, float y)
{
	rt.left += x; rt.right += x;
	rt.top += y; rt.bottom += y;
}


DXPieChart::DXPieChart()
:m_bgColor(RGB(255,0,0))
,m_radius(1)
,m_pFont(NULL)
,m_pBorderPen(NULL)
{
	m_pieColor.push_back(RGB(255,0,0));
	m_pieColor.push_back(RGB(0,255,0));
	m_pieColor.push_back(RGB(0,0,255));
/*
 	m_pizza["1011"] = (Pizza("1011", 1, RGB(255,1,10)));
 	m_pizza["1012"] = (Pizza("1012", 1.5, RGB(255,255,1)));
 	m_pizza["1013"] = (Pizza("1013", 0.5, RGB(255,1,1)));
 	m_pizza["2011"] = (Pizza("2011", 3, RGB(50,1,255)));
 	m_pizza["3011"] = (Pizza("3011", 2.1, RGB(255,50,1)));
 	m_pizza["4011"] = (Pizza("4011", 2, RGB(50,1,1)));
*/
 }

DXPieChart::~DXPieChart()
{
	
}

#define MARGIN 20
void DXPieChart::SetRect(const Rect &rt)
{
	m_rt = D2D1::RectF( rt.left + MARGIN, rt.top + MARGIN, rt.right - MARGIN, rt.bottom - MARGIN);

	m_ct = CenterPoint(m_rt);
	BOOL isWide = (m_rt.right-m_rt.left) > (m_rt.bottom - m_rt.top);
	m_radius = isWide? (m_rt.bottom - m_rt.top)/2: (m_rt.right - m_rt.top)/2;

	_resetPoints();
}

void DXPieChart::_resetPoints()
{
	PIZZAMAP::iterator itor = m_pizza.begin(), itor_end = m_pizza.end();
	float sum = 0, as = 0, ae = 0, txtPos = 0;

	do
	{
		sum += itor->second.value;
	}
	while(++itor != itor_end);

	itor = m_pizza.begin();
	do
	{
		itor->second.ratio = itor->second.value / sum;
		as = ae;
		ae += itor->second.ratio * 2 * M_PI;

		Rect pieRt = D2D1::RectF( m_ct.x - m_radius, m_ct.y - m_radius, m_ct.x + m_radius, m_ct.y + m_radius );
		itor->second.SetPie( pieRt, as, ae);
	}
	while(++itor != itor_end);
}

BOOL _isDark(COLORREF cr)
{
	int totalColor = ((int)(BYTE)cr) + ((int)(BYTE)(cr>>8)) + ((int)(BYTE)(cr>>16));
	return totalColor < (255);
}

void DXPieChart::SaveContext(DC& dc)
{
	//Brush brush(m_bgColor);

	//m_pOldBr = dc.SelectObject(&brush);
	//m_pOrgFont = dc.SelectObject(m_pFont);
	//m_pOrgPen = dc.SelectObject(m_pBorderPen);
	//m_bkMode = dc.SetBkMode(TRANSPARENT);
	//m_oldTxCr = dc.SetTextColor(DARK_COLOR);

}

void DXPieChart::RestoreContext(DC& dc)
{
/*
	dc.SetTextColor(m_oldTxCr);
	dc.SetBkMode(m_bkMode);
	dc.SelectObject(m_pOldBr);
	dc.SelectObject(m_pOrgFont);
	dc.SelectObject(m_pOrgPen);
*/
}

void DXPieChart::Draw(DC& dc)
{
	SaveContext(dc);

	PIZZAMAP::iterator itor_begin = m_pizza.begin(), itor_end = m_pizza.end();
	//draw pie
	PIZZAMAP::iterator itor = itor_begin;
	while(itor != itor_end)
	{
		itor->second.DrawPie(dc);
		itor++;
	}
	//draw text
	itor = itor_begin;
	while(itor != itor_end)
	{
		itor->second.DrawLabel(dc);
		itor++;
	}

	RestoreContext(dc);
}

string DXPieChart::PointWhichPie(Point pt)
{
	string focusKey;
	PIZZAMAP::iterator itor = m_pizza.begin(), itor_end = m_pizza.end();
	while(itor != itor_end)
	{
		itor->second.large = itor->second.CheckPoint(pt);
		if (itor->second.large)
			focusKey = itor->first;
	}

	return focusKey;
}


void DXPieChart::GetAllPieColor(PIECOLOR& map)
{
	map.clear();

	PIZZAMAP::iterator itor = m_pizza.begin(), itor_end = m_pizza.end();
	while(itor != itor_end)
	{
		map[itor->first] = itor->second.color;
		itor++;
	}
}

/********************/
/*	Pizza struct	*/
/********************/

float getAngle(Size v)
{
	if (!v.width) return (v.height>0)? M_PI/2: - M_PI/2;
	
	/* dimension 2,3 : x<0, 4: x>0, y<0 */
	return atan(v.height/v.width) + ( (v.width<0)? M_PI: (v.height<0)? 2*M_PI: 0 );
}

#define EL 10.0f
void Pizza::SetPie(Rect pieRt, float as, float ae)
{
	pieRect = pieRt;

	_SetPie(pieRect, as, ae, pt1, pt2, txPt);

	//	largePieRect = Rect( pieRt.left - EL, pieRt.top - EL, pieRt.right + EL, pieRt.bottom + EL);
	float mid = (as + ae)/2;
	largePieRect = pieRect;
	OffsetRect(largePieRect, EL * cos(mid), EL * sin(mid) );

	_SetPie(largePieRect, as, ae, pt1m, pt2m, txPtm);
}

void Pizza::_SetPie(Rect pieRt, float as, float ae, Point& pt1, Point& pt2, Point& txPt )
{
	Point ct = CenterPoint(pieRt);
	float r = abs(pieRt.right - pieRt.left)/2;

	pt1 = D2D1::Point2( ct.x + (r * cos(as)), ct.y + (r * sin(as)) );
	pt2 = D2D1::Point2( ct.x + (r * cos(ae)), ct.y + (r * sin(ae)) );
	
	float txtPos = (as + ae)/2;
	txPt = D2D1::Point2( ct.x + (r*2/3 * cos(txtPos)), ct.y + (r*2/3 * sin(txtPos)) );

}


BOOL Pizza::CheckPoint(Point pt)
{
	/* test distance from center */
	float r = abs(pieRect.right - pieRect.left)/2;
	Point ct = CenterPoint(pieRect);
	Size v = D2D1::SizeF(pt.x - ct.x, pt.y - ct.y);
	float dist = sqrt(v.width*v.width + v.height*v.height);
	
	if (dist > r ) return FALSE;
	
	/* test angle between two vectors */
	Size v1 = D2D1::SizeF(pt1.x - ct.x, pt1.y - ct.y), v2 = D2D1::SizeF(pt2.x - ct.x, pt2.y - ct.y);	
	
	float a1 = getAngle(v1), a2 = getAngle(v2), a = getAngle(v);
	
	if(v1.height && !v2.height && v2.width) a2 = 2*M_PI;
	
	return (a2 >= a) && (a1 <= a);
}

void Pizza::DrawPie(DC& dc)
{
	//Brush brush( color );
	//dc.SelectObject(&brush);

	//if (large)
	//	dc.Pie(largePieRect, pt2m, pt1m);
	//else
	//	dc.Pie(pieRect, pt2, pt1);
}

void Pizza::DrawLabel(DC& dc)
{
	if (ratio < 0.005) return;
	string label;
	
	//label.Format("%s:%.0f", name, value);
	//dc.SetTextColor( _isDark(color)? LIGHT_COLOR: DARK_COLOR );
	//Point *p = large? &txPtm: &txPt;
	//dc.TextOut(p->x, p->y, label);
	//label.Format("%.2f%%", ratio*100);
	//dc.TextOut(p->x, p->y + 16, label);
}
