/**
 * @file
 *
 * @author Christopher Blöcker
 */

/* -------------------------------------------------------------------------- */
#include <math.h>
/* -------------------------------------------------------------------------- */
#include "vector.h"
/* -------------------------------------------------------------------------- */
#define DEGTORAD(x) ((x)*(M_PI)/(180))
#define RADTODEG(x) ((180)*(x)/(M_PI))
/* -------------------------------------------------------------------------- */

/**
 * Creates a 2D vector.
 *
 * @param[in] x  x-component.
 * @param[in] y  y-component.
 *
 * @return The vector with the given components.
 */
extern Vector vectorMake(double x, double y)
{
  Vector res;

  res.x = x;
  res.y = y;

  return res;
}

/**
 * Adds two vectors.
 *
 * @param[in] u  First vector.
 * @param[in] v  Second vector.
 *
 * @return u+v.
 */
extern Vector vectorAdd(Vector u, Vector v)
{
  return vectorMake(u.x + v.x, u.y + v.y);
}

/**
 * Subtracts one vector from another.
 *
 * @param[in] u  First vector.
 * @param[in] v  Second vector.
 *
 * @return u-v.
 */
extern Vector vectorSub(Vector u, Vector v)
{
  return vectorMake(u.x - v.x, u.y - v.y);
}

/**
 * Scales a vector by a scalar.
 *
 * @param[in] u  Vector that should be scaled.
 * @param[in] s  Scaling factor.
 *
 * @return v*s.
 */
extern Vector vectorScale(Vector u, double s)
{
  return vectorMake(u.x * s, u.y * s);
}

/**
 * Rotates a vector by the given angle.
 *
 * @param[in] u      Vector that should be rotated.
 * @param[in] angle  Rotation angle.
 *
 * @return Vector rotated by the given angle.
 */
extern Vector vectorRotate(Vector u, double angle)
{
  double a = DEGTORAD(angle);

  return vectorMake( u.x * cos(a) - u.y * sin(a)
                   , u.x * sin(a) + u.y * cos(a)
                   );
}
