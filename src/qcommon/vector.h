#ifndef TREMULOUS_VECTOR_H
#define TREMULOUS_VECTOR_H

#include "mathlib.h"

class Vec3 {
public:
    float x;
    float y;
    float z;

    Vec3();
    explicit Vec3(const float xyz) { Set(xyz, xyz, xyz); }
    explicit Vec3(const float x, const float y, const float z);

    void Set(const float x, const float y, const float z);
    void Clear();

    Vec3 operator*(const float a) const;
    Vec3 operator+(const Vec3& a) const;
    Vec3 operator-() const;
    Vec3 operator-(const Vec3& a) const;
    Vec3 operator/(const float a) const;
    Vec3& operator*=(const float a);
    Vec3& operator+=(const Vec3& a);
    Vec3& operator-=(const Vec3& a);
    Vec3& operator/=(const Vec3& a);
    Vec3& operator/=(const float a);
    Vec3& operator=(const Vec3& a);
    float operator*(const Vec3& a) const;
    float operator[](const int i) const;
    float& operator[](const int i);

    friend Vec3 operator*(const float a, const Vec3 b);

    bool Compare(const Vec3& a) const;  // exact compare, no epsilon
    bool Compare(const Vec3& a, const float epsilon) const;  // compare with epsilon
    bool operator!=(const Vec3& a) const;  // exact compare, no epsilon
    bool operator==(const Vec3& a) const;  // exact compare, no epsilon

    //bool FixDegenerateNormal();  // fix degenerate axial cases
    //bool FixDenormals();  // change tiny numbers to zero

    Vec3 Cross(const Vec3& a) const;
    Vec3& Cross(const Vec3& a, const Vec3& b);
    float Length() const;
    float LengthFast() const;
    float LengthSquared() const;
    float Normalize();  // returns length
    float Normalize2();  // returns length
    float NormalizeFast();  // returns length

    //float ToYaw() const;
    //float ToPitch() const;
    //idAngles ToAngles() const;
    //idPolar3 ToPolar() const;

    void NormalVectors(Vec3& left, Vec3& down) const;  // vector should be normalized
    //void ProjectOntoPlane(const Vec3& normal, const float overBounce = 1.0f);
    //bool ProjectAlongPlane(const Vec3& normal, const float epsilon, const float overBounce = 1.0f);
    //void ProjectSelfOntoSphere(const float radius);

    void Lerp(const Vec3& v1, const Vec3& v2, const float l);
    //void SLerp(const Vec3& v1, const Vec3& v2, const float l);
	void Snap();// snap to closest integer value
};


#endif
