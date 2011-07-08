/* sunpos.c
**
**    finding the sun position
**    Copyright (C) 2001  Florian Berger
**    Email: harpin_floh@yahoo.de, florian.berger@jk.uni-linz.ac.at
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

#include <stdio.h>
#include <stdlib.h>
#include "sunpos.h"

double phi_mod( double phi, double e )
/* g = rmax/rmin */
{
    double rval;
//    e=(g-1.0)/(g+1.0);
//    rval=phi+2.0*e/(1.0+e*e)*sin(phi);
    rval=phi+1.7*e*sin(phi);    /* 1.7 adjusted to xearth/xglobe */
    return rval;
}

void sunpos(time_t t, VMvect * pos, int ellipse_on)
{
    int snwd, perihelion;
    double snwd_long;
    double tilt;
    double rev_per_year;
    double ang_per_sec_earth_rel;
    double ang_per_sec_earth, ang_per_sec_sun, ang_earth, ang_sun;
    double ang_sun_snwd;  /* angle from perihelion at solistice (snwd) */
    double ecc;
    VMvect bx, by, bz, sunvec;

    tilt=23.4431; /* degrees */
//    tilt=23.5; /* degrees */
    snwd=898434180;  /* 16.03h (=15.03 CET = 14.03 GT) summers-time 21.juni 1998 */
    perihelion=978591600; /* 9:00 UT, 5:00 Eastern Std Time */
    ecc = 0.017;

    snwd_long = 2.05*15.0;  /* 2h03min in degrees */
    rev_per_year=365.249+1.0;
    t-=snwd;
//    t=898434180+60*57+24*3600*10-snwd;

//    fprintf(stderr,"solstice=%s\n",ctime(&snwd));
//    t*=100;
//    t*=3600*24*10;
//    t-=3600.0*8.25+3600.0*24.0*60;

//    ang_per_sec_earth = 2.0*M_PI*rev_per_year/365.249/24.0/3600.0;
    ang_per_sec_sun   = 2.0*M_PI/365.249/24.0/3600.0;
    ang_per_sec_earth = ang_per_sec_sun+2.0*M_PI/24.0/3600.0;
    ang_per_sec_earth_rel = 2.0*M_PI/24.0/3600.0; /* relative to sun */

    ang_sun_snwd = -(double)(perihelion-snwd)*ang_per_sec_sun;
    ang_sun   = ang_per_sec_sun   * t;
    ang_earth = ang_per_sec_earth * t + snwd_long*M_PI/180.0;
//    ang_earth = ang_per_sec_sun * t + ang_per_sec_earth_rel * t + snwd_long*M_PI/180.0;

    //the x-basis(axis) points to greenich
    //the z-basis(axis) points north
    bz.x = -sin(tilt*M_PI/180.0);
    bz.y = 0.0;
    bz.z = cos(tilt*M_PI/180.0) ;

    by.x =  0.0;
    by.y = -1.0;
    by.z =  0.0;

    bx=vec_cross(by,bz);

    // rotate bx,by around bz
    rot_ax(bz,&bx,1,ang_earth);
    rot_ax(bz,&by,1,ang_earth);

    // rotate sunvec around z
    if( ellipse_on ){
        sunvec.x = -cos( phi_mod(ang_sun+ang_sun_snwd,ecc)
                         -phi_mod(ang_sun_snwd,ecc) );
        sunvec.y = -sin( phi_mod(ang_sun+ang_sun_snwd,ecc)
                         -phi_mod(ang_sun_snwd,ecc) );
    } else {
        sunvec.x = -cos( ang_sun );
        sunvec.y = -sin( ang_sun );
    }
    sunvec.z = 0.0;

    pos->x = vec_mul(sunvec,bx);
    pos->y = vec_mul(sunvec,by);
    pos->z = vec_mul(sunvec,bz);

}


