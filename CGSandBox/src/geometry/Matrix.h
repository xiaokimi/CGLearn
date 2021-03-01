#pragma once

template<typename T>
class Vec3;

template<typename T>
class Matrix44
{
public:
	Matrix44() {}
	Matrix44(
		T a, T b, T c, T d,
		T e, T f, T g, T h,
		T i, T j, T k, T l,
		T m, T n, T o, T p)
	{
		m[0][0] = a;
		m[0][1] = b;
		m[0][2] = c;
		m[0][3] = d;

		m[1][0] = e;
		m[1][1] = f;
		m[1][2] = g;
		m[1][3] = h;

		m[2][0] = i;
		m[2][1] = j;
		m[2][2] = k;
		m[2][3] = l;

		m[3][0] = m;
		m[3][1] = n;
		m[3][2] = o;
		m[3][3] = p;
	}
	
	const T* operator[](uint8_t i) const
	{
		return m[i];
	}

	T* operator[](uint8_t i)
	{
		return m[i];
	}

	Matrix44 operator*(const Matrix44& rhs) const
	{
		Matrix44 mult;

#if 0
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
#else
		const T* __restrict ap  = &m[0][0];
		const T* __restrict bp  = &rhs.m[0][0];
		T* __restrict cp		= &mult.m[0][0];

		T a0, a1, a2, a3;

		a0 = ap[0];
		a1 = ap[1];
		a2 = ap[2];
		a3 = ap[3];

		cp[0] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8]  + a3 * bp[12];
		cp[1] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9]  + a3 * bp[13];
		cp[2] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
		cp[3] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

		a0 = ap[4];
		a1 = ap[5];
		a2 = ap[6];
		a3 = ap[7];

		cp[4] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8]  + a3 * bp[12];
		cp[5] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9]  + a3 * bp[13];
		cp[6] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
		cp[7] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

		a0 = ap[8];
		a1 = ap[9];
		a2 = ap[10];
		a3 = ap[11];

		cp[8]  = a0 * bp[0] + a1 * bp[4] + a2 * bp[8]  + a3 * bp[12];
		cp[9]  = a0 * bp[1] + a1 * bp[5] + a2 * bp[9]  + a3 * bp[13];
		cp[10] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
		cp[11] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

		a0 = ap[12];
		a1 = ap[13];
		a2 = ap[14];
		a3 = ap[15];

		cp[12] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8]  + a3 * bp[12];
		cp[13] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9]  + a3 * bp[13];
		cp[14] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
		cp[15] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];
#endif

		return mult;
	}

#ifdef ROW_MAJOR

	friend Vec3<T> operator*(const Vec3<T>& v, const Matrix44& mat)
	{
		T x, y, z, w;

		x = v[0] * mat[0][0] + v[1] * mat[1][0] + v[2] * mat[2][0] + mat[3][0];
		y = v[0] * mat[0][1] + v[1] * mat[1][1] + v[2] * mat[2][1] + mat[3][1];
		z = v[0] * mat[0][2] + v[1] * mat[1][2] + v[2] * mat[2][2] + mat[3][2];
		w = v[0] * mat[0][3] + v[1] * mat[1][3] + v[2] * mat[2][3] + mat[3][3];

		return Vec3<T>(x / w, y / w, z / w);
	}

#else

	friend Vec3<T> operator*(const Matrix44& mat, const Vec3<T>& v)
	{
		T x, y, z, w;

		x = v[0] * mat[0][0] + v[1] * mat[0][1] + v[2] * mat[0][2] + mat[0][3];
		y = v[0] * mat[1][0] + v[1] * mat[1][1] + v[2] * mat[1][2] + mat[1][3];
		z = v[0] * mat[2][0] + v[1] * mat[2][1] + v[2] * mat[2][2] + mat[2][3];
		w = v[0] * mat[3][0] + v[1] * mat[3][1] + v[2] * mat[3][2] + mat[3][3];

		return Vec3<T>(x / w, y / w, z / w);
	}

#endif

	friend Matrix44 transposed(const Matrix44& mat)
	{
#if 0
		Matrix44 transpMat;

		for (uint8_t i = 0; i < 4; ++i)
		{
			for (uint8_t j = 0; j < 4; ++j)
			{
				transpMat[i][j] = m[j][i];
			}
		}

		return transpMat;
#else
		return Matrix44(
			mat[0][0], mat[1][0], mat[2][0], mat[3][0],
			mat[0][1], mat[1][1], mat[2][1], mat[3][1],
			mat[0][2], mat[1][2], mat[2][2], mat[3][2],
			mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
#endif
	}

	friend Matrix44 inverse(const Matrix44& mat)
	{
		int i, j, k;
		Matrix44 s;
		Matrix44 t(mat);
		
		// forward elimination
		for (i = 0; i < 3; i++)
		{
			int pivot = i;

			T pivotsize = t[i][i];

			if (pivotsize < 0)
			{
				pivotsize = -pivotsize;
			}

			for (j = i + 1; j < 4; j++)
			{
				T tmp = t[j][i];

				if (tmp < 0)
				{
					tmp = -tmp;
				}

				if (tmp > pivotsize)
				{
					pivot = j;
					pivotsize = tmp;
				}
			}

			if (pivotsize == 0)
			{
				// Cannot invert singular matrix
				return Matrix44();
			}

			if (pivot != i)
			{
				for (j = 0; j < 4; j++)
				{
					T tmp;

					tmp = t[i][j];
					t[i][j] = t[pivot][j];
					t[pivot][j] = tmp;

					tmp = s[i][j];
					s[i][j] = s[pivot][j];
					s[pivot][j] = tmp;
				}
			}

			for (j = i + 1; j < 4; j++)
			{
				T f = t[j][i] / t[i][i];
				
				for (k = 0; k < 4; k++)
				{
					t[j][k] -= f * t[i][k];
					t[j][k] -= f * s[i][k];
				}
			}
		}

		// Backward substitution
		for (i = 3; i >= 0; --i)
		{
			T f;

			if ((f = t[i][i]) == 0)
			{
				// Cannot invert singular matrix
				return Matrix44();
			}

			for (j = 0; j < 4; j++)
			{
				t[i][j] /= f;
				s[i][j] /= f;
			}

			for (j = 0; j < i; j++)
			{
				f = t[i][j];

				for (k = 0; k < 4; k++)
				{
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		return s;
	}

	friend std::ostream& operator<<(std::ostream& s, const Matrix44& mat)
	{
		std::ios_base::fmtflags oldFlags = s.flags();
		int width = 12;
		s.precision(5);
		s.setf(std::ios_base::fixed);

		s << "(" << std::setw(width) << mat[0][0] <<
			 " " << std::setw(width) << mat[0][1] <<
			 " " << std::setw(width) << mat[0][2] <<
			 " " << std::setw(width) << mat[0][3] << "\n" <<

			 " " << std::setw(width) << mat[1][0] <<
			 " " << std::setw(width) << mat[1][1] <<
			 " " << std::setw(width) << mat[1][2] <<
			 " " << std::setw(width) << mat[1][3] << "\n" <<

			 " " << std::setw(width) << mat[2][0] <<
			 " " << std::setw(width) << mat[2][1] <<
			 " " << std::setw(width) << mat[2][2] <<
			 " " << std::setw(width) << mat[2][3] << "\n" <<

			 " " << std::setw(width) << mat[3][0] <<
			 " " << std::setw(width) << mat[3][1] <<
			 " " << std::setw(width) << mat[3][2] <<
			 " " << std::setw(width) << mat[3][3] << ")\n";

		s.flags(oldFlags);
		return s;
	}

private:
	T m[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
};

typedef Matrix44<float> Matrix44f;