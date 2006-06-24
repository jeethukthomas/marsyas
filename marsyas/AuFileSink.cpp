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
   \class AuFileSink
   \brief SoundFileSink for .au (.snd) soundfiles
   
   SoundFileSink writer for .au (.snd) sound files.
   (Next, Sun audio format). 
*/

#include "AuFileSink.h"

using namespace std;
using namespace Marsyas;

#define SND_MAGIC_NUM 0x2e736e64

/********  NeXT/Sun Soundfile Header Struct   *******/

/* struct snd_header 
{
char pref[4];
long hdrLength;
long fileLength;
long mode;
long srate;
long channels;
char comment[1024];
};
*/ 

/* Array containing descriptions of
the various formats for the samples
of the Next .snd/ Sun .au format */

/* types of .snd files */  
#define SND_FORMAT_UNSPECIFIED 0
#define SND_FORMAT_MULAW_8     1
#define SND_FORMAT_LINEAR_8    2
#define SND_FORMAT_LINEAR_16   3
#define SND_FORMAT_LINEAR_24   4
#define SND_FORMAT_LINEAR_32   5
#define SND_FORMAT_FLOAT       6

AuFileSink::AuFileSink(string name)
{
  type_ = "AuFileSink";
  name_ = name;
  cdata_ = NULL;
  sdata_ = NULL;
  addControls();
}

AuFileSink::~AuFileSink()
{
}

MarSystem* 
AuFileSink::clone() const
{
  return new AuFileSink(*this);
}


void 
AuFileSink::addControls()
{
  addDefaultControls();
  addctrl("mrs_natural/nChannels", (mrs_natural)1);
  setctrlState("mrs_natural/nChannels", true);
  addctrl("mrs_string/filename", "daufile");
  setctrlState("mrs_string/filename", true);
}


bool 
AuFileSink::checkExtension(string filename)
{
  FileName fn(filename);
  string auext  = "au";
  string sndext = "snd";
  
  if ((fn.ext() == auext) || (fn.ext() == sndext))
    return true;
  else 
    return false;
}

void 
AuFileSink::update()
{
  MRSDIAG("AudioFileSink::update");
  setctrl("mrs_natural/onSamples", getctrl("mrs_natural/inSamples"));
  setctrl("mrs_natural/onObservations", getctrl("mrs_natural/inObservations"));
  setctrl("mrs_real/osrate", getctrl("mrs_real/israte"));

  nChannels_ = getctrl("mrs_natural/inObservations").toNatural();      
  setctrl("mrs_natural/nChannels", nChannels_);
  
  
  delete sdata_;
  delete cdata_;
  
  sdata_ = new short[getctrl("mrs_natural/inSamples").toNatural() * nChannels_];
  cdata_ = new unsigned char[getctrl("mrs_natural/inSamples").toNatural() * nChannels_];

  filename_ = getctrl("mrs_string/filename").toString();
  
  defaultUpdate();
}

  
void 
AuFileSink::putHeader(string filename)
{
  mrs_natural nChannels = (mrs_natural)getctrl("mrs_natural/nChannels").toNatural();
  
  written_ = 0;
  char *comment = "MARSYAS 2001, George Tzanetakis.\n";
  int commentSize = strlen(comment);
  sfp_ = fopen(filename.c_str(), "wb");
  hdr_.pref[0] = '.';
  hdr_.pref[1] = 's';
  hdr_.pref[2] = 'n';
  hdr_.pref[3] = 'd';


#if defined(__BIG_ENDIAN__)
	  hdr_.hdrLength = 24 + commentSize;
	  hdr_.fileLength = 0;
	  hdr_.mode = SND_FORMAT_LINEAR_16;                           
	  hdr_.srate = (mrs_natural)getctrl("mrs_real/israte").toReal();
	  hdr_.channels = nChannels;

#else
	  hdr_.hdrLength = ByteSwapLong(24 + commentSize);
	  hdr_.fileLength = ByteSwapLong(0);
	  hdr_.mode = ByteSwapLong(SND_FORMAT_LINEAR_16);                           
	  hdr_.srate = ByteSwapLong((mrs_natural)getctrl("mrs_real/israte").toReal());
	  hdr_.channels = ByteSwapLong(nChannels);

#endif 

  fwrite(&hdr_, 24, 1, sfp_);
  // Write comment part of header 
  fwrite(comment, commentSize, 1, sfp_);      
  sfp_begin_ = ftell(sfp_);  
}


unsigned long 
AuFileSink::ByteSwapLong(unsigned long nLongNumber)
{
  return (((nLongNumber&0x000000FF)<<24)+((nLongNumber&0x0000FF00)<<8)+
	  ((nLongNumber&0x00FF0000)>>8)+((nLongNumber&0xFF000000)>>24));
}

unsigned short 
AuFileSink::ByteSwapShort (unsigned short nValue)
{
  return (((nValue>> 8)) | (nValue << 8));
}

void 
AuFileSink::putLinear16(realvec& slice)
{
  
  for (c=0; c < nChannels_; c++)
    for (t=0; t < inSamples_; t++)
      {
#if defined(__BIG_ENDIAN__)
	sdata_[t*nChannels_ + c] = (short)(slice(0,t) * MAXSHRT);      
#else
	sdata_[t*nChannels_ + c] = ByteSwapShort((short)(slice(0,t) * MAXSHRT));
	
#endif 
      }
  
  
  if ((mrs_natural)fwrite(sdata_, sizeof(short), nChannels_ * inSamples_, sfp_) != nChannels_ * inSamples_)
    {
      MRSWARN("Problem: could not write window to file" + filename_);
    }

}

void 
AuFileSink::process(realvec& in, realvec& out)
{

  checkFlow(in,out);
  
  // copy input to output 
  for (o=0; o < inObservations_; o++)
    for (t=0; t < inSamples_; t++)
      {
	if (in(o,t) > 1.0)
	  MRSWARN("AuFileSink::Value out of range > 1.0");
	if (in(o,t) < -1.0)
	  MRSWARN("AuFileSink::Value out of range < -1.0"); 

	  out(o,t) = in(o,t);
      }

  long fileSize;
  fpos_ = ftell(sfp_);
  fseek(sfp_, 8, SEEK_SET);
  written_ += inSamples_;
#if defined(__BIG_ENDIAN__)
  fileSize = (written_ * 2 * nChannels_);
#else
  fileSize = ByteSwapLong(written_ * 2 * nChannels_);
#endif

  fwrite(&fileSize, 4, 1, sfp_);
  fseek(sfp_, fpos_, SEEK_SET);
  
  putLinear16(in);

}








  
  



	

	
