/*
** Copyright (C) 1998-2005 George Tzanetakis <gtzan@cs.uvic.ca>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/


/**
   \class Color:
   \brief Triplet of RGB values 
   
   Simply a triplet of RGB values representing a color
plus various utility functions.
*/ 

#if !defined(__Color_h)
#define __Color_h

namespace Marsyas
{

class Color
{
protected:
  float red_ ;
  float green_ ;
  float blue_;
public:
  Color(float r, float g, float b);
  Color();
  ~Color();
  void setRGB(float r, float g, float b);
  void setR();
  void setG();
  void setB();
  float getR();
  float getG();
  float getB();
};

}//namespace Marsyas

#endif
