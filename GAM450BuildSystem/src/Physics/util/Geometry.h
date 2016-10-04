/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Geometry.h 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#ifndef H_PAINT_PHYSICS_UTIL_GEOMETRY
#define H_PAINT_PHYSICS_UTIL_GEOMETRY

#include "Base/util/math/Float4.h"
#include "Base/util/math/Float4x4.h"
#include "base/util/math/MathUtil.h"

#include <algorithm>
#include <vector>

namespace Physics
{
  float4 RotateVectorIntoBasis(float4x4 const &Orientation, float4 const &v);
  float4 RotatePointIntoBasis(float4x4 const &Orientation, float4 const &Origin, float4 const &p);
  float4 RotateVectorOutOfBasis(float4x4 const &Orientation, float4 const &v);
  float4 RotatePointOutOfBasis(float4x4 const &Orientation, float4 const &Origin, float4 const &p);

  struct Box
  {
    // assumes in local space
    float4 m_HalfExtents;
    // half extents, 1 on each axis is a unit box
    Box(float4 HalfExtents = float4(0.5f, 0.5f, 0.5f, 0.f)) 
      : m_HalfExtents(HalfExtents) {}
    Box(Box const &copy) : m_HalfExtents(copy.m_HalfExtents) {}

    float4 GetExtents(void) const { return m_HalfExtents * 2.f; }

    float GetWidth(void) const { return m_HalfExtents.x * 2.f; }
    float GetHeight(void) const { return m_HalfExtents.y * 2.f; }
    float GetDepth(void) const { return m_HalfExtents.z * 2.f; }

    void SetWidth(float fWidth) { m_HalfExtents.x = fWidth / 2.f; }
    void SetHeight(float fHeight) { m_HalfExtents.y = fHeight / 2.f; }
    void SetDepth(float fDepth) { m_HalfExtents.z = fDepth / 2.f; }

    float4 GetMin(void) const
    {
      return -m_HalfExtents;
    }

    float4 GetMax(void) const
    {
      return m_HalfExtents;
    }

    float GetVolume(void) const 
    { 
      return m_HalfExtents.x * m_HalfExtents.y * m_HalfExtents.z; 
    }

    float4x4 GetInverseInertiaTensor(void) const
    {
      // a full extent is y * y, would half an extent be 2 * y * 2 * y?
      float x = 4 * m_HalfExtents.y * m_HalfExtents.y + 4 * m_HalfExtents.z * m_HalfExtents.z;
      float y = 4 * m_HalfExtents.x * m_HalfExtents.x + 4 * m_HalfExtents.z * m_HalfExtents.z;
      float z = 4 * m_HalfExtents.x * m_HalfExtents.x + 4 * m_HalfExtents.y * m_HalfExtents.y;

      return float4x4(
        12.f / x, 0.f, 0.f, 0.f,
        0.f, 12.f / y, 0.f, 0.f,
        0.f, 0.f, 12.f / z, 0.f,
        0.f, 0.f, 0.f, 0.f);
    }
  };

  struct Sphere
  {
    float m_fRadius;
    // 0.5f radius is the unit sphere
    Sphere(float fRadius = 0.5f) : m_fRadius(fRadius) {}
    Sphere(Sphere const &copy) : m_fRadius(copy.m_fRadius) {}

    float GetVolume(void) const { return (4.f / 3.f) * PI * m_fRadius * m_fRadius * m_fRadius; }

    float4x4 GetInverseInertiaTensor(void) const
    {
      float fRadSq = m_fRadius * m_fRadius;

      return float4x4(
        (5.f * fRadSq) / 2.f, 0.f, 0.f, 0.f,
        0.f, (5.f * fRadSq) / 2.f, 0.f, 0.f,
        0.f, 0.f, (5.f * fRadSq) / 2.f, 0.f,
        0.f, 0.f, 0.f, 0.f);
    }
  };

  struct Capsule
  {
    // assumes in local space
    // the two points that make the center axis of the capsule
    float4 m_TopPoint;
    float4 m_BottomPoint;
    float m_fRadius;

    Capsule(void) 
      : m_TopPoint(0.f, 0.5f, 0.f, 0.f)
      , m_BottomPoint(0.f, -0.5f, 0.f, 0.f)
      , m_fRadius(0.5f)
    {
    }
    Capsule(float4 const &TopPoint, float4 const &BottomPoint, float fRadius)
      : m_TopPoint(TopPoint)
      , m_BottomPoint(BottomPoint)
      , m_fRadius(fRadius)
    {
    }

    float GetHeight(void) const
    {
      return (m_BottomPoint - m_TopPoint).Length();
    }

    void SetHeight(float fHeight)
    {
      float4 Normal = (m_TopPoint - m_BottomPoint);
      Normal.Normalize();

      m_TopPoint = Normal * (fHeight / 2.f);
      m_BottomPoint = -Normal * (fHeight / 2.f);
    }

    float GetVolume(void) const
    {
      float cylinderVolume = PI * m_fRadius * m_fRadius * GetHeight();
      float sphereVolume = (4.f / 3.f) * PI * m_fRadius * m_fRadius * m_fRadius;
      return cylinderVolume + sphereVolume;
    }

    float4x4 GetInvereInertiaTensor(void) const
    {
      float fHeight = GetHeight();

      float x = ((3.f / 8.f) * m_fRadius + (fHeight / 2.f)) *
        ((3.f / 8.f) * m_fRadius + (fHeight / 2.f));
      x -= ((3.f / 8.f) * m_fRadius) * ((3.f / 8.f) * m_fRadius);
      float z = x;
      float y = (2.f / 5.f) * m_fRadius * m_fRadius;

      float4x4 hemisphereTensor(
        x, 0.f, 0.f, 0.f,
        0.f, y, 0.f, 0.f,
        0.f, 0.f, z, 0.f,
        0.f, 0.f, 0.f, 0.f);

      x = (1.f / 12.f) * ((3.f * m_fRadius * m_fRadius) + (fHeight * fHeight));
      z = x;
      y = (m_fRadius * m_fRadius) / 2.f;

      float4x4 cylinderTensor(
        x, 0.f, 0.f, 0.f,
        0.f, y, 0.f, 0.f,
        0.f, 0.f, z, 0.f,
        0.f, 0.f, 0.f, 0.f);

      float4x4 result = cylinderTensor + (hemisphereTensor * 2);
      // TODO: technically should check these for zero first
      result.m00 = 1.f / result.m00;
      result.m11 = 1.f / result.m11;
      result.m22 = 1.f / result.m22;

      return result;
    }
  };

  struct Plane
  {
    // (n.x, n.y, n.z, d)
    // n is the normal of the plane, d is the distance from the origin
    float4 m_Data;

    // default constructor is a plane pointed up in the y direction at at the origin
    Plane(void) : m_Data(0.f, 1.f, 0.f, 0.f) {}
    Plane(float4 const &Normal, float fDistance) 
      : m_Data(Normal.x, Normal.y, Normal.z, fDistance) {}
    Plane(float4 const &P0, float4 const &P1, float4 const &P2)
      : m_Data()
    {
      SetNormal((P1 - P0).Cross(P2 - P0).Normal());
      SetDistance(GetNormal().Dot(P0));
    }

    float4 GetNormal(void) const { return float4(m_Data.x, m_Data.y, m_Data.z, 0.f); }
    float GetDistance(void) const { return m_Data.w; }

    void SetNormal(float4 const &Normal) { m_Data.x = Normal.x; m_Data.y = Normal.y; m_Data.z = Normal.z; }
    void SetDistance(float fDistance) { m_Data.w = fDistance; }

    float4 GetPlanePoint(void) const { return GetNormal() * GetDistance(); }
    float DistanceToPoint(float4 const &Point) const { return GetNormal().Dot(Point) - GetDistance(); }
    // project Point onto the plane and return that new point
    float4 ProjectPoint(float4 const &Point) { Point - (GetNormal() * DistanceToPoint(Point)); }

    // given an n sided polygon, compute a best fit plane from it
    void ComputeNewell(std::vector<float4> const &points);

    bool Coplanar(Plane const &plane) const
    {
      // a plane is coplanar if the corresponding plane p's normal
      // is with a certain tolerance to 0 when taking the cross product
      // of their normals
      float4 u = GetNormal().Cross(plane.GetNormal());
      // TODO: Find out why these numbers?
      float tolerance = 0.0005f * 1.41f;

      if (u.LengthSq() < tolerance * tolerance)
        return true;
      return false;
    }

    bool PointOnPlane(float4 const &Point) const { return Equals(DistanceToPoint(Point), 0.f); }
  };

  // assumes a counter clockwise order
  struct Triangle
  {
    float4 m_Point0;
    float4 m_Point1;
    float4 m_Point2;

    Triangle(void) : m_Point0(-0.5f, 0.f, -0.5f, 0.f), 
      m_Point1(0.5f, 0.f, -0.5f, 0.f), m_Point2(0.f, 0.f, 0.5f, 0.f) {}
    Triangle(float4 const &Point0, float4 const &Point1, float4 const &Point2)
      : m_Point0(Point0), m_Point1(Point1), m_Point2(Point2) {}

    // uses barycentric coordinates to check point and triangle collision
    bool PointOnTriangle(float4 const &/*Point*/) const { return false; }
  };

  struct Cylinder
  {
    // assumes in local space
    // the two points that make the center axis of the capsule
    float4 m_TopPoint;
    float4 m_BottomPoint;
    float m_fRadius;

    Cylinder(void) 
      : m_TopPoint(0.f, 0.5f, 0.f, 0.f)
      , m_BottomPoint(0.f, -0.5f, 0.f, 0.f)
      , m_fRadius(0.5f)
    {
    }
    Cylinder(float4 const &TopPoint, float4 const &Point1, float fRadius)
      : m_TopPoint(TopPoint)
      , m_BottomPoint(Point1)
      , m_fRadius(fRadius)
    {
    }

    float GetHeight(void) const
    {
      return (m_BottomPoint - m_TopPoint).Length();
    }

    void SetHeight(float fHeight)
    {
      float4 Normal = (m_TopPoint - m_BottomPoint);
      Normal.Normalize();

      m_TopPoint = Normal * (fHeight / 2.f);
      m_BottomPoint = -Normal * (fHeight / 2.f);
    }

    float GetVolume(void) const
    {
      return PI * m_fRadius * m_fRadius * GetHeight();
    }

    float4x4 GetInvereInertiaTensor(void) const
    {
      float fHeight = GetHeight();

      float x = (1.f / 12.f) * ((3.f * m_fRadius * m_fRadius) + (fHeight * fHeight));
      float z = x;
      float y = (m_fRadius * m_fRadius) / 2.f;

      float4x4 cylinderTensor(
        x, 0.f, 0.f, 0.f,
        0.f, y, 0.f, 0.f,
        0.f, 0.f, z, 0.f,
        0.f, 0.f, 0.f, 0.f);

      // TODO: technically should check these for zero first
      cylinderTensor.m00 = 1.f / cylinderTensor.m00;
      cylinderTensor.m11 = 1.f / cylinderTensor.m11;
      cylinderTensor.m22 = 1.f / cylinderTensor.m22;

      return cylinderTensor;
    }
  };

  struct Ray
  {
    float4 m_Start;
    float4 m_Direction;
    float m_IntersectionTime;

    Ray(void) : m_Start(), m_Direction(1.f, 0.f, 0.f, 0.f), m_IntersectionTime(0.f) {}
    Ray(float4 const &Start, float4 const &Direction) : m_Start(Start),
      m_Direction(Direction) { m_Direction.Normalize(); }

    // returns the intersection point, p = s + t * d in world space
    float4 GetIntersectionPoint(void) { return m_Start + m_Direction * m_IntersectionTime; }
    void RotateIntoBasis(float4 const &Origin, float4x4 const &Rotation);
    void RotateOutOfBasis(float4 const &Origin, float4x4 const &Rotation);
  };

  struct Line
  {
    float4 m_P0;
    float4 m_P1;

    Line(void) : m_P0(), m_P1() {}
    Line(float4 const &Start, float4 const &End) : m_P0(Start), m_P1(End) {}

    float4 GetAxis(void) { return m_P1 - m_P0; }

    float4 GetClosestPoint(float4 const &p) const
    {
      float4 ab = m_P1 - m_P0;
      float t = (p - m_P0).Dot(ab) / ab.Dot(ab);

      t = Clamp(t, 0.f, 1.f);
      return m_P0 + ab * t;
    }

    Line GetClosestPoints(Line const &rhs) const
    {
      // direction of this line
      float4 d0 = m_P1 - m_P0;
      // direction of rhs line
      float4 d1 = rhs.m_P1 - rhs.m_P0;
      float4 r = m_P0 - rhs.m_P0;
      float s = 0.f, t = 0.f, a = d0.Dot(d0), e = d1.Dot(d1), f = d1.Dot(r);

      // check if either or both segments degenerate into a point
      if (a <= EPSILON && e <= EPSILON)
        return Line(m_P0, rhs.m_P0);

      if (a <= EPSILON)
      {
        s = 0.f;
        t = f / e;
        t = Clamp(t, 0.f, 1.f);
      }
      else
      {
        float c = d0.Dot(r);
        if (e <= EPSILON)
        {
          t = 0.f;
          s = Clamp(-c / a, 0.f, 1.f);
        }
        else
        {
          float b = d0.Dot(d1);
          float denom = a * e - b * b;

          if (!Equals(denom, 0.f))
            s = Clamp((b * f - c * e) / denom, 0.f, 1.f);
          else
            s = 0.f;

          t = (b * s + f) / e;

          if (t < 0.f)
          {
            t = 0.f;
            s = Clamp(-c / a, 0.f, 1.f);
          }
          else if (t > 1.f)
          {
            t = 1.f;
            s = Clamp((b - c) / a, 0.f, 1.f);
          }
        }
      }

      float4 c0 = m_P0 + d0 * s;
      float4 c1 = rhs.m_P0 + d1 * t;
      return Line(c0, c1);
    }

    // grabs the length of this line (p1 - p0).length()
    float GetLength(void) const { (m_P1 - m_P0).Length(); }
    float GetLengthSq(void) const {(m_P1 - m_P0).LengthSq(); }
  };

  // assumes the shape is uniform and the ray is in the same coordinate space as the other shape
  bool RayIntersectionTest(Ray &ray, Box const &box);
  bool RayIntersectionTest(Ray &ray, Sphere const &sphere);
  bool RayIntersectionTest(Ray &ray, Capsule const &capsule);
  bool RayIntersectionTest(Ray &ray, Plane const &plane);
  bool RayIntersectionTest(Ray &ray, Triangle const &triangle);
  bool RayIntersectionTest(Ray &ray, Cylinder const &cylinder);
}

#endif