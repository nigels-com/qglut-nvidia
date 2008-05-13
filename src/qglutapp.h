
#ifndef QGLUTAPP_H
#define QGLUTAPP_H

#include <QtCore/QTimer>
#include <QtCore/QMap>
#include <QtGui/QApplication>


struct GlutTimer
{
	int id;
	void (*func)(int id);
};


class QGlutApplication : public QApplication
{
	Q_OBJECT
public:

	QGlutApplication(int & argc, char ** argv);
	~QGlutApplication();
	
	void setIdleFunc(void (*func)(void));
	void setTimerFunc(unsigned int msecs, void (*func)(int id), int id);

protected slots:
	void updateIdle();
	
protected:
	virtual void timerEvent(QTimerEvent *event);
	
private:
	
	QTimer idleTimer;
	QMap<int, GlutTimer> timerMap;	// id to timer
	QMap<int, int> timerToId;
	
	void (*idleFunc)(void);
	
};

#endif // QGLUTAPP_H
