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
		T n, T o, T p, T q)
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

		m[3][0] = n;
		m[3][1] = o;
		m[3][2] = p;
		m[3][3] = q;
	}
	
	const T* operator[](int i) const
	{
		return m[i];
	}

	T* operator[](int i)
	{
		return m[i];
	}

	bool operator==(const Matrix44<T>& mat) const
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (m[i][j] != mat[i][j])
				{
					return false;
				}
			}
		}

		return true;
	}

	bool operator!=(const Matrix44<T>& mat) const
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (m[i][j] != mat[i][j])
				{
					return true;
				}
			}
		}

		return false;
	}

	Matrix44<T> operator+(T r) const
	{
		return Matrix44(
			m[0][0] + r, m[0][1] + r, m[0][2] + r, m[0][3] + r,
			m[1][0] + r, m[1][1] + r, m[1][2] + r, m[1][3] + r,
			m[2][0] + r, m[2][1] + r, m[2][2] + r, m[2][3] + r,	
			m[3][0] + r, m[3][1] + r, m[3][2] + r, m[3][3] + r);
	}

	Matrix44<T> operator+(const Matrix44<T>& mat) const
	{
		return Matrix44(
			m[0][0] + mat[0][0], 
			m[0][1] + mat[0][1], 
			m[0][2] + mat[0][2],
			m[0][3] + mat[0][3],
			
			m[1][0] + mat[1][0],
			m[1][1] + mat[1][1],
			m[1][2] + mat[1][2],
			m[1][3] + mat[1][3],

			m[2][0] + mat[2][0],
			m[2][1] + mat[2][1],
			m[2][2] + mat[2][2],
			m[2][3] + mat[2][3],

			m[3][0] + mat[3][0],
			m[3][1] + mat[3][1],
			m[3][2] + mat[3][2],
			m[3][3] + mat[3][3]);
	}

	Matrix44<T> operator-(T r) const
	{
		return Matrix44(
			m[0][0] - r, m[0][1] - r, m[0][2] - r, m[0][3] - r,
			m[1][0] - r, m[1][1] - r, m[1][2] - r, m[1][3] - r,
			m[2][0] - r, m[2][1] - r, m[2][2] - r, m[2][3] - r,
			m[3][0] - r, m[3][1] - r, m[3][2] - r, m[3][3] - r);
	}

	Matrix44<T> operator-(const Matrix44<T>& mat) const
	{
		return Matrix44(
			m[0][0] - mat[0][0],
			m[0][1] - mat[0][1],
			m[0][2] - mat[0][2],
			m[0][3] - mat[0][3],

			m[1][0] - mat[1][0],
			m[1][1] - mat[1][1],
			m[1][2] - mat[1][2],
			m[1][3] - mat[1][3],

			m[2][0] - mat[2][0],
			m[2][1] - mat[2][1],
			m[2][2] - mat[2][2],
			m[2][3] - mat[2][3],

			m[3][0] - mat[3][0],
			m[3][1] - mat[3][1],
			m[3][2] - mat[3][2],
			m[3][3] - mat[3][3]);
	}

	Matrix44<T> operator*(T r) const
	{
		return Matrix44(
			m[0][0] * r, m[0][1] * r, m[0][2] * r, m[0][3] * r,
			m[1][0] * r, m[1][1] * r, m[1][2] * r, m[1][3] * r,
			m[2][0] * r, m[2][1] * r, m[2][2] * r, m[2][3] * r,
			m[3][0] * r, m[3][1] * r, m[3][2] * r, m[3][3] * r);
	}

	Matrix44<T> operator*(const Matrix44<T>& rhs) const
	{
		Matrix44<T> mult;

#if 0
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
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
		T* __restrict cp        = &mult.m[0][0];

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

	Matrix44<T> operator/(T r) const
	{
		r = T(1) / r;

		return Matrix44(
			m[0][0] * r, m[0][1] * r, m[0][2] * r, m[0][3] * r,
			m[1][0] * r, m[1][1] * r, m[1][2] * r, m[1][3] * r,
			m[2][0] * r, m[2][1] * r, m[2][2] * r, m[2][3] * r,
			m[3][0] * r, m[3][1] * r, m[3][2] * r, m[3][3] * r);
	}

	friend Matrix44<T> operator+(T r, const Matrix44<T>& mat)
	{
		return Matrix44(
			mat[0][0] + r, mat[0][1] + r, mat[0][2] + r, mat[0][3] + r,
			mat[1][0] + r, mat[1][1] + r, mat[1][2] + r, mat[1][3] + r,
			mat[2][0] + r, mat[2][1] + r, mat[2][2] + r, mat[2][3] + r,
			mat[3][0] + r, mat[3][1] + r, mat[3][2] + r, mat[3][3] + r);
	}

#ifdef ROW_MAJOR

	friend Vec3<T> operator*(const Vec3<T>& v, const Matrix44<T>& mat)
	{
		T x, y, z, w;

		x = v[0] * mat[0][0] + v[1] * mat[1][0] + v[2] * mat[2][0] + mat[3][0];
		y = v[0] * mat[0][1] + v[1] * mat[1][1] + v[2] * mat[2][1] + mat[3][1];
		z = v[0] * mat[0][2] + v[1] * mat[1][2] + v[2] * mat[2][2] + mat[3][2];
		w = v[0] * mat[0][3] + v[1] * mat[1][3] + v[2] * mat[2][3] + mat[3][3];

		return Vec3<T>(x / w, y / w, z / w);
	}

#else

	friend Vec3<T> operator*(const Matrix44<T>& mat, const Vec3<T>& v)
	{
		T x, y, z, w;

		x = v[0] * mat[0][0] + v[1] * mat[0][1] + v[2] * mat[0][2] + mat[0][3];
		y = v[0] * mat[1][0] + v[1] * mat[1][1] + v[2] * mat[1][2] + mat[1][3];
		z = v[0] * mat[2][0] + v[1] * mat[2][1] + v[2] * mat[2][2] + mat[2][3];
		w = v[0] * mat[3][0] + v[1] * mat[3][1] + v[2] * mat[3][2] + mat[3][3];

		return Vec3<T>(x / w, y / w, z / w);
	}

#endif

	friend Matrix44<T> transpose(const Matrix44<T>& mat)
	{
#if 0
		Matrix44<T> transpMat;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
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

	friend Matrix44<T> inverse(const Matrix44<T>& mat)
	{
		int i, j, k;
		Matrix44<T> s;
		Matrix44<T> t(mat);
		
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

			if (pivotsize == T(0))
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

	friend Matrix44<T> translate(const Matrix44<T>& mat, const Vec3<T>& v)
	{
		Matrix44<T> trans;

#ifdef ROW_MAJOR

		trans[3][0] = v[0];
		trans[3][1] = v[1];
		trans[3][2] = v[2];

		return mat * trans;

#else

		trans[0][3] = v[0];
		trans[1][3] = v[1];
		trans[2][3] = v[2];

		return trans * mat;

#endif
	}

	friend Matrix44<T> scale(const Matrix44<T>& mat, const Vec3<T>& v)
	{
		Matrix44<T> s;
		s[0][0] = v[0];
		s[1][1] = v[1];
		s[2][2] = v[2];

#ifdef ROW_MAJOR

		return mat * s;

#else

		return s * mat;

#endif
	}

	friend Matrix44<T> rotate(const Matrix44<T>& mat, const Vec3<T>& v, T angle)
	{
		Vec3<T> a = normalize(v);
		
		T degree = M_PI * angle / 180;
		T c = std::cos(degree);
		T s = std::sin(degree);

		Vec3<T> X(
			a[0] * a[0] * (1 - c) + c,
			a[0] * a[1] * (1 - c) + a[2] * s,
			a[0] * a[2] * (1 - c) - a[1] * s);

		Vec3<T> Y(
			a[0] * a[1] * (1 - c) - a[2] * s,
			a[1] * a[1] * (1 - c) + c,
			a[1] * a[2] * (1 - c) + a[0] * s);

		Vec3<T> Z(
			a[0] * a[2] * (1 - c) + a[1] * s,
			a[1] * a[2] * (1 - c) - a[0] * s,
			a[2] * a[2] * (1 - c) + c);

#ifdef ROW_MAJOR

		Matrix44<T> rota(
			X[0], X[1], X[2], T(0),
			Y[0], Y[1], Y[2], T(0),
			Z[0], Z[1], Z[2], T(0),
			T(0), T(0), T(0), T(1));

		return mat * rota;

#else

		Matrix44<T> rota(
			X[0], Y[0], Z[0], T(0),
			X[1], Y[1], Z[1], T(0),
			X[2], Y[2], Z[2], T(0),
			T(0), T(0), T(0), T(1));

		return rota * mat;

#endif 
	}


	friend std::ostream& operator<<(std::ostream& os, const Matrix44<T>& mat)
	{
		std::ios_base::fmtflags oldFlags = os.flags();
		int width = 12;
		os.precision(5);
		os.setf(std::ios_base::fixed);

		os << "(" << std::setw(width) << mat[0][0] <<
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

		os.flags(oldFlags);
		return os;
	}

private:
	T m[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
};

typedef Matrix44<float> Matrix44f;
