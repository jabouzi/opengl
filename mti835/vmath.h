/* vmath.h
**
**    includefile: some vector mathematics and structures (float or double)
**    Copyright (C) 2001  Florian Berger
**    Email:  harpin_floh@yahoo.de,  florian.berger@jk.uni-linz.ac.at
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License Version 2 as
**    published by the Free Software Foundation;
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program; if not, write to the Free Software
**    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
*/
#ifndef VMATH_H
#define VMATH_H

#define VMATH_SINGLE_PRECISION 0  /* this should be in the using code */

#ifdef VMATH_SINGLE_PRECISION
typedef float VMfloat;
#else
typedef double VMfloat;
#endif

struct Vect{
          VMfloat x,y,z;
       };
typedef struct Vect VMvect;


struct Shape{
          int pnr;
          int closed;
          struct Vect *p;
       };
typedef struct Shape VMshape;


struct Triangle{
          struct Vect v1,v2,v3;
       };
typedef struct Triangle VMtriangle;


struct Mesh{
          int tnr;
          struct Triangle *t;
          struct Triangle *n;
       };
typedef struct Mesh VMmesh;


struct Point{
    struct Vect pos;
    struct Point * next;
};
typedef struct Point VMpoint;


struct PolyPoint{
    int pnr;
    struct PolyPoint * next;
};
typedef struct PolyPoint VMpolypoint;



void         shape_copy( struct Shape shs, struct Shape *shd );
void         shape_flip( struct Shape *sh );
void         shape_trans( struct Shape *s, struct Vect t );
struct Vect  vec_cross( struct Vect v1, struct Vect v2 );
VMfloat      vec_mul( struct Vect v1, struct Vect v2 );
struct Vect  vec_diff( struct Vect v1, struct Vect v2 );
struct Vect  vec_add( struct Vect v1, struct Vect v2 );
struct Vect  vec_scale( struct Vect v1, VMfloat scale );
VMfloat      vec_abs( struct Vect v );
struct Vect  vec_unit( struct Vect v );
int          vec_equal( struct Vect v1, struct Vect v2 );
int          vec_nearly_equal( struct Vect v1, struct Vect v2, VMfloat tolerance );
struct Vect  vec_xyz( VMfloat x, VMfloat y, VMfloat z );
struct Vect  vec_ex();
struct Vect  vec_ey();
struct Vect  vec_ez();
struct Vect  vec_null();
void         rot_ax( struct Vect ax, struct Vect *v, int nr, VMfloat phi );
void         shape_rot_ax( struct Vect ax, struct Shape *s, struct Vect m, VMfloat phi,
                           struct Vect nax, VMfloat sc_x, VMfloat sc_y, VMfloat sc_z );

struct Vect  meshpt_nr( struct Point * plist, int index );

int remove_idle_points( struct Shape * sh, struct Shape * shd );

VMfloat      vec_angle( struct Vect v1, struct Vect v2 );

struct Vect  vec_proj( struct Vect v1, struct Vect v2 );

VMfloat      shape_area( struct Shape sh );

#endif  //VMATH_H
