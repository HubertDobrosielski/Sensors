
#include "ui_draw.h"

void ui::draw(const Title &t, Renderer &r)
{
    r.text(t.pos, t.text.c_str(), t.Color);
    // dodac font size
}

void ui::draw(const ChartArea &a, Renderer &r)
{
    r.filledRect(R(a.rect), a.bg);
}

void ui::draw(const Grid &g, Renderer &r)
{
    for (auto &[a, b] : g.lines)
    {
        r.line(V(a), V(b), BLACK);
    }
}