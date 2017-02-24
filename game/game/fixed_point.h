#ifndef FIXED_POINT_H_
#define FIXED_POINT_H_

#define FP_SCALE_SHIFT 8

class fixed_point
{
public:
	typedef int T;

	fixed_point(void);
	fixed_point(const fixed_point &a);

	fixed_point(unsigned int a);
	fixed_point(float a);
	//inline operator fixed_point();
	//inline operator float();

	float get_float(void);
	
	void set_value_by_unsigned_int(unsigned int a)
	{
		value = a << FP_SCALE_SHIFT;
	}

	void set_value_by_signed_int(int a)
	{
		value = a << FP_SCALE_SHIFT;
	}

	void set_value_by_float(float a)
	{
		value = (T)(a * (float)(1 << FP_SCALE_SHIFT));
	}
	
	T value;

	static const fixed_point mul(const fixed_point a, const fixed_point b)
	{
		fixed_point f;
		f.value = (a.value * b.value) >> FP_SCALE_SHIFT;
		return f;
	}

	static const fixed_point div(const fixed_point a, const fixed_point b)
	{
		fixed_point f;
		f.value = (a.value << FP_SCALE_SHIFT) / b.value;
		return f;
	}
};


inline fixed_point::fixed_point(void)
{
	value = 0;
}
inline fixed_point::fixed_point(const fixed_point &a)
{
	value = a.value;
}

inline fixed_point::fixed_point(unsigned int a)
{
	set_value_by_unsigned_int(a);
}

inline fixed_point::fixed_point(float a)
{
	set_value_by_float(a);
}

inline float fixed_point::get_float(void)
{
	return (float)value / (1 << FP_SCALE_SHIFT);
}

inline fixed_point operator+(fixed_point a, fixed_point b)
{
	fixed_point f;
	f.value = a.value + b.value;
	return f;
}

inline fixed_point operator-(fixed_point a, fixed_point b)
{
	fixed_point f;
	f.value = a.value - b.value;
	return f;
}

inline fixed_point operator-(fixed_point a)
{
	fixed_point f;
	f.value = -a.value;
	return f;
}

inline fixed_point operator*(fixed_point a, fixed_point b)
{
	fixed_point f = fixed_point::mul(a, b);
	return f;
}

inline fixed_point operator/(fixed_point a, fixed_point b)
{
	fixed_point f = fixed_point::div(a, b);
	return f;
}

inline bool operator<(fixed_point a, fixed_point b)
{
	return a.value < b.value;
}

inline bool operator<=(fixed_point a, fixed_point b)
{
	return a.value <= b.value;
}


#endif /*FIXED_POINT_H_*/
