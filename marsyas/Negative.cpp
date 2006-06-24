/*
** Copyright (C) 1998-2006 George Tzanetakis <gtzan@cs.uvic.ca>
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
    \class Negative
    \brief Negate the input
*/

#include "Negative.h"

using namespace std;
using namespace Marsyas;

Negative::Negative(string name)
{
  type_ = "Negative";
  name_ = name;
  addControls();
}


Negative::~Negative()
{
}

void
Negative::addControls()
{
  addDefaultControls();
}

MarSystem* 
Negative::clone() const 
{
  return new Negative(*this);
}



void
Negative::update()
{
  setctrl("mrs_natural/onSamples", getctrl("mrs_natural/inSamples"));
  setctrl("mrs_natural/onObservations", getctrl("mrs_natural/inObservations"));
  setctrl("mrs_real/osrate", getctrl("mrs_real/israte"));
  defaultUpdate();
}

void 
Negative::process(realvec& in, realvec& out)
{
  for (t = 0; t < inSamples_; t++)
    out(t) = - in(t);
}






	

	
