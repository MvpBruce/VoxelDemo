#pragma once

class ToolHandler
{
public:
    virtual ~ToolHandler() {};

    virtual void Active() = 0;
    virtual void Deactive() = 0;

    virtual bool MouseLButtonDown() = 0;
    //virtual bool MouseLButtonUp() = 0;
    virtual bool MouseRbuttonDown() = 0;
    //virtual bool MouseRbuttonUp() = 0;
    //virtual bool MouseMove(int x, int y) = 0;
};
