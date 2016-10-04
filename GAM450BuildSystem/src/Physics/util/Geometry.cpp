/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Geometry.cpp 
 * Author: Jonah Dube
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"

#include <limits>

#include "util/Geometry.h"

namespace
{
  using namespace Physics;
  bool RayToSphereHelper(Ray &ray, Sphere const &sphere, float4 const &Position)
  {
    // algorithm from Real-Time Collision Detection by Christer Ericson
    float4 m = ray.m_Start - Position;
    float b = m.Dot(ray.m_Direction);
    float c = m.Dot(m) - sphere.m_fRadius * sphere.m_fRadius;

    // exit if r's origin out s (c > 0) and r pointing away from s (b > 0)
    if (c > 0.f && b > 0.f)
      return false;

    float disc = b * b - c;
    // a negative discrimiante means the ray didn't hit the sphere
    if (disc < 0.f)
      return false;

    // ray intersected with the sphere, compute the smallest value of t
    float t = -b - std::sqrt(disc);
    // if t is negative, the ray started in the xphere, clamp it to zero
    if (t < 0.f)
      t = 0.f;

    ray.m_IntersectionTime = t;

    return true;
  }
}

namespace Physics
{
  float4 RotateVectorIntoBasis(float4x4 const &Orientation, float4 const &v)
  {
    return Orientation.Transpose() * v;
  }

  float4 RotatePointIntoBasis(float4x4 const &Orientation, float4 const &Origin, float4 const &p)
  {
    return Orientation.Transpose() * (p - Origin);
  }

  float4 RotateVectorOutOfBasis(float4x4 const &Orientation, float4 const &v)
  {
    return Orientation * v;
  }

  float4 RotatePointOutOfBasis(float4x4 const &Orientation, float4 const &Origin, float4 const &p)
  {
    return (Orientation * p) + Origin;
  }

  void Plane::ComputeNewell(std::vector<float4> const &points)
  {
    int n = static_cast<int>(points.size());
    float4 center, normal;

    for (int i = n - 1, j = 0; j < n; i = j, j++)
    {
      normal.x += (points[i].y - points[j].y) * (points[i].z + points[j].z);
      normal.y += (points[i].z - points[j].z) * (points[i].x + points[j].x);
      normal.z += (points[i].x - points[j].x) * (points[i].y + points[j].y);
      center += points[j];
    }

    // normalize and fill in the plane equation
    normal.Normalize();
    SetNormal(normal);
    SetDistance(center.Dot(normal) / static_cast<float>(n)); // centroid / n is the true centroid point
  }

  void Ray::RotateIntoBasis(float4 const &Origin, float4x4 const &Rotation)
  {
    m_Start = RotatePointIntoBasis(Rotation, Origin, m_Start);
    m_Direction = RotateVectorOutOfBasis(Rotation, m_Direction);
  }

  void Ray::RotateOutOfBasis(float4 const &Origin, float4x4 const &Rotation)
  {
    m_Start = RotatePointOutOfBasis(Rotation, Origin, m_Start);
    m_Direction = RotateVectorOutOfBasis(Rotation, m_Direction);
  }

  bool RayIntersectionTest(Ray &ray, Box const &box)
  {
    // algorithm from Real-Time Collision Detection by Christer Ericson
    // box is at the origin, ray is in the boxes space
    float tMin = 0.f;
    float tMax = std::numeric_limits<float>::max();

    for (unsigned i = 0; i < 3; ++i)
    {
      // the ray is parallel to the axis of the box
      if (std::abs(ray.m_Direction[i]) < EPSILON)
      {
        // if the origin is not within the "slab" on this axis, there can't be an intersection
        if (ray.m_Start[i] < -box.m_HalfExtents[i] || ray.m_Start[i] > box.m_HalfExtents[i])
          return false;
      }
      else
      {
        // computer the intersection value of the ray to the min and max extents
        float ood = Equals(ray.m_Direction[i], 0.f) ? 0.f : 1.f / ray.m_Direction[i];
        float t1 = (-box.m_HalfExtents[i] - ray.m_Start[i]) * ood;
        float t2 = (box.m_HalfExtents[i] - ray.m_Start[i]) * ood;

        // make t1 be the intersection with the min and t2 with the max
        if (t1 > t2)
          std::swap(t1, t2);

        // compute the slab intersection intervals
        tMin = std::max(tMin, t1);
        tMax = std::min(tMax, t2);

        if (tMin > tMax)
          return false;
      }
    }

    // there was intersection
    ray.m_IntersectionTime = tMin;
    return true;
  }

  bool RayIntersectionTest(Ray &ray, Sphere const &sphere)
  {
    // the sphere is at the origin
    return RayToSphereHelper(ray, sphere, float4());
  }

  bool RayIntersectionTest(Ray &ray, Capsule const &capsule)
  {
    // if we collide with the cylinder we can consider it done
    Cylinder cylinder(capsule.m_TopPoint, capsule.m_BottomPoint, capsule.m_fRadius);
    bool bResult = RayIntersectionTest(ray, cylinder);
    if (bResult)
      return true;

    // to an intersection test against the two spheres
    Sphere sphere(capsule.m_fRadius);

    bool bSphere0 = RayToSphereHelper(ray, sphere, capsule.m_TopPoint);
    float tSphere0 = 0.f, tSphere1 = 0.f;

    tSphere0 = ray.m_IntersectionTime;

    bool bSphere1 = RayToSphereHelper(ray, sphere, capsule.m_BottomPoint);
    tSphere1 = ray.m_IntersectionTime;

    // the ray hit both spheres, take the minimum
    if (bSphere0 && bSphere1)
      ray.m_IntersectionTime = std::min(tSphere0, tSphere1);
    // we hit only the first sphere
    else if (bSphere0)
      ray.m_IntersectionTime = tSphere0;
    else if (bSphere1)
      ray.m_IntersectionTime = tSphere1;
    // we didn't hit either sphere, reset the intersection time and return false
    else
    {
      ray.m_IntersectionTime = 0.f;
      return false;
    }
    
    return true;
  }

  bool RayIntersectionTest(Ray &ray, Plane const &plane)
  {
    float4 Normal = plane.GetNormal();
    float4 PlaneP0 = plane.GetPlanePoint();
    float4 Diff = ray.m_Start - PlaneP0;

    float denom = Normal.Dot(ray.m_Direction);
    if (Equals(denom, 0.f))
      return false;

    // takes the plane equation, n * (p - p0) = 0
    // and the ray equation, p(t) = start + direction * t
    // substitute p from the ray equation into the plane equation and solve for t
    float t = (-Normal.Dot(Diff)) / denom;
    if (t < 0.f)
      return false;

    ray.m_IntersectionTime = t;

    return true;
  }

  bool RayIntersectionTest(Ray &ray, Triangle const &triangle)
  {
    Plane plane(triangle.m_Point0, triangle.m_Point1, triangle.m_Point2);

    bool bResult = RayIntersectionTest(ray, plane);
    if (!bResult)
      return false;

    // it intersected the plane now use barycentric coordinates to determine
    // if the intersected point is in the triangle
    bResult = triangle.PointOnTriangle(ray.GetIntersectionPoint());
    // it didn't actually hit the triangle, intersection time should get reset
    if (!bResult)
      ray.m_IntersectionTime = 0.f;

    return bResult;
  }

  // algorithm from Real-Time Collision Detection by Christer Ericson
  bool RayIntersectionTest(Ray &ray, Cylinder const &cylinder)
  {
    // assume no intersection
    ray.m_IntersectionTime = 0.f;

    float4 d = cylinder.m_TopPoint - cylinder.m_BottomPoint;
    float4 m = ray.m_Start - cylinder.m_BottomPoint;
    float4 n = ray.m_Direction;

    float md = m.Dot(d);
    float nd = n.Dot(d);
    float dd = d.Dot(d);

    // test if the ray is completely outside of the cylinder's end caps
    // ray is outside the p cap
    if (md < 0.f && md + nd < 0.f)
      return false;
    // ray is outside the q cap
    if (md > dd && md + nd > dd)
      return false;

    float nn = n.Dot(n);
    float mn = m.Dot(n);
    float a = dd * nn - nd * nd;
    float k = m.Dot(m) - cylinder.m_fRadius * cylinder.m_fRadius;
    float c = dd * k - md * md;

    // the ray is parallel to the cylinder axis
    if (abs(a) < EPSILON)
    {
      if (c > 0.f)
        return false;

      // intersected with the p cap
      if (md < 0.f)
        ray.m_IntersectionTime = -mn / nn;
      // intersected with the q cap
      else if (md > dd)
        ray.m_IntersectionTime = (nd - mn) / nn;
      // the ray is inside the cylinder
      else
        ray.m_IntersectionTime = 0.f;
      return true;
    }

    float b = dd * mn - nd * md;
    float disc = b * b - a * c;
    if (disc < 0.f)
      return false;

    float t = (-b - std::sqrt(disc)) / a;
    // no intersection the time value is before the start point of the ray
    if (t < 0.f)
      return false;

    // intersected outside cylinder on the p cap
    if (md + t * nd < 0.f)
    {
      // segment is pointing away from the end cap
      if (nd <= 0.f)
        return false;
      t = -md / nd;

      bool bResult = k + 2 * t * (mn + t * nn) <= 0.f;
      if (bResult)
        ray.m_IntersectionTime = t;
      return bResult;
    }
    // intersected outside cylinder on the q cap
    else if (md + t * nd > dd)
    {
      // segment is pointing away from the endcap
      if (nd >= 0.f)
        return false;
      t = (dd - md) / nd;

      bool bResult = k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.f;
      if (bResult)
        ray.m_IntersectionTime = t;
      return bResult;
    }

    // ray intersects the cylinder between the endcaps, t is correct
    ray.m_IntersectionTime = t;
    return true;
  }
}
