#pragma once

#include "Matrix.h"

template<typename T>
class Quaternion
{
public:
	Quaternion() : v(T(0)), w(T(1)) {}
	Quaternion(const Quaternion<T>& q) : v(q.v), w(q.w) {}
	Quaternion(const Vec3f<T>& axis, T angle)
	{
		T degree = angle * M_PI / 180;

		v = std::sin(degree * 0.5) * axis[0];
		w = std::cos(degree * 0.5);
	}
	Quaternion(const Matrix44<T>& m)
	{
		T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
		T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
		T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
		T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

		int biggestIndex = 0;
		T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
		if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}
		if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}
		if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		T biggestVal = std::sqrt(fourBiggestSquaredMinus1 + T(1)) * T(0.5);
		T mult = T(0.25) / biggestVal;

		switch (biggestIndex)
		{
		case 0:
		{
			v = Vec3<T>(m[1][2] - m[2][1], m[2][0] - m[0][2], m[0][1] - m[1][0]) * mult;
			m = biggestVal;
			break;
		}
		case 1:
		{
			v = Vec3<T>(
				biggestVal,
				(m[0][1] + m[1][0]) * mult, 
				(m[2][0] + m[0][2]) * mult);

			w = (m[1][2] - m[2][1]) * mult;
			break;
		}	
		case 2:
		{
			v = Vec3<T>(
				(m[0][1] + m[1][0]) * mult,
				biggestVal,
				(m[1][2] + m[2][1]) * mult);

			w = (m[2][0] - m[0][2]) * mult;
			break;
		}
		case 3:
		{
			v = Vec3<T>(
				(m[2][0] + m[0][2]) * mult, 
				(m[1][2] + m[2][1]) * mult, 
				biggestVal);

			w = (m[0][1] - m[1][0]) * mult;
			break;
		}
		}
	}

	Matrix44<T> getMatrix() const
	{
		T xx = v[0] * v[0], yy = v[1] * v[1], zz = v[2] * v[2];
		T xy = v[0] * v[1], xz = v[0] * v[2], yz = v[1] * v[2];
		T wx = v[0] * w, wy = v[1] * w, wz = v[2] * w;

		Matrix44<T> mat;
		mat[0][0] = 1 - 2 * (yy + zz);
		mat[0][1] = 2 * (xy + wz);
		mat[0][2] = 2 * (xz - wy);

		mat[1][0] = 2 * (xy - wz);
		mat[1][1] = 1 - 2 * (xx + zz);
		mat[1][2] = 2 * (yz + wx);

		mat[2][0] = 2 * (xz + wy);
		mat[2][1] = 2 * (yz - wx);
		mat[2][2] = 1 - 2 * (xx + yy);

		return mat;
	}

	Quaternion<T> operator-() const
	{
		Quaternion<T> ret;
		ret.v = -v;
		ret.w = -w;

		return ret;
	}

	Quaternion<T> operator+(const Quaternion& q) const
	{
		Quaternion<T> ret;
		ret.v = v + q.v;
		ret.w = w + q.w;

		return ret;
	}

	Quaternion<T> operator-(const Quaternion& q) const
	{
		Quaternion<T> ret;
		ret.v = v - q.v;
		ret.w = w - q.w;

		return ret;
	}

	Quaternion<T> operator*(T r) const
	{
		Quaternion<T> ret;
		ret.v = v * r;
		ret.w = w * r;

		return ret;
	}

	Quaternion<T> operator/(T r) const
	{
		Quaternion<T> ret;
		ret.v = v / r;
		ret.w = w / r;

		return ret;
	}

	Quaternion<T> operator+=(const Quaternion& q)
	{
		v += q.v;
		w += q.w;

		return *this;
	}

	Quaternion<T> operator-=(const Quaternion& q)
	{
		v -= q.v;
		w -= q.w;

		return *this;
	}

	Quaternion<T> operator*=(T r)
	{
		v *= r;
		w *= r;

		return *this;
	}

	Quaternion<T> operator/=(T r)
	{
		v /= r;
		w /= r;

		return *this;
	}

	friend Quaternion<T> operator*(T r, const Quaternion<T>& q)
	{
		Quaternion<T> ret;
		ret.v = q.v * r;
		ret.w = q.w * r;

		return ret;
	}

	friend Quaternion<T> operator/(T r, const Quaternion<T>& q)
	{
		Quaternion<T> ret;
		ret.v = q.v / r;
		ret.w = q.w / r;

		return ret;
	}

	friend T dot(const Quaternion<T>& q0, const Quaternion<T>& q1)
	{
		return dot(q0.v, q1.v) + q0.w * q1.w;
	}

	friend Quaternion<T> normalize(const Quaternion<T>& q)
	{
		return q / std::sqrt(dot(q, q));
	}

	friend Quaternion<T> lerp(const Quaternion<T>& q0, const Quaternion<T>& q1, T t)
	{
		return (T(1) - t) * q0 + t * q1;
	}

	friend Quaternion<T> lerp(const Quaternion<T>& q0, const Quaternion<T>& q1, T t)
	{
		Quaternion<T> q2 = q1;

		T costheta = dot(q0, q1);
		if (costheta < T(0))
		{
			q2 = -q1;
			costheta = -costheta;
		}

		if (costheta > T(0.9995))
		{
			return lerp(q0, q1, t);
		}

		T theta = std::acos(costheta);
		return (std::sin((T(1) - t) * theta) * q0 + std::sin(t * theta) * q2) / std::sin(theta);
	}

private:
	Vec3<T> v;
	T w;
};