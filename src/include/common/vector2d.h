
#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>

#define		MATH_EPSILON		1.0e-5f			// Tolerance for float

template<class T> struct TVector2
{
	union 
	{
		struct 
		{
			T	x,y; // 3 real components of the vector
		};

		T	v[2]; // Array access useful in loops
	};

	// =========================================================================
	// ���캯��
	//==========================================================================
	TVector2(){}
	TVector2( const T x, const T y ) : x(x), y(y) {}
	TVector2( const TVector2& v ) : x(v.x), y(v.y) {}
	TVector2( const T f[2]){ v[0] = f[0]; v[1] = f[1]; }

	// =========================================================================
	// ��ʼ������
	// =========================================================================
	void Init( T a=0, T b=0);
	void Init(const TVector2& v);
	void Init(const T f[2]);

	// =========================================================================
	// ʸ���Ļ�������
	// =========================================================================		
	// ʸ�����ȵ�ƽ��
	T MagSqr() const;
	// ʸ���ĳ���
	T Mag() const;
	// ʸ�����ȵĿ��ټ���
	T MagFast() const;

	// ����һ��ԭʸ���ĵ�λʸ����ԭʸ�����ı�
	const TVector2 Unit() const;
	// ������λ��
	const TVector2& Normalize();
	// 
	void Norm(T nVal = 1);
	// ʸ����λ���Ŀ��ټ���
	void NormFast(T nVal = 1);


	// �ж�һ��ʸ���Ƿ������(��ָ���ľ�����) ��fTolΪ�Ƚϵľ���
	bool IsZero(float fTol = MATH_EPSILON)	const;
	// �ж��Ƿ������һ��ʸ��(��ָ���ľ�����) ��fTolΪ�Ƚϵľ���
	bool IsEqual(const TVector2& b, const T r = MATH_EPSILON) const;
	// �ж�һ��ʸ���ĳ����Ƿ����len
	bool IsLongerThan(T len) const;
	// ��������һ��ʸ����ľ���
	float Dist( const TVector2& right ) const;

	// =====================================================================
	// ʸ��������
	// =====================================================================
	// ʸ���ĵ��
	T Dot( const TVector2& b ) const;

	TVector2	GetMaxAxis()const;
	TVector2	GetMinAxis()const;

	// =====================================================================
	// ����������
	// =====================================================================
	const T& operator [] ( const int i ) const;
	T& operator [] ( const int i );

	const TVector2 operator + (const TVector2& b) const;
	const TVector2 operator - (const TVector2& b) const;
	const TVector2 operator * (const T s) const;
	const TVector2 operator / (const T s) const;
	const TVector2 operator - () const;

	friend inline const TVector2 operator * (const T s, const TVector2& v)
	{
		return v * s;
	}

	const TVector2& operator = ( const TVector2& b );
	const TVector2& operator += (const TVector2& b); 
	const TVector2& operator -= (const TVector2& b) ;
	const TVector2& operator *= (const T s);
	const TVector2& operator /= (const T s);

	// ���
	T operator * (const TVector2& v) const;    
	// ���
	TVector2 operator ^ (const TVector2& v) const;   

	bool operator == ( const TVector2& b ) const;
	bool operator != ( const TVector2& b ) const;

	bool operator > (const TVector2 &other) const;
	bool operator < (const TVector2 &other) const; 
	bool operator >= (const TVector2 &other) const; 
	bool operator <= (const TVector2 &other) const; 
};

//====================================================================
// ��ʼ������
//====================================================================
template<class T> inline void TVector2<T>::Init( T a, T b)
{
	x = a;
	y = b;
}

template<class T> inline void TVector2<T>::Init(const TVector2& v)
{
	x = v.x;
	y = v.y;
}

template<class T> inline void TVector2<T>::Init(const T f[2])
{
	v[0] = f[0];
	v[1] = f[1];
}

//========================================================================
// ʸ���Ļ�������
//========================================================================
// ʸ���ĳ���
template<class T> inline T TVector2<T>::Mag() const
{
	return static_cast<T>(sqrtf( static_cast<float>(x*x + y*y) ));
}

// ʸ�����ȵ�ƽ��
template<class T> inline T TVector2<T>::MagSqr() const
{
	return x*x + y*y;
}

// ����һ��ԭʸ���ĵ�λʸ����ԭʸ�����ı�
template<class T> inline const TVector2<T> TVector2<T>::Unit() const
{
	float fLen = this->Mag();
	return (*this) / fLen;
}

// ������λ��
template<class T> inline const TVector2<T>& TVector2<T>::Normalize()
{
	float fLen = this->Mag();
	(*this) /= fLen;

	return *this;
}

inline bool gIsZero( float a, float fTol = MATH_EPSILON)
{ 
	return ( a <= 0.0f ) ? ( a >= -fTol ) : ( a <= fTol ); 
}

