#pragma once

template<typename T>
class Vec2
{
public:
	Vec2() : x(T(0)), y(T(0)) {}
	Vec2(T xx) : x(xx), y(xx) {}
	Vec2(T xx, T yy) : x(xx), y(yy) {}
	Vec2(const Vec2<T>& v) : x(v.x), y(v.y) {}

	Vec2<T>& operator=(const Vec2<T>& v)
	{
		x = v.x;
		y = v.y;

		return *this;
	}

	const T& operator[](int i) const
	{
		return (&x)[i];
	}

	T& operator[](int i)
	{
		return (&x)[i];
	}

	Vec2<T> operator+(const Vec2<T>& v) const
	{
		return Vec2<T>(x + v.x, y + v.y);
	}

	Vec2<T> operator+(T r) const
	{
		return Vec2<T>(x + r, y + r);
	}

	Vec2<T> operator-(const Vec2<T>& v) const
	{
		return Vec2<T>(x - v.x, y - v.y);
	}

	Vec2<T> operator-(T r) const
	{
		return Vec2<T>(x - r, y - r);
	}

	Vec2<T> operator*(T r) const
	{
		return Vec2<T>(x * r, y * r);
	}

	Vec2<T> operator/(T r) const
	{
		return Vec2<T>(x / r, y / r);
	}

	Vec2<T>& operator+=(const Vec2<T>& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vec2<T>& operator+=(T r)
	{
		x += r;
		y += r;

		return *this;
	}

	Vec2<T>& operator-=(const Vec2<T>& v)
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	Vec2<T>& operator-=(T r)
	{
		x -= r;
		y -= r;

		return *this;
	}

	Vec2<T>& operator*=(T r)
	{
		x *= r;
		y *= r;

		return *this;
	}

	Vec2& operator/=(T r)
	{
		x /= r;
		y /= r;

		return *this;
	}

	friend Vec2<T> operator*(T r, const Vec2<T>& v)
	{
		return Vec2<T>(v.x * r, v.y * r);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
	{
		return os << "(" << v.x << " " << v.y << ")";
	}

private:
	T x, y;
};

typedef Vec2<float> Vec2f;

template<typename T>
class Vec3
{
public:
	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(T xx) : x(xx), y(xx), z(xx) {}
	Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	Vec3(const Vec3<T>& v) : x(v.x), y(v.y), z(v.z) {}

	Vec3<T>& operator=(const Vec3<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	int getMaxExtend() const
	{
		if (x > y && x > z)
		{
			return 0;
		}
		else if (y > z)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}

	static Vec3<T> min(const Vec3<T>& a, const Vec3<T>& b)
	{
		T xMin = std::min(a.x, b.x);
		T yMin = std::min(a.y, b.y);
		T zMin = std::min(a.z, b.z);

		return Vec3<T>(xMin, yMin, zMin);
	}

	static Vec3<T> max(const Vec3<T>& a, const Vec3<T>& b)
	{
		T xMax = std::max(a.x, b.x);
		T yMax = std::max(a.y, b.y);
		T zMax = std::max(a.z, b.z);

		return Vec3<T>(xMax, yMax, zMax);
	}

	const T& operator[](int i) const
	{
		return (&x)[i];
	}

	T& operator[](int i)
	{
		return (&x)[i];
	}

	Vec3<T> operator+(const Vec3<T>& v) const
	{
		return Vec3<T>(x + v.x, y + v.y, z + v.z);
	}

	Vec3<T> operator+(T r) const
	{
		return Vec3<T>(x + r, y + r, z + r);
	}

	Vec3<T> operator-(const Vec3<T>& v) const
	{
		return Vec3<T>(x - v.x, y - v.y, z - v.z);
	}

	Vec3<T> operator-(T r) const
	{
		return Vec3<T>(x - r, y - r, z - r);
	}

	Vec3<T> operator*(const Vec3<T>& v) const
	{
		return Vec3<T>(x * v.x, y * v.y, z * v.z);
	}

	Vec3<T> operator*(T r) const
	{
		return Vec3<T>(x * r, y * r, z * r);
	}

	Vec3<T> operator/(T r) const
	{
		return Vec3<T>(x / r, y / r, z / r);
	}

	Vec3<T>& operator+=(const Vec3<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vec3<T>& operator+=(T r)
	{
		x += r;
		y += r;
		z += r;

		return *this;
	}

	Vec3<T>& operator-=(const Vec3<T>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vec3<T>& operator-=(T r)
	{
		x -= r;
		y -= r;
		z -= r;

		return *this;
	}

	Vec3<T>& operator*=(T r)
	{
		x *= r;
		y *= r;
		z *= r;

		return *this;
	}

	Vec3<T>& operator/=(T r)
	{
		x /= r;
		y /= r;
		z /= r;

		return *this;
	}

	friend Vec3<T> operator*(T r, const Vec3<T>& v)
	{
		return Vec3<T>(v.x * r, v.y * r, v.z * r);
	}

	friend T length(const Vec3<T>& v)
	{
		return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	friend Vec3<T> normalize(const Vec3<T>& v)
	{
		T len2 = v.x * v.x + v.y * v.y + v.z * v.z;
		if (len2 > 0)
		{
			T fractor = 1 / std::sqrt(len2);
			return Vec3<T>(v.x * fractor, v.y * fractor, v.z * fractor);
		}

		return Vec3<T>();
	}

	friend T dot(const Vec3<T>& a, const Vec3<T>& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	friend Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	friend Vec3<T> reflect(const Vec3<T>& I, const Vec3<T>& N)
	{
		return I - 2 * dot(I, N) * N;
	}

	friend bool refract(const Vec3<T>& I, const Vec3<T>& N, float ior, Vec3<T>& refracted)
	{
		float etai = 1.0f;
		float etat = ior;
		Vec3<T> normal = N;

		float cosi = dot(I, N);
		if (cosi > 0)
		{
			cosi = -cosi;
			normal = -N;
			std::swap(etai, etat);
		}

		float eta = etai / etat;
		float discriminant = 1.0f - eta * eta * (1.0f - cosi * cosi);
		if (discriminant > 0.0f)
		{
			refracted = eta * I - (eta * cosi + std::sqrtf(discriminant)) * normal;
			return true;
		}

		return false;
	}

	friend std::ostream& operator<<(std::ostream& s, const Vec3<T>& v)
	{
		return s << "(" << v.x << " " << v.y << " " << v.z << ")";
	}

private:
	T x, y, z;
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
