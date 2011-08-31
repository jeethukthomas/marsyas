/*
** Copyright (C) 1998-2010 George Tzanetakis <gtzan@cs.uvic.ca>
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

#ifndef MARSYAS_HARMONICSTRENGTH_H
#define MARSYAS_HARMONICSTRENGTH_H

#include "MarSystem.h"

namespace Marsyas
{
/**
	\class HarmonicStrength
	\ingroup Processing
	\brief Calculates the relative strength of harmonics

	Calculates the strength of multiples of the base_frequency
compared to the overall spectrum rms.

	Controls:
	- \b mrs_real/base_frequency [w] : sets the fundamental
	  frequency.
	- \b mrs_realvec/harmonics [w] : which harmonics to examine.
	  Need not be integer multiples (e.g. 0.5, 1.5, is ok)
	- \b mrs_realvec/harmonicsSize [w] : how many harmonics
          (long story, it's a workaround for some weird memory thing)
	- \b mrs_real/harmonicsWidth [w] : percent of frequency to
      search for a peak (i.e. a 1000hz harmonic with a
      harmonicsWidth of 0.1 would look at all bins from 900 to
      1100 for the peak.
*/

class marsyas_EXPORT HarmonicStrength: public MarSystem
{
private:

    /// Add specific controls needed by this MarSystem.
    void addControls();

    /// Reads changed controls and sets up variables if necessary.
    void myUpdate(MarControlPtr sender);


    MarControlPtr ctrl_base_frequency_;
    MarControlPtr ctrl_harmonics_;
    MarControlPtr ctrl_harmonicsSize_;
    MarControlPtr ctrl_harmonicsWidth_;

    mrs_real find_peak_magnitude(mrs_real central_bin,
                                 mrs_realvec& in, mrs_natural t, mrs_real radius);
    mrs_real quadratic_interpolation(mrs_real best_bin,
                                     mrs_realvec& in, mrs_natural t);

public:
    /// HarmonicStrength constructor.
    HarmonicStrength(std::string name);

    /// HarmonicStrength copy constructor.
    HarmonicStrength(const HarmonicStrength& a);

    /// HarmonicStrength destructor.
    ~HarmonicStrength();

    /// Implementation of the MarSystem::clone() method.
    MarSystem* clone() const;

    /// Implementation of the MarSystem::myProcess method.
    void myProcess(realvec& in, realvec& out);
};

}
//namespace Marsyas

#endif
//MARSYAS_HARMONICSTRENGTH_H

