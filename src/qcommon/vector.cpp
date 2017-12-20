#include "vector.h"

Vec3::Vec3() {}

Vec3::Vec3(const float x, const float y, const float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

float Vec3::operator[](const int i) const
{
    return (&x)[i];
}

float& Vec3::operator[](const int i)
{
    return (&x)[i];
}

void Vec3::Set(const float x, const float y, const float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vec3::Clear()
{
    x = y = z = 0.0f;
}

Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);
}

Vec3& Vec3::operator=(const Vec3& a)
{
    x = a.x;
    y = a.y;
    z = a.z;
    return *this;
}

Vec3 Vec3::operator-(const Vec3& a) const
{ 
    return Vec3(x - a.x, y - a.y, z - a.z);
}

float Vec3::operator*(const Vec3& a) const
{
    return x * a.x + y * a.y + z * a.z;
}

Vec3 Vec3::operator*(const float a) const
{
    return Vec3(x * a, y * a, z * a);
}

Vec3 Vec3::operator/(const float a) const
{
    float inva = 1.0f / a;
    return Vec3(x * inva, y * inva, z * inva);
}

Vec3 operator*(const float a, const Vec3 b)
{
    return Vec3(b.x * a, b.y * a, b.z * a);
}

Vec3 operator/(const float a, const Vec3 b)
{
    return Vec3(a / b.x, a / b.y, a / b.z);
}

Vec3 Vec3::operator+(const Vec3& a) const
{
    return Vec3(x + a.x, y + a.y, z + a.z);
}

Vec3& Vec3::operator+=(const Vec3& a)
{
    x += a.x;
    y += a.y;
    z += a.z;

    return *this;
}

Vec3& Vec3::operator/=(const Vec3& a)
{
    x /= a.x;
    y /= a.y;
    z /= a.z;

    return *this;
}

Vec3& Vec3::operator/=(const float a)
{
    float inva = 1.0f / a;
    x *= inva;
    y *= inva;
    z *= inva;

    return *this;
}

Vec3& Vec3::operator-=(const Vec3& a)
{
    x -= a.x;
    y -= a.y;
    z -= a.z;

    return *this;
}

Vec3& Vec3::operator*=(const float a)
{
    x *= a;
    y *= a;
    z *= a;

    return *this;
}

bool Vec3::Compare(const Vec3& a) const
{
    return ((x == a.x) && (y == a.y) && (z == a.z));
}

bool Vec3::Compare(const Vec3& a, const float epsilon) const
{
    if (fabs(x - a.x) > epsilon)
    {
        return false;
    }

    if (fabs(y - a.y) > epsilon)
    {
        return false;
    }

    if (fabs(z - a.z) > epsilon)
    {
        return false;
    }

    return true;
}

bool Vec3::operator==(const Vec3& a) const
{
    return Compare(a);
}

bool Vec3::operator!=(const Vec3& a) const
{
    return !Compare(a);
}

float Vec3::Normalize()
{
    float length, ilength;

    length = x * x + y * y + z * z;

    if (length)
    {
        ilength = 1 / (float)sqrt(length);
        // ilength = idMath::InvSqrt( sqrLength );  // TODO
        length *= ilength;
        x *= ilength;
        y *= ilength;
        z *= ilength;
    }

    return length;
}

float Vec3::Normalize2()
{
    float length, ilength;

    length = x * x + y * y + z * z;

    if (length)
    {
        ilength = 1 / (float)sqrt(length);
        // invLength = idMath::InvSqrt( sqrLength );  // TODO
        length *= ilength;
        x *= ilength;
        y *= ilength;
        z *= ilength;
    }
    else
    {
        Clear();
    }

    return length;
}

Vec3 Vec3::Cross(const Vec3& a) const
{
    return Vec3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
}

Vec3& Vec3::Cross(const Vec3& a, const Vec3& b)
{
    x = a.y * b.z - a.z * b.y;
    y = a.z * b.x - a.x * b.z;
    z = a.x * b.y - a.y * b.x;
    return *this;
}

float Vec3::Length() const
{
    return (float)sqrt(x * x + y * y + z * z);
}

float Vec3::LengthSquared() const
{
    return (float)x * x + y * y + z * z;
}

void Vec3::NormalVectors( Vec3& left, Vec3& down ) const
{
	float d;
	
	d = x * x + y * y;
	if( !d )
	{
		left[0] = 1;
		left[1] = 0;
		left[2] = 0;
	}
	else
	{
		//d = idMath::InvSqrt( d ); // TODO
        d = 1 / (float)sqrt(d);
		left[0] = -y * d;
		left[1] = x * d;
		left[2] = 0;
	}
	down = left.Cross( *this );
}

void Vec3::Snap()
{
    x = round(x);
    y = round(y);
    z = round(z);
    //x = float( int( x ) );
	//y = float( int( y ) );
	//z = float( int( z ) );
}

void Vec3::Lerp( const Vec3& v1, const Vec3& v2, const float l )
{
	if( l <= 0.0f )
	{
		( *this ) = v1;
	}
	else if( l >= 1.0f )
	{
		( *this ) = v2;
	}
	else
	{
		( *this ) = v1 + l * ( v2 - v1 );
	}
}

Vec3 Vec3::ToAngles() const
{
	float forward;
	float yaw;
	float pitch;
	
	if( ( x == 0.0f ) && ( y == 0.0f ) )
	{
		yaw = 0.0f;
		if( z > 0.0f )
		{
			pitch = 90.0f;
		}
		else
		{
			pitch = 270.0f;
		}
	}
	else
	{
		yaw = RAD2DEG( atan2( y, x ) );
		if( yaw < 0.0f )
		{
			yaw += 360.0f;
		}
		
		forward = sqrt( x * x + y * y );
		pitch = RAD2DEG( atan2( z, forward ) );
		if( pitch < 0.0f )
		{
			pitch += 360.0f;
		}
	}
	
	return Vec3( -pitch, yaw, 0.0f );
}

