#pragma once

struct RenderState;
struct Bindable
{
	Bindable() = default;
	virtual ~Bindable() = default;
    
	virtual void bind(RenderState& gfx) {};
	virtual void unbind(RenderState& gfx) {};
};

struct Drawable : Bindable
{
	Drawable() = default;
	virtual ~Drawable() = default;
    
	virtual void draw(RenderState& gfx) = 0;
};