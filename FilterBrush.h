//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef FILTERBRUSH_H
#define FILTERBRUSH_H

#include "ImpBrush.h"

class FilterBrush :
	public ImpBrush
{
public:
	FilterBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	~FilterBrush();

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void SetColor(const Point source);
	char* BrushName(void);
};

#endif
