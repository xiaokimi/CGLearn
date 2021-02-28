#pragma once

template<typename T>
class Vec3
{
public:
	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(const T& xx) : x(xx), y(xx), z(xx) {}
	Vec3(const T& xx, const T& yy, const T& zz) : x(xx), y(yy), z(zz) {}
	//~virtual Vec3() {}

	Vec3<T> operator+(const Vec3<T>& v) const
	{
		return Vec3<T>(x + v.x, y + v.y, z + v.z);
	}

	Vec3<T> operator-(const Vec3<T>& v) const
	{
		return Vec3<T>(x - v.x, y - v.y, z - v.z);
	}

	Vec3<T> operator*(const Vec3<T>& v) const
	{
		return Vec3<T>(x * v.x, y * v.y, z * v.z);
	}

public:
	T x, y, z;
};

typedef Vec3<float> Vec3f;

template<typename T>
T length(const Vec3<T>& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template<typename T>
Vec3<T> normalize(const Vec3<T>& v)
{
	T len2 = v.x * v.x + v.y * v.y + v.z * v.z;
	if (len2 > 0)
	{
		T invLen = 1 / std::sqrt(len2);
		return Vec3<T>(v.x * invLen, v.y * invLen, v.z * invLen);
	}

	return Vec3<T>();
}

template<typename T>
T dot(const Vec3<T>& a, const Vec3<T>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b)
{
	return Vec3<T>(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}