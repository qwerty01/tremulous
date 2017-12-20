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


void Vec3::RotatePointAroundVector( const Vec3 dir, const Vec3 point, float degrees )
{
	float cos_ia = DEG2RAD(degrees);
	float sin_a = sin(cos_ia);
	float cos_a = cos(cos_ia);
	cos_ia = 1.0F - cos_a;

	float i_i_ia = dir[0] * dir[0] * cos_ia;
	float j_j_ia = dir[1] * dir[1] * cos_ia;
	float k_k_ia = dir[2] * dir[2] * cos_ia;
	float i_j_ia = dir[0] * dir[1] * cos_ia;
	float i_k_ia = dir[0] * dir[2] * cos_ia;
	float j_k_ia = dir[1] * dir[2] * cos_ia;

	float a_sin = dir[0] * sin_a;
	float b_sin = dir[1] * sin_a;
	float c_sin = dir[2] * sin_a;

	float rot[3][3];
	rot[0][0] = i_i_ia + cos_a;
	rot[0][1] = i_j_ia - c_sin;
	rot[0][2] = i_k_ia + b_sin;
	rot[1][0] = i_j_ia + c_sin;
	rot[1][1] = j_j_ia + cos_a;
	rot[1][2] = j_k_ia - a_sin;
	rot[2][0] = i_k_ia - b_sin;
	rot[2][1] = j_k_ia + a_sin;
	rot[2][2] = k_k_ia + cos_a;

	this->x = point[0] * rot[0][0] + point[1] * rot[0][1] + point[2] * rot[0][2];
	this->y = point[0] * rot[1][0] + point[1] * rot[1][1] + point[2] * rot[1][2];
	this->z = point[0] * rot[2][0] + point[1] * rot[2][1] + point[2] * rot[2][2];
}

void Vec3::AngleVectors(const Vec3& angles, Vec3* forward, Vec3* right, Vec3* up)
{
	float angle = angles[YAW] * (M_PI*2 / 360);
	float sy = sin(angle);
	float cy = cos(angle);

	angle = angles[PITCH] * (M_PI*2 / 360);
	float sp = sin(angle);
	float cp = cos(angle);

	angle = angles[ROLL] * (M_PI*2 / 360);
	float sr = sin(angle);
	float cr = cos(angle);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}
	if (right)
	{
		right->x = (-1*sr*sp*cy+-1*cr*-sy);
		right->y = (-1*sr*sp*sy+-1*cr*cy);
		right->z = -1*sr*cp;
	}
	if (up)
	{
		up->x = (cr*sp*cy+-sr*-sy);
		up->y = (cr*sp*sy+-sr*cy);
		up->z = cr*cp;
	}
}

