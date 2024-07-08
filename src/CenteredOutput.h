#pragma once
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>

class CenteredOutput : public Fl_Output
{
public:
    CenteredOutput(int x, int y, int w, int h, const char* l = nullptr) : Fl_Output(x, y, w, h, l) {}
protected:
    void draw() override
    {
        // Draw the box and the frame as usual
        Fl_Output::draw_box();

        // Get the text to be displayed
        const char* text = value();
        if (!text) return;

        // Get the width and height of the text
        int text_width = 0;
        int text_height = 0;
        fl_font(labelfont(), labelsize());
        fl_measure(text, text_width, text_height, false);

        // Calculate the position to center the text
        int x = this->x() + (w() - text_width) / 2;
        int y = this->y() + (h() + text_height) / 2 - fl_descent();

        // Draw the text
        fl_color(textcolor());
        fl_draw(text, x, y);
    }
};