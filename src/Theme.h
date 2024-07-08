#include <cstring>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Tooltip.H>

#define OS_BUTTON_UP_BOX              FL_GTK_UP_BOX
#define OS_CHECK_DOWN_BOX             FL_GTK_DOWN_BOX
#define OS_BUTTON_UP_FRAME            FL_GTK_UP_FRAME
#define OS_CHECK_DOWN_FRAME           FL_GTK_DOWN_FRAME
#define OS_PANEL_THIN_UP_BOX          FL_GTK_THIN_UP_BOX
#define OS_SPACER_THIN_DOWN_BOX       FL_GTK_THIN_DOWN_BOX
#define OS_PANEL_THIN_UP_FRAME        FL_GTK_THIN_UP_FRAME
#define OS_SPACER_THIN_DOWN_FRAME     FL_GTK_THIN_DOWN_FRAME
#define OS_RADIO_ROUND_DOWN_BOX       FL_GTK_ROUND_DOWN_BOX
#define OS_HOVERED_UP_BOX             FL_PLASTIC_UP_BOX
#define OS_DEPRESSED_DOWN_BOX         FL_PLASTIC_DOWN_BOX
#define OS_HOVERED_UP_FRAME           FL_PLASTIC_UP_FRAME
#define OS_DEPRESSED_DOWN_FRAME       FL_PLASTIC_DOWN_FRAME
#define OS_INPUT_THIN_DOWN_BOX        FL_PLASTIC_THIN_DOWN_BOX
#define OS_INPUT_THIN_DOWN_FRAME      FL_PLASTIC_ROUND_DOWN_BOX
#define OS_MINI_BUTTON_UP_BOX         FL_GLEAM_UP_BOX
#define OS_MINI_DEPRESSED_DOWN_BOX    FL_GLEAM_DOWN_BOX
#define OS_MINI_BUTTON_UP_FRAME       FL_GLEAM_UP_FRAME
#define OS_MINI_DEPRESSED_DOWN_FRAME  FL_GLEAM_DOWN_FRAME
#define OS_DEFAULT_BUTTON_UP_BOX      FL_DIAMOND_UP_BOX
#define OS_DEFAULT_HOVERED_UP_BOX     FL_PLASTIC_THIN_UP_BOX
#define OS_DEFAULT_DEPRESSED_DOWN_BOX FL_DIAMOND_DOWN_BOX
#define OS_TOOLBAR_BUTTON_HOVER_BOX   FL_GLEAM_ROUND_UP_BOX
#define OS_SWATCH_BOX                 FL_ENGRAVED_BOX
#define OS_SWATCH_FRAME               FL_ENGRAVED_FRAME
#define OS_BG_BOX                     FL_FREE_BOXTYPE
#define OS_BG_DOWN_BOX                (Fl_Boxtype)(FL_FREE_BOXTYPE+1)
#define OS_TOOLBAR_FRAME              (Fl_Boxtype)(FL_FREE_BOXTYPE+2)

namespace OS
{
	static Fl_Color activated_color(Fl_Color c)
	{
		return Fl::draw_box_active() ? c : fl_inactive(c);
	}


	static void vertical_gradient(int x1, int y1, int x2, int y2, Fl_Color c1, Fl_Color c2)
	{
		int imax = y2 - y1;
		int d = imax ? imax : 1;
		if (Fl::draw_box_active())
		{
			for (int i = 0; i <= imax; i++)
			{
				float w = 1.0f - (float)i / d;
				fl_color(fl_color_average(c1, c2, w));
				fl_xyline(x1, y1 + i, x2);
			}
		}
		else
		{
			for (int i = 0; i <= imax; i++)
			{
				float w = 1.0f - (float)i / d;
				fl_color(fl_inactive(fl_color_average(c1, c2, w)));
				fl_xyline(x1, y1 + i, x2);
			}
		}
	}


	static void horizontal_gradient(int x1, int y1, int x2, int y2, Fl_Color c1, Fl_Color c2)
	{
		int imax = x2 - x1;
		int d = imax ? imax : 1;
		if (Fl::draw_box_active())
		{
			for (int i = 0; i <= imax; i++)
			{
				float w = 1.0f - (float)i / d;
				fl_color(fl_color_average(c1, c2, w));
				fl_yxline(x1 + i, y1, y2);
			}
		}
		else
		{
			for (int i = 0; i <= imax; i++)
			{
				float w = 1.0f - (float)i / d;
				fl_color(fl_inactive(fl_color_average(c1, c2, w)));
				fl_yxline(x1 + i, y1, y2);
			}
		}
	}


	static void use_native_settings()
	{
		Fl::visible_focus(0);
		Fl::scrollbar_size(15);
		Fl_Tooltip::size(12);
		Fl_Tooltip::delay(0.5f);
	}


	/************************* Dark (Adobe Photoshop CS6) *************************/

	static void dark_button_up_frame(int x, int y, int w, int h, Fl_Color)
	{
		// top outer border
		fl_color(activated_color(fl_rgb_color(0x30, 0x30, 0x30)));
		fl_xyline(x + 2, y, x + w - 3);
		// side outer borders
		fl_color(activated_color(fl_rgb_color(0x2C, 0x2C, 0x2C)));
		fl_yxline(x, y + 2, y + h - 3);
		fl_yxline(x + w - 1, y + 2, y + h - 3);
		// bottom outer border
		fl_color(activated_color(fl_rgb_color(0x27, 0x27, 0x27)));
		fl_xyline(x + 2, y + h - 1, x + w - 3);
		// top inner border
		fl_color(activated_color(fl_rgb_color(0x91, 0x91, 0x91)));
		fl_xyline(x + 2, y + 1, x + w - 3);
		// corners
		fl_color(activated_color(fl_rgb_color(0x47, 0x47, 0x47)));
		fl_xyline(x, y + 1, x + 1, y);
		fl_xyline(x, y + h - 2, x + 1, y + h - 1);
		fl_yxline(x + w - 2, y, y + 1, x + w - 1);
		fl_yxline(x + w - 2, y + h - 1, y + h - 2, x + w - 1);
	}

	static void dark_button_up_box(int x, int y, int w, int h, Fl_Color c)
	{
		if (c != FL_RED && c != FL_GREEN)
		{
			if (w >= h)
			{
				vertical_gradient(x + 1, y + 2, x + w - 2, y + h - 1, activated_color(fl_rgb_color(0x75, 0x75, 0x75)),
					activated_color(fl_rgb_color(0x62, 0x62, 0x62)));
			}
			else
			{
				horizontal_gradient(x + 1, y + 2, x + w - 2, y + h - 1, activated_color(fl_rgb_color(0x75, 0x75, 0x75)),
					activated_color(fl_rgb_color(0x62, 0x62, 0x62)));
			}
		}
		else
			vertical_gradient(x + 1, y + 2, x + w - 2, y + h - 1, activated_color(c), activated_color(c));
		dark_button_up_frame(x, y, w, h, c);
	}

	static void dark_panel_thin_up_frame(int x, int y, int w, int h, Fl_Color)
	{
		// top and left borders
		fl_color(activated_color(fl_rgb_color(0x6A, 0x6A, 0x6A)));
		fl_yxline(x, y + h - 2, y, x + w - 2);
		// bottom and right borders
		fl_color(activated_color(fl_rgb_color(0x28, 0x28, 0x28)));
		fl_xyline(x, y + h - 1, x + w - 1, y);
	}

	static void dark_panel_thin_up_box(int x, int y, int w, int h, Fl_Color c)
	{
		fl_color(activated_color(c));
		fl_rectf(x + 1, y + 1, w - 2, h - 2);
		dark_panel_thin_up_frame(x, y, w, h, c);
	}

	static void dark_spacer_thin_down_frame(int x, int y, int w, int h, Fl_Color)
	{
		// top and left borders
		fl_color(activated_color(fl_rgb_color(0x38, 0x38, 0x38)));
		fl_yxline(x, y + h - 2, y, x + w - 2);
		// bottom and right borders
		fl_color(activated_color(fl_rgb_color(0x74, 0x74, 0x74)));
		fl_xyline(x, y + h - 1, x + w - 1, y);
	}

	static void dark_spacer_thin_down_box(int x, int y, int w, int h, Fl_Color c)
	{
		fl_color(activated_color(c));
		fl_rectf(x + 1, y + 1, w - 2, h - 2);
		dark_spacer_thin_down_frame(x, y, w, h, c);
	}

	static void dark_radio_round_down_frame(int x, int y, int w, int h, Fl_Color)
	{
		fl_color(activated_color(fl_rgb_color(0x30, 0x30, 0x30)));
		fl_arc(x, y, w, h, 0.0, 360.0);
	}

	static void dark_radio_round_down_box(int x, int y, int w, int h, Fl_Color c)
	{
		// top edges
		fl_color(activated_color(fl_rgb_color(0x75, 0x75, 0x75)));
		fl_arc(x + 1, y + 1, w - 2, h - 2, 0.0, 180.0);
		// bottom edges
		fl_color(activated_color(fl_rgb_color(0x62, 0x62, 0x62)));
		fl_arc(x + 1, y + 1, w - 2, h - 2, 180.0, 360.0);
		// gradient
		vertical_gradient(x + 2, y + 2, x + w - 3, y + h - 3, fl_rgb_color(0x74, 0x74, 0x74), fl_rgb_color(0x63, 0x63, 0x63));
		dark_radio_round_down_frame(x, y, w, h, c);
	}

	static void dark_depressed_down_frame(int x, int y, int w, int h, Fl_Color)
	{
		// top outer border
		fl_color(activated_color(fl_rgb_color(0x27, 0x27, 0x27)));
		fl_xyline(x + 2, y, x + w - 3);
		// side outer borders
		fl_color(activated_color(fl_rgb_color(0x2C, 0x2C, 0x2C)));
		fl_yxline(x, y + 2, y + h - 3);
		fl_yxline(x + w - 1, y + 2, y + h - 3);
		// bottom outer border
		fl_color(activated_color(fl_rgb_color(0x30, 0x30, 0x30)));
		fl_xyline(x + 2, y + h - 1, x + w - 3);
		// top inner border
		fl_color(activated_color(fl_rgb_color(0x33, 0x33, 0x33)));
		fl_xyline(x + 2, y + 1, x + w - 3);
		// corners
		fl_color(activated_color(fl_rgb_color(0x32, 0x32, 0x32)));
		fl_xyline(x, y + 1, x + 1, y);
		fl_xyline(x, y + h - 2, x + 1, y + h - 1);
		fl_yxline(x + w - 2, y, y + 1, x + w - 1);
		fl_yxline(x + w - 2, y + h - 1, y + h - 2, x + w - 1);
		fl_color(activated_color(fl_rgb_color(0x4B, 0x4B, 0x4B)));
		fl_point(x, y);
		fl_point(x + w - 1, y);
		fl_point(x, y + h - 1);
		fl_point(x + w - 1, y + h - 1);
	}

	static void dark_depressed_down_box(int x, int y, int w, int h, Fl_Color c)
	{
		vertical_gradient(x + 1, y + 2, x + w - 2, y + h - 1, activated_color(fl_rgb_color(0x3F, 0x3F, 0x3F)),
			activated_color(fl_rgb_color(0x37, 0x37, 0x37)));
		dark_depressed_down_frame(x, y, w, h, c);
	}

	static void dark_input_thin_down_frame(int x, int y, int w, int h, Fl_Color)
	{
		// top and side outer borders
		fl_color(activated_color(fl_rgb_color(0x30, 0x30, 0x30)));
		fl_xyline(x, y, x + w - 1);
		fl_yxline(x, y + 1, y + h - 2);
		fl_yxline(x + w - 1, y + 1, y + h - 2);
		// bottom outer border
		fl_color(activated_color(fl_rgb_color(0x29, 0x29, 0x29)));
		fl_xyline(x, y + h - 1, x + w - 1);
		// top inner border
		fl_color(activated_color(fl_rgb_color(0x37, 0x37, 0x37)));
		fl_xyline(x + 1, y + 1, x + w - 2);
		// top and side innermost borders
		fl_color(activated_color(fl_rgb_color(0x39, 0x39, 0x39)));
		fl_xyline(x + 1, y + 2, x + w - 2);
		fl_yxline(x + 1, y + 3, y + h - 2);
		fl_yxline(x + w - 2, y + 3, y + h - 2);
	}

	static void dark_input_thin_down_box(int x, int y, int w, int h, Fl_Color c)
	{
		fl_color(activated_color(c));
		fl_rectf(x + 2, y + 3, w - 4, h - 4);
		dark_input_thin_down_frame(x, y, w, h, c);
	}

	static void dark_swatch_frame(int x, int y, int w, int h, Fl_Color)
	{
		// outer border
		fl_color(activated_color(fl_rgb_color(0x25, 0x25, 0x25)));
		fl_rect(x, y, w, h);
		// inner border
		fl_color(activated_color(fl_rgb_color(0xFF, 0xFF, 0xFF)));
		fl_rect(x + 1, y + 1, w - 2, h - 2);
	}

	static void dark_swatch_box(int x, int y, int w, int h, Fl_Color c)
	{
		fl_color(activated_color(c));
		fl_rectf(x + 2, y + 2, w - 4, h - 4);
		dark_swatch_frame(x, y, w, h, c);
	}

