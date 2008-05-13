
#include "qglutapp.h"

QGlutApplication::QGlutApplication(int & argc, char ** argv) : QApplication(argc, argv), idleFunc(NULL)
{
	connect(this, SIGNAL(lastWindowClosed()), this, SLOT(quit()));
	
	connect(&idleTimer, SIGNAL(timeout()), this, SLOT(updateIdle()));
}

QGlutApplication::~QGlutApplication()
{
}

void QGlutApplication::updateIdle()
{
	idleFunc();
}

void QGlutApplication::timerEvent(QTimerEvent *event)
{
	if (timerToId.contains(event->timerId())) {
		int id = timerToId.value(event->timerId());
		
		const GlutTimer & timer = timerMap.value(id);
		timer.func(id);
	}
}

void QGlutApplication::setIdleFunc(void (*func)(void))
{
	if (idleFunc != NULL) {
		idleTimer.stop();
	}
	
	idleFunc = func;
	
	if (idleFunc != NULL) {
		idleTimer.start(0);
	}
}

void QGlutApplication::setTimerFunc(unsigned int msecs, void (*func)(int id), int id)
{
	// Remove previous timer.
	if (timerMap.contains(id)) {
		const GlutTimer & timer = timerMap.value(id);
		killTimer(timer.id);
		
		timerMap.remove(id);
		timerToId.remove(timer.id);
	}
	
	if (func != NULL) {
		GlutTimer timer = { startTimer(msecs), func };
		timerMap.insert(id, timer);
		timerToId.insert(timer.id, id);
	}
}

