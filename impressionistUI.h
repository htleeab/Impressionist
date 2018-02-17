//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Color_Chooser.H>

#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Int_Input.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

struct filterValueUserData {
	void* self;
	int row_index;
	int col_index;

	filterValueUserData(void* pointer, int i, int j) {
		self = pointer;
		row_index = i;
		col_index = j;
	}
};

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Choice*			m_BrushTypeChoice;

	Fl_Slider*			m_BrushSizeSlider;
	Fl_Button*          m_ClearCanvasButton;

	Fl_Slider*			m_BrushLineWidthSlider;
	Fl_Slider*			m_BrushLineAngleSlider;
	Fl_Choice*			m_BrushDirectionChoice;
	Fl_Slider*			m_BrushAlphaSlides;

	Fl_Button*			m_AutoPaintButton;
	Fl_Button*			m_AnotherGradientButton;

	//filter
	Fl_Button*          m_FilterBlurButton;
	Fl_Button*          m_FilterSharpenButton;
	Fl_Button*          m_FilterCustomizeButton;
	Fl_Window*			filterDialog;
	Fl_Int_Input*		filterRowInput;
	Fl_Int_Input*		filterColInput;
	Fl_Button*			initFilterButton;
	Fl_Int_Input***		filterValueInput;//2D pointers
	Fl_Button*			applyFilterButton;
	Fl_Button*			applyConvolutionButton;
	Fl_Check_Button*	normalizeFilterCheckBox;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);
	void				deleteFilterInput();

	// Interface to get attribute
	int					getSize();
	void				setSize(int size);
	int					getWidth();
	void				setWidth(int width);
	int					getAngle();
	void				setAngle(int angle);
	float				getAlpha();
	void				rightMouseSet(float angle, float size);

private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nSize;
	int		m_lWidth;
	int		m_lAngle;
	float	m_alpha;
	int		filterRow;	//it is the number of int input box, the filter size that using should be found in Doc
	int		filterCol;
	bool	normalizeFilter; //default true
	int**	filterValue;//2D array
	bool	anotherGradientButtonBool;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];
	static Fl_Menu_Item		brushDirectionMenu[4];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_swap_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_pick_color(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void cb_load_another_image(Fl_Menu_* o, void*);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_brushDirectionChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void	cb_widthSlides(Fl_Widget* o, void* v);
	static void	cb_angleSlides(Fl_Widget* o, void* v);
	static void	cb_alphaSlides(Fl_Widget* o, void* v);

	static void cb_auto_paint_button(Fl_Widget* o, void* v);
	static void cb_another_gradient_button(Fl_Widget* o, void* v);
	static void cb_undo_button(Fl_Widget* o, void* v);

	static void cb_blur_filter_button(Fl_Widget* o, void* v);
	static void cb_sharpen_filter_button(Fl_Widget* o, void* v);
	static void cb_customize_filter_button(Fl_Widget* o, void* v);
	static void cb_filterRowInput(Fl_Widget* o, void* v);
	static void cb_filterColInput(Fl_Widget* o, void* v);
	static void cb_init_filter(Fl_Widget* o, void* v);
	static void cb_update_filter(Fl_Widget* o, void* v);
	static void cb_normalize_checkbox(Fl_Widget* o, void* v);
	static void cb_apply_filter(Fl_Widget* o, void* v);
	static void cb_convolution(Fl_Widget* o, void* v);

};

#endif