	static void use_dark_scheme()
	{
		Fl::scheme("gtk+");
		Fl::set_boxtype(OS_BUTTON_UP_BOX, dark_button_up_box, 1, 1, 2, 2);
		Fl::set_boxtype(OS_CHECK_DOWN_BOX, OS_BUTTON_UP_BOX);
		Fl::set_boxtype(OS_BUTTON_UP_FRAME, dark_button_up_frame, 1, 1, 2, 2);
		Fl::set_boxtype(OS_CHECK_DOWN_FRAME, OS_BUTTON_UP_FRAME);
		Fl::set_boxtype(OS_PANEL_THIN_UP_BOX, dark_panel_thin_up_box, 1, 1, 2, 2);
		Fl::set_boxtype(OS_SPACER_THIN_DOWN_BOX, dark_spacer_thin_down_box, 1, 1, 2, 2);
		Fl::set_boxtype(OS_PANEL_THIN_UP_FRAME, dark_panel_thin_up_frame, 1, 1, 2, 2);
		Fl::set_boxtype(OS_SPACER_THIN_DOWN_FRAME, dark_spacer_thin_down_frame, 1, 1, 2, 2);
		Fl::set_boxtype(OS_RADIO_ROUND_DOWN_BOX, dark_radio_round_down_box, 1, 1, 2, 2);
		Fl::set_boxtype(OS_HOVERED_UP_BOX, OS_BUTTON_UP_BOX);
		Fl::set_boxtype(OS_DEPRESSED_DOWN_BOX, dark_depressed_down_box, 1, 1, 2, 2);
		Fl::set_boxtype(OS_HOVERED_UP_FRAME, OS_BUTTON_UP_FRAME);
		Fl::set_boxtype(OS_DEPRESSED_DOWN_FRAME, dark_depressed_down_frame, 1, 1, 2, 2);
		Fl::set_boxtype(OS_INPUT_THIN_DOWN_BOX, dark_input_thin_down_box, 1, 2, 2, 4);
		Fl::set_boxtype(OS_INPUT_THIN_DOWN_FRAME, dark_input_thin_down_frame, 1, 2, 2, 4);
		Fl::set_boxtype(OS_DEFAULT_BUTTON_UP_BOX, OS_BUTTON_UP_BOX);
		Fl::set_boxtype(OS_DEFAULT_HOVERED_UP_BOX, OS_HOVERED_UP_BOX);
		Fl::set_boxtype(OS_DEFAULT_DEPRESSED_DOWN_BOX, OS_DEPRESSED_DOWN_BOX);
		Fl::set_boxtype(OS_TOOLBAR_BUTTON_HOVER_BOX, OS_BUTTON_UP_BOX);
		Fl::set_boxtype(OS_SWATCH_BOX, dark_swatch_box, 2, 2, 4, 4);
		Fl::set_boxtype(OS_SWATCH_FRAME, dark_swatch_frame, 2, 2, 4, 4);
		Fl::set_boxtype(OS_MINI_BUTTON_UP_BOX, OS_BUTTON_UP_BOX);
		Fl::set_boxtype(OS_MINI_DEPRESSED_DOWN_BOX, OS_DEPRESSED_DOWN_BOX);
		Fl::set_boxtype(OS_MINI_BUTTON_UP_FRAME, OS_BUTTON_UP_FRAME);
		Fl::set_boxtype(OS_MINI_DEPRESSED_DOWN_FRAME, OS_DEPRESSED_DOWN_FRAME);
		Fl::set_boxtype(FL_UP_BOX, OS_BUTTON_UP_BOX);
		Fl::set_boxtype(FL_DOWN_BOX, OS_BUTTON_UP_BOX);
		Fl::set_boxtype(FL_ROUND_DOWN_BOX, OS_RADIO_ROUND_DOWN_BOX);
		Fl::set_boxtype(OS_BG_BOX, FL_FLAT_BOX);
		Fl::set_boxtype(OS_BG_DOWN_BOX, OS_BG_BOX);
		Fl::set_boxtype(OS_TOOLBAR_FRAME, OS_PANEL_THIN_UP_FRAME);
	}

	static void use_dark_colors()
	{
		Fl::background(0x53, 0x53, 0x53);
		Fl::background2(0x3A, 0x3A, 0x3A);
		Fl::foreground(0xEC, 0xEC, 0xEC);
		Fl::set_color(FL_INACTIVE_COLOR, 0x26, 0x26, 0x26);
		Fl::set_color(FL_SELECTION_COLOR, 0xD6, 0xD6, 0xD6);
		Fl::set_color(FL_FREE_COLOR, 0x24, 0x24, 0x24);
		Fl_Tooltip::color(fl_rgb_color(0xFF, 0xFF, 0xCC));
		Fl_Tooltip::textcolor(fl_rgb_color(0x00, 0x00, 0x00));
	}

	static void use_dark_theme()
	{
		use_dark_scheme();
		use_dark_colors();
		use_native_settings();
	}
}