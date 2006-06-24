
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
    \class ShiftOuput
    \brief Shift next D samples  

    Shift next D samples from the input source to the output source 
into the right-hand end of the outputSlice. 
with gain and put them in the output vector. 
*/

#include "ShiftOutput.h"

using namespace std;
using namespace Marsyas;

ShiftOutput::ShiftOutput():MarSystem()
{
  type_ = "ShiftOutput";
}


ShiftOutput::ShiftOutput(string name)
{
  type_ = "ShiftOutput";
  name_ = name;
  addControls();
  
}


ShiftOutput::~ShiftOutput()
{
}

MarSystem* 
ShiftOutput::clone() const 
{
  return new ShiftOutput(*this);
}



void
ShiftOutput::addControls()
{
  addDefaultControls();
  addctrl("mrs_natural/Interpolation", (mrs_natural)MRS_DEFAULT_SLICE_NSAMPLES / 2);
  setctrlState("mrs_natural/Interpolation", true);
  addctrl("mrs_natural/WindowSize", (mrs_natural)MRS_DEFAULT_SLICE_NSAMPLES);
  addctrl("mrs_natural/Decimation", (mrs_natural)MRS_DEFAULT_SLICE_NSAMPLES/2);
}



void
ShiftOutput::update()
{
  setctrl("mrs_natural/onSamples", getctrl("mrs_natural/Interpolation"));
  setctrl("mrs_natural/onObservations", (mrs_natural)1);
  setctrl("mrs_real/osrate", getctrl("mrs_real/israte"));  
  
  setctrl("mrs_natural/WindowSize",getctrl("mrs_natural/inSamples"));


  defaultUpdate();
  
  
  tmpSlice_.stretch(inObservations_, inSamples_);
  
  I_ = getctrl("mrs_natural/onSamples").toNatural();
  N_ = getctrl("mrs_natural/inSamples").toNatural();
  Nw_ = getctrl("mrs_natural/WindowSize").toNatural();
  D_ = getctrl("mrs_natural/Decimation").toNatural();


}




void 
ShiftOutput::process(realvec& in, realvec& out)
{
  checkFlow(in,out);
  
  
  n_ += 2*D_ + Nw_ / 2;
  
  
  for (o=0; o < inObservations_; o++)
    for (t =0; t < inSamples_; t++)
      {
	tmpSlice_(o,t) = in(o,t);
      }

  if (n_ >= 0.0)
    for (t = 0; t < I_; t++)
      {
	out(t) = in(t);
      }
  
}






	

	
  
