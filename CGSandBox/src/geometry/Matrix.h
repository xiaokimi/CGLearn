#pragma once

template<typename T>
class Vec3;

template<typename T>
class Matrix44
{
public:
	Matrix44() {}

	const T* operator[](const uint8_t& i) const
	{
		return m[i];
	}

	T* operator[](const uint8_t& i)
	{
		return m[i];
	}

	Matrix44<T> operator*(const Matrix44<T>& rhs) const
	{
		Matrix44<T> mult;

		for (uint8_t i = 0; i < 4; ++i)
		{
			for (uint8_t j = 0; j < 4; ++j)
			{
				mult[i][j] = m[i][0] * rhs[0][j] +
							 m[i][1] * rhs[1][j] +
							 m[i][2] * rhs[2][j] +
							 m[i][3] * rhs[3][j];
			}
		}

		return mult;
	}

public:
	T m[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
};

typedef Matrix44<float> Matrix44f;

#ifdef ROW_MAJOR

template<typename T>
Vec3<T> operator*(const Vec3<T>& v, const Matrix44<T>& m)
{
	return Vec3<T>(
		v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
		v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
		v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]);
}

#else

template<typename T>
Vec3<T> operator*(const Matrix44<T>& m, const Vec3<T>& v)
{
	return Vec3<T>(
		v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2],
		v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2],
		v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2]);
}

#endif

template<typename T>
Matrix44<T> transpose(const Matrix44<T>& m)
{
	Matrix44<T> transpMat;

	for (uint8_t i = 0; i < 4; ++i)
	{
		for (uint8_t j = 0; j < 4; ++j)
		{
			transpMat[i][j] = m[j][i];
		}
	}

	return transpMat;
}