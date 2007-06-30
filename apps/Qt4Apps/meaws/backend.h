#ifndef MEAWS_BACKEND_H
#define MEAWS_BACKEND_H

#include "defs.h"
#include <QObject>
#include "MarSystemManager.h"
#include "../MarSystemQtWrapper.h"
using namespace Marsyas;

class MarBackend: public QObject {
	Q_OBJECT

public:
	MarBackend(int getType);
	~MarBackend();

	void start();
	void stop();
//	void setFileName(std::string filename);
	void playFile();
	void open(std::string filename);

	void analyze();

private:
	MarSystemQtWrapper *mrsWrapper;
	MarSystemManager mng;
	MarSystem *sourceNet;
//	MarSystem *pitchNet;
//	MarSystem *amplitudeNet;
	MarSystem *allNet;

	realvec pitchList;
	int method;
	int introBeats;

// "constructor"
	MarSystem* makeSourceNet(std::string filename);
	MarSystem* makePitchNet(mrs_real source_osrate);
	MarSystem* makeAmplitudeNet();

	void startIntonation();
	void startControl();

	void setupAllNet();
	void delNet();
  MarControlPtr filenamePtr;
};
#endif

