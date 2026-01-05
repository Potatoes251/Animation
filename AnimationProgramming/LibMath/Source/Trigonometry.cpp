
#include <cmath>

#include "LibMath/Trigonometry.h"
#include <iostream>


float LibMath::sin(Radian angle)
{
	angle.wrap();

	float x = angle.raw();
	float xSquare = x * x;
	float sin = x;

	float sign = -1;

	long long int factorials[8] = { 6, 120, 5040, 362880, 39916800, 6227020800, 1307674368000, 355687428096000 };
	// precompute the factorial to avoid too many unnecessary calculations

	// formula : (-1)^n * x^(2n + 1)
	//				  (2n + 1)!

	for (int i = 0; i < 8; ++i)
	{
		x *= xSquare;
		sin += sign * x / factorials[i];
		sign = -sign;
	}
	return sin;
}

float LibMath::cos(Radian angle)
{
	angle.wrap();

	double	x = angle.raw();
	double	xSquare = x * x;
	double	cos = 1;
	int	sign = -1;
	long long int		factorials[8] = { 2, 24, 720, 40320, 3628800, 479001600, 87178291200, 20922789888000 };
	// precompute the factorial to avoid too many unnecessary calculations

	x = 1;

	// formula : (-1)^n * x^(2n)
	//				  (2n)!

	for (int i = 0; i < 8; ++i)
	{
		x *= xSquare;
		cos += sign * x / factorials[i];
		sign = -sign;
	}
	return static_cast<float>(cos);
}

float LibMath::tan(Radian angle)
{
	// tan = sin/cos
	return sin(angle) / cos(angle);
}

LibMath::Radian LibMath::asin(float sin)
{
	double	x = sin;
	double	xSquare = x * x;
	double	angle = 0;
	double	coeficients[8] = {
	1.0,
	0.1666666666666667,     // 1/6
	0.075,                  // 3/40
	0.04464285714285714,    // 5/112
	0.03038194444444444,    // 35/1152
	0.022372159090909092,   // 63/2816
	0.017352764423076924,   // 231/13312
	0.013983054005396834    // 143/10240
	};
	// precompute the coeficients to avoid too many unnecessary calculations

	//coef formula :		(2n)!
	//				4^n * (n!)^2 * (2n + 1)

	for (int i = 0; i < 8; ++i)
	{
		angle += x * coeficients[i];
		x *= xSquare;
	}
	return Radian(static_cast<float>(angle));
}

LibMath::Radian LibMath::acos(float value)
{
	return Radian(g_Pi / 2.f - asin(value).raw());
}

LibMath::Radian LibMath::atan(float x)
{
	if (x > 1.f)
	{
		const float halfPi = g_Pi / 2;
		return Radian(halfPi) - Radian(atan(1 / x));
	}

	int		tanSign = 1;	// store the sign of x

	if (x < 0)
	{
		tanSign = -1;
		x = -x;
	}

	float	xPower = x;			// store the consecutive power of x (x^1, x^3,...)
	float	xSquare = x * x;
	float	angle = x;
	int		sign = 1;
	int		denominator = 1;


	// formula :  (-1)^n * x^(2n + 1)
	//					2n + 1

	for (int i = 0; i < 8; ++i)
	{
		xPower *= xSquare;
		denominator += 2;
		sign = -sign;
		angle += sign * xPower / denominator;
	}
	return Radian(tanSign * angle);
}

LibMath::Radian LibMath::atan(float y, float x)	// atan per quadrant ((x, y) => coordinates on circle)
{
	if (x == 0)	// angle == +/- PI/2 (90)
	{
		if (y > 0)	// top middle of circle
		{
			return Radian(g_Pi / 2);
		}
		else if (y < 0) // bottom middle of circle
		{
			return Radian(-g_Pi / 2);
		}
		else // y == 0	=> center
		{
			return Radian(0);
		}
	}

	Radian baseAngle = atan(y / x);

	if (x > 0)
	{
		return baseAngle;	// Quadrant I or IV sign depend on Y anyway
	}
	else // x < 0
	{
		if (y >= 0)
		{
			return baseAngle + Radian(g_Pi); // Quadrant II
		}
		else
		{
			return baseAngle - Radian(g_Pi); // Quadrant III
		}
	}
}
