//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	
	GLubyte color[3];

	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	if (pDoc->colorPicked) {
		GLubyte pickedcolor[3];
		memcpy(pickedcolor, pDoc->Pickedcolor, 3);
		for (int i = 0; i != 3; i++) {
			color[i] = color[i] * (pickedcolor[i] / 255.0);
		}
	}
	
	GLubyte alphaByte = (byte)static_cast<unsigned int>(pDoc->getAlpha() * 255);
	glColor4ub(color[0], color[1], color[2], alphaByte);
}

float ImpBrush::GetColorValue(const GLubyte color[3]) {
	return ((float)color[0] / 255.0 + (float)color[1] / 255.0 + (float)color[2] / 255.0) / 3.0;
}