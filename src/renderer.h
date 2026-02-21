#pragma once
#include "cpoint.h" // CPoint, CRect
#include <vector>

#include "../lib/primlib.h"
#include <stdexcept>

struct Renderer
{
    // primitives
    virtual void pixel(CPoint p, color c) = 0;
    virtual void line(CPoint a, CPoint b, color c) = 0;
    virtual void wideLine(CPoint a, CPoint b, int width, color c) = 0;

    // shapes
    virtual void rect(CRect r, color c) = 0;
    virtual void filledRect(CRect r, color c) = 0;
    virtual void circle(CPoint p, int radius, color c) = 0;
    virtual void filledCircle(CPoint p, int radius, color c) = 0;

    // text
    virtual void text(CPoint p, const char *s, color c) = 0;

    // helpers
    virtual void polyline(const std::vector<CPoint> &pts, color c) = 0;

    // frame
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    virtual ~Renderer() = default;

    // renderer.h
    virtual void clear(color c) = 0;
};

struct GfxRenderer : public Renderer
{
    explicit GfxRenderer()
    {
        if (gfx_init())
            throw std::runtime_error("gfx_init failed");
    }

    void pixel(CPoint p, color c) override
    {
        gfx_pixel(p.x, p.y, c);
    }

    void line(CPoint a, CPoint b, color c) override
    {
        gfx_line(a.x, a.y, b.x, b.y, c);
    }

    void wideLine(CPoint a, CPoint b, int w, color c) override
    {
        gfx_wline(a.x, a.y, b.x, b.y, c, w);
    }

    void rect(CRect r, color c) override
    {
        auto br = r.getBottomRight();
        gfx_rect(r.topleft.x, r.topleft.y, br.x, br.y, c);
    }

    void filledRect(CRect r, color c) override
    {
        auto br = r.getBottomRight();
        gfx_filledRect(r.topleft.x, r.topleft.y, br.x, br.y, c);
    }

    void circle(CPoint p, int radius, color c) override
    {
        gfx_circle(p.x, p.y, radius, c);
    }

    void filledCircle(CPoint p, int radius, color c) override
    {
        gfx_filledCircle(p.x, p.y, radius, c);
    }

    void text(CPoint p, const char *s, color c) override
    {
        gfx_textout(p.x, p.y, s, c);
    }

    void polyline(const std::vector<CPoint> &pts, color c) override
    {
        if (pts.size() < 2)
            return;
        for (size_t i = 1; i < pts.size(); ++i)
            line(pts[i - 1], pts[i], c);
    }

    void beginFrame() override
    {
        // gfx has implicit frame (OK)
    }

    void endFrame() override
    {
        gfx_updateScreen();
    }

    // gfx_renderer.h
    void clear(color c) override
    {
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, c);
    }
};