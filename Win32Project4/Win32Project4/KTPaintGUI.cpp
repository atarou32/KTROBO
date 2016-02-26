#include "KTPaintGUI.h"
#pragma once
#include "windows.h"
#include "wingdi.h"
#include "gdiplus.h"
#include "math.h"
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "GdiPlus.lib") //gdiplus.h‚ÌŽg—p

using namespace Gdiplus;

KTPaintGUI::KTPaintGUI(void)
{
}


KTPaintGUI::~KTPaintGUI(void)
{
}

void KTPaintGUI::drawRectangleToWindow(HDC hDC, unsigned char r, unsigned char g, unsigned char b, int left, int top, int right, int bottom) {

	HPEN   hNewPen   = (HPEN)CreatePen(PS_INSIDEFRAME,4,RGB(0xFF,0x66,0x00));
    HPEN   hOldPen   = (HPEN)SelectObject(hDC,hNewPen);
    HBRUSH hNewBrush = (HBRUSH)CreateSolidBrush(RGB(r,g,b));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC,hNewBrush);
    
    Rectangle( hDC, left, top, right, bottom );
    
    DeleteObject( SelectObject(hDC,hOldBrush) );
    DeleteObject( SelectObject(hDC,hOldPen) );


}

void KTPaintGUI::drawAkiRectangleToWindow(HDC hDC, int left, int top, int right, int bottom) {

	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC,GetStockObject(NULL_BRUSH));
    HPEN   hNewPen   = (HPEN)CreatePen(PS_INSIDEFRAME,4,RGB(0xFF,0x66,0x00));
    HPEN   hOldPen   = (HPEN)SelectObject(hDC,hNewPen);
    
    Rectangle( hDC, left, top, right, bottom );
    
    SelectObject( hDC, hOldBrush );
    SelectObject( hDC, hOldPen );
    DeleteObject( hNewPen );



}

void KTPaintGUI::drawGradiationTriangleToWindow(HDC hdc, MYTRIANGLEPOINT* points) {

	TRIVERTEX vt[] = {
		{points[0].x, points[0].y,points[0].r<<8, points[0].g<<8, points[0].b<<8, 0 },
		{points[1].x, points[1].y,points[1].r<<8, points[1].g<<8, points[1].b<<8, 0 },
		{points[2].x, points[2].y,points[2].r<<8, points[2].g<<8, points[2].b<<8, 0 }
	};

	

	GRADIENT_TRIANGLE gt;
	gt.Vertex1 = 2;
	gt.Vertex2 = 1;
	gt.Vertex3 = 0;

	GradientFill(hdc,vt,3,&gt,1,GRADIENT_FILL_TRIANGLE);

}


GdiplusStartupInput gdiSI;
ULONG_PTR           gdiToken;

void KTPaintGUI::startUp() {

	GdiplusStartup(&gdiToken,&gdiSI,NULL);

}
void KTPaintGUI::shutDown() {

	GdiplusShutdown(gdiToken);
}

void KTPaintGUI::drawGradiationCircleToWindow(HDC hdc, POINT center, float radius) {
	float theta = 3.1415926535*2/6;
	PointF pts[6];
	for (int i=0;i<6;i++) {
		pts[i].X = center.x+radius*cos(theta*i)*2/1.732;
		pts[i].Y = center.y+radius*sin(theta*i)*2/1.732;
	}

	GraphicsPath path;
	//path.AddEllipse((int)(center.x-radius), (int)(center.y-radius), (int)(radius*2),(int)(2*radius));
	//path.AddRectangle(rec);
	//path.AddLines(points,12);
	/*for (int i=0;i<6;i++) {
		path.AddLine(pts[i].X,pts[i].Y, pts[i].X, pts[i].Y);
	}*/
	PathGradientBrush pBrush(pts,6);
	
	Color colors[] = {
					  Color(255, 255, 0, 0),  // (0, 0) red
					  Color(255, 255, 255, 0),  // (160, 0) green
					  Color(255, 0, 255, 0),  // (160, 200) green
					  Color(255, 0, 255, 255),  // (80, 150) blue
					  Color(255, 0, 0, 255), //5
					  Color(255, 255, 0, 255), 
	}; // (0, 200) red

	int count = 6;
	pBrush.SetSurroundColors(colors,&count);//, &count);

	pBrush.SetCenterColor(Color(255, 255, 255, 255));

	// Use the path gradient brush to fill a rectangle.
	Graphics gdayo(hdc);
	gdayo.FillEllipse(&pBrush, (int)(center.x-radius), (int)(center.y-radius), (int)(radius*2),(int)(2*radius));
//	gdayo.FillRectangle(&pBrush,(int)(center.x-radius),(int)(center.y-radius),(int)(radius*2),(int)(radius*2));

	float radiust = radius * 0.7;
	Ellipse(hdc,(int)(center.x-radiust), (int)(center.y-radiust), (int)(center.x+radiust),(int)(center.y+radiust));
}



