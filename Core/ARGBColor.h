#ifndef __COLOR_H__
#define __COLOR_H__

#include "TypeDefinitions.h"
#include "Debug.h"
#include "Vector.h"

using OGE::math::Vector;

namespace OGE
{
	class ARGBColor
	{
	public:
		ARGBColor();
		ARGBColor(float a, float r, float g, float b);
		~ARGBColor();

		void setColorf(float a, float r, float g, float b);

		Vector ARGB() const;
		Vector RGBA() const;
		uint32 value();

		float alpha() const;
		float red() const;
		float green() const;
		float blue() const;

		ARGBColor operator * (float value);
		ARGBColor operator + (const ARGBColor& ARGBColor);
		void operator += (const ARGBColor& rColor);

	private: 

		float a;
		float r;
		float g;
		float b;
	};

	inline ARGBColor::ARGBColor() :
		a(0.0f), r(0.0f), g(0.0f), b(0.0f) {}
	inline ARGBColor::ARGBColor(float a, float r, float g, float b) : a(a),
		r(r), g(g), b(b) 
	{
	}

	inline ARGBColor::~ARGBColor() { }

	inline ARGBColor ARGBColor::operator * (float value)
	{
		return ARGBColor(0.0f, (r * value), (g * value), (b * value));
	}

	inline ARGBColor ARGBColor::operator + (const ARGBColor& color)
	{
		return ARGBColor(a + color.a, r + color.r, g + color.g, b + color.b);
	}

	inline void ARGBColor::operator += (const ARGBColor& rColor)
	{
		this->a += rColor.a;
		this->r += rColor.r;
		this->g += rColor.g;
		this->b += rColor.b;

		if(a > 1.0f) a = 1.0f;
		if(r > 1.0f) r = 1.0f;
		if(g > 1.0f) g = 1.0f;
		if(b > 1.0f) b = 1.0f;
	}

	inline void ARGBColor::setColorf(float a, float r, float g, float b)
	{
		this->a = a;
		this->r = r;
		this->g = g;
		this->b = b;

		if(a > 1.0f) a = 1.0f;
		if(r > 1.0f) r = 1.0f;
		if(g > 1.0f) g = 1.0f;
		if(b > 1.0f) b = 1.0f;
	}

	inline Vector ARGBColor::ARGB() const
	{
		return Vector(a, r, g, b);
	}

	inline Vector ARGBColor::RGBA() const
	{
		return Vector(r, g, b, a);
	}

	inline float ARGBColor::alpha() const
	{
		return a;
	}

	inline float ARGBColor::red() const
	{
		return r;
	}

	inline float ARGBColor::green() const
	{
		return r;
	}

	inline float ARGBColor::blue() const
	{
		return b;
	}

	inline uint32 ARGBColor::value()
	{
		if(a > 1.0f) a = 1.0f;
		if(r > 1.0f) r = 1.0f;
		if(g > 1.0f) g = 1.0f;
		if(b > 1.0f) b = 1.0f;

		uint32 alpha = (uint32)(a * 255);
		uint32 red = (uint32)(r * 255);
		uint32 green  = (uint32)(g * 255);
		uint32 blue = (uint32)(b * 255);

		return  (alpha << 24)  | (red << 16) | (green << 8) | blue;
	}
}

#endif

