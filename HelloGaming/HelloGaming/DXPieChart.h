#pragma once

#include "DirectXHelper.h"
#include <string>

using namespace std;

typedef D2D1_POINT_2F Point;
typedef D2D1_SIZE_F Size;
typedef Microsoft::WRL::ComPtr<ID2D1DeviceContext> DC;
typedef D2D1_RECT_F Rect;
typedef void Font;
typedef void Pen;
typedef void Brush;

class DXPieChart;
struct Pizza
{
	Pizza(string name = "pizza", float value = 1, COLORREF cr = RGB(255,0,0)):name(name), value(value), color(cr), large(FALSE){}
	BOOL CheckPoint(Point pt);
	void DrawPie(DC&);
	void DrawLabel(DC&);
	void SetPie(Rect pieRt, float aStart, float aEnd);

	string name;
	float value;
	COLORREF color;
	float ratio;
	BOOL large;

	friend DXPieChart;
private:
	void _SetPie(Rect pieRt, float as, float ae, Point& pt1, Point& pt2, Point& txPt );
	Point pt1, pt2, txPt;
	Point pt1m, pt2m, txPtm;
	Rect pieRect, largePieRect;
};

typedef map<string, Pizza> PIZZAMAP;
typedef map<string, COLORREF> PIECOLOR;

class DXPieChart  
{
public:
	DXPieChart();
	virtual ~DXPieChart();

	void SetRect(const Rect& rt);
	void Draw(DC&);
	void SetFont(Font* font){ m_pFont = font; }
	void SetBorderPen(Pen* pen){ m_pBorderPen = pen; }

	void SetPizza(Pizza& p){ m_pizza[p.name] = p; _resetPoints(); }

	void SetData(string name, float value, COLORREF c){ Pizza p(name, value, c); SetPizza(p); }

	string PointWhichPie(Point pt);
	void EnlargePie(string name);
	void GetAllPieColor(PIECOLOR& map);

private:
	void SaveContext(DC&);
	void RestoreContext(DC&);

	Brush* m_pOldBr;
	Font* m_pOrgFont;
	Pen* m_pOrgPen;
	int m_bkMode;
	COLORREF m_oldTxCr;

private:
	void _resetPoints();

	COLORREF m_bgColor;
	Rect m_rt, m_pieRt;
	Point m_ct;

	Font *m_pFont;
	Pen *m_pBorderPen;

	vector<COLORREF> m_pieColor;
	PIZZAMAP m_pizza;

	float	m_radius;
};