// �ж�һ��ʸ���Ƿ������(��ָ���ľ�����) ��fTolΪ�Ƚϵľ���
template<class T> inline bool TVector2<T>::IsZero(float fTol)	const
{
	if( gIsZero(x, fTol) && gIsZero(y, fTol))
		return true;
	else
		return false;
}

// �ж��Ƿ������һ��ʸ��(��ָ���ľ�����) ��fTolΪ�Ƚϵľ���
template<class T> inline bool TVector2<T>::IsEqual(const TVector2& b, const T r) const
{
	//within a tolerance
	const TVector2 t = *this - b;//difference

	return t.Dot(t) <= r*r;//radius
}

// �ж�һ��ʸ���ĳ����Ƿ����len
template<class T> inline bool TVector2<T>::IsLongerThan(T len) const
{
	return (x*x + y*y) > (len * len);
}

// ��������һ��ʸ����ľ���
template<class T> inline float TVector2<T>::Dist( const TVector2& right ) const
{
	TVector2<T> DistVec( x - right.x, y - right.y );
	return static_cast<float>(DistVec.Mag());
}

//=================================================================================
// ʸ��������
//=================================================================================
// ʸ���ĵ��
template<class T> inline T TVector2<T>::Dot( const TVector2& b ) const
{
	return x*b.x + y*b.y;
}

//******************************************************************************
/*! \fn     TVector2 TVector2<T>::GetMainAxis()const
*   \brief  �õ�ʸ���������
*           ����ʸ����, x, y, z���ö�Ӧ���Ǹ���
*   \return TVector2 �õ��������
*   \sa     GetMinAxis
*******************************************************************************/
template<class T> inline TVector2<T> TVector2<T>::GetMaxAxis()const
{
	if (x >= y)
		return TVector2(1.0f, 0.0f);
	else
		return TVector2(0.0f, 1.0f);
}

//******************************************************************************
/*! \fn    TVector2 TVector2<T>::GetMinAxis()const
*   \brief  �õ�ʸ������С��
*           ����ʸ����, x, y, z��С�ö�Ӧ���Ǹ���
*   \return TVector2 �õ�����С��
*   \sa     GetMaxAxis
*******************************************************************************/
template<class T> inline TVector2<T> TVector2<T>::GetMinAxis()const
{
	if (x < y)
		return TVector2(1.0f, 0.0f);
	else
		return TVector2(0.0f, 1.0f);
}

template<class T> inline const T& TVector2<T>::operator [] ( const int i ) const
{
	return v[i];
}

template<class T> inline T& TVector2<T>::operator [] ( const int i )
{
	return v[i];
}

template<class T> inline const TVector2<T> TVector2<T>::operator + (const TVector2& b) const
{
	return TVector2(x + b.x, y + b.y);
}

template<class T> inline const TVector2<T> TVector2<T>::operator - (const TVector2& b) const
{
	return TVector2(x - b.x, y - b.y);
}

template<class T> inline const TVector2<T> TVector2<T>::operator * (const T s) const
{
	return TVector2(x*s, y*s);
}


template<class T> inline const TVector2<T> TVector2<T>::operator / (const T s) const
{
	return TVector2(x/s, y/s);
}

template<class T> inline const TVector2<T>& TVector2<T>::operator = ( const TVector2& b )
{
	x = b.x;
	y = b.y;

	return *this;
}

template<class T> inline const TVector2<T>& TVector2<T>::operator += (const TVector2& b) 
{
	x += b.x;
	y += b.y;

	return *this;
}

template<class T> inline const TVector2<T>& TVector2<T>::operator -= (const TVector2& b) 
{
	x -= b.x;
	y -= b.y;

	return *this;
}

template<class T> inline const TVector2<T>& TVector2<T>::operator *= (const T s)
{
	x *= s;
	y *= s;

	return *this;
}

template<class T> inline const TVector2<T>& TVector2<T>::operator /= (const T s)
{
	x /= s;
	y /= s;

	return *this;
}


// ���
template<class T> inline T TVector2<T>::operator * (const TVector2& b) const     
{ 
	return x*b.x + y*b.y; 
}

template<class T> inline bool TVector2<T>::operator == ( const TVector2& b ) const
{
	return (b.x==x && b.y==y);
}

template<class T> inline bool TVector2<T>::operator != ( const TVector2& b ) const
{
	return !(b == *this);
}

template<class T> inline const TVector2<T> TVector2<T>::operator - () const
{
	return TVector2( -x, -y );
}


template<class T> inline bool TVector2<T>::operator > (const TVector2 &other) const 
{
	return ((x>other.x) && (y>other.y)); 
}

template<class T> inline bool TVector2<T>::operator < (const TVector2 &other) const 
{ 
	return ((x<other.x) && (y<other.y)); 
}

template<class T> inline bool TVector2<T>::operator >= (const TVector2 &other) const 
{ 
	return ((x>=other.x) && (y>=other.y)); 
}

template<class T> inline bool TVector2<T>::operator <= (const TVector2 &other) const 
{ 
	return ((x<=other.x) && (y<=other.y)); 
}

#endif

