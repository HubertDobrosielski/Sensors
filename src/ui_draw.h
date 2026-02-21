// ui_draw.h
#pragma once
#include "renderer.h"
#include "chart.h"

using V = CPoint;
using R = CRect;

namespace ui
{
    void draw(const Title &, Renderer &);
    void draw(const ChartArea &, Renderer &);
    void draw(const Grid &g, Renderer &r);
}

