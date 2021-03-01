#pragma once

template<typename T>
class Vec3
{
public:
	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(T xx) : x(xx), y(xx), z(xx) {}
	Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}

	Vec3 operator+(const Vec3& v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3 operator-(const Vec3& v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3 operator*(const Vec3& v) const
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	Vec3 operator*(T r) const
	{
		return Vec3(x * r, y * r, z * r)
	}

	const T& operator[](uint8_t i) const
	{
		return (&x)[i];
	}

	T& operator[](uint8_t i)
	{
		return (&x)[i];
	}

	friend Vec3 operator*(T r, const Vec3& v)
	{
		return Vec3(v.x * r, v.y * r, v.z * r);
	}

	friend T length(const Vec3& v)
	{
		return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	friend Vec3 normalize(const Vec3& v)
	{
		T len2 = v.x * v.x + v.y * v.y + v.z * v.z;
		if (len2 > 0)
		{
			T fractor = 1 / std::sqrt(len2);
			return Vec3(v.x * fractor, v.y * fractor, v.z * fractor);
		}

		return Vec3();
	}

	friend T dot(const Vec3& a, const Vec3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	friend Vec3 cross(const Vec3& a, const Vec3& b)
	{
		return Vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	friend std::ostream& operator<<(std::ostream& s, const Vec3& v)
	{
		return s << "(" << v.x << " " << v.y << " " << v.z << ")";
	}

private:
	T x, y, z;
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
