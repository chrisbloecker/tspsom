/**
 * @file
 *
 * @author Christopher Blöcker
 */
#ifndef __VECTOR_H__
#define __VECTOR_H__

/* -------------------------------------------------------------------------- */

typedef struct {
  double x
       , y
       ;
} Vector;

/* -------------------------------------------------------------------------- */
#define vectorLength(u) (sqrt((u.x)*(u.x)+(u.y)*(u.y)))
/* -------------------------------------------------------------------------- */

/**
 * Creates a 2D vector.
 *
 * @param[in] x  x-component.
 * @param[in] y  y-component.
 *
 * @return The vector with the given components.
 */
extern Vector vectorMake(double x, double y);

/**
 * Adds two vectors.
 *
 * @param[in] u  First vector.
 * @param[in] v  Second vector.
 *
 * @return u+v.
 */
extern Vector vectorAdd(Vector u, Vector v);

/**
 * Subtracts one vector from another.
 *
 * @param[in] u  First vector.
 * @param[in] v  Second vector.
 *
 * @return u-v.
 */
extern Vector vectorSub(Vector u, Vector v);

/**
 * Scales a vector by a scalar.
 *
 * @param[in] u  Vector that should be scaled.
 * @param[in] s  Scaling factor.
 *
 * @return v*s.
 */
extern Vector vectorScale(Vector u, double s);

/**
 * Rotates a vector by the given angle.
 *
 * @param[in] u      Vector that should be rotated.
 * @param[in] angle  Rotation angle.
 *
 * @return Vector rotated by the given angle.
 */
extern Vector vectorRotate(Vector u, double angle);

#endif
