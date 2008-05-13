
#ifndef QGLUTWIDGET_H
#define QGLUTWIDGET_H

#include <QtGui/QMainWindow>
#include <QtOpenGL/QGLWidget>

class QGlutWidget;

// Top level window.
class QGlutMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	QGlutMainWindow(const QGLFormat & format);
	~QGlutMainWindow();

	QGlutWidget * glutWidget();
	
	void setSize(int w, int h);
	//virtual QSize sizeHint () const;
	
public slots:
	
	void toggleFullScreen(bool checked);
	void toggleShowMenu(bool checked);
	
private:
	
	bool clientSizeSet;
	int clientWidth;
	int clientHeight;
	bool showMenu;
	
};


// GLUT window.
class QGlutWidget : public QGLWidget
{
	Q_OBJECT
public:
	QGlutWidget(const QGLFormat & format, QWidget * parent = 0);
	~QGlutWidget();

	void setDisplayFunc(void (*func)(void));
	void setOverlayDisplayFunc(void (*func)(void));
	void setReshapeFunc(void (*func)(int width, int height));
	void setKeyboardFunc(void (*func)(unsigned char key, int x, int y));
	void setKeyboardUpFunc(void (*func)(unsigned char key, int x, int y));
	void setMouseFunc(void (*func)(int button, int state, int x, int y));
	void setMotionFunc(void (*func)(int x, int y));
	void setPassiveMotionFunc(void (*func)(int x, int y));
	void setVisibilityFunc(void (*func)(int state));
	void setStatusFunc(void (*func)(int state));
	void setEntryFunc(void (*func)(int state));
	void setSpecialFunc(void (*func)(int key, int x, int y));
	void setSpecialUpFunc(void (*func)(int key, int x, int y));
	void setCloseFunc(void (*func)(void));
	
	bool ignoreKeyRepeat(bool ignore);
	
	
protected:

	virtual void initializeGL();
	virtual void paintGL();
	virtual void resizeGL(int width, int height);

	virtual void keyPressEvent(QKeyEvent * event);
	virtual void keyReleaseEvent(QKeyEvent * event);
	
	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void wheelEvent(QWheelEvent * event);
	
	virtual void closeEvent(QCloseEvent * event);
	
	virtual void enterEvent(QEvent * event);
	virtual void leaveEvent(QEvent * event);
	
	virtual void hideEvent(QHideEvent * event);
	virtual void showEvent(QShowEvent * event);
	

	
private:

	void (*displayFunc)(void);
	void (*overlayDisplayFunc)(void);
	void (*reshapeFunc)(int width, int height);
	void (*keyboardFunc)(unsigned char key, int x, int y);
	void (*keyboardUpFunc)(unsigned char key, int x, int y);
	void (*mouseFunc)(int button, int state, int x, int y);
	void (*motionFunc)(int x, int y);
	void (*passiveMotionFunc)(int x, int y);
	void (*visibilityFunc)(int state);
	void (*statusFunc)(int state);
	void (*entryFunc)(int state);
	void (*specialFunc)(int key, int x, int y);
	void (*specialUpFunc)(int key, int x, int y);
	void (*closeFunc)(void);

	bool m_ignoreKeyRepeat;
	
};


#endif // QGLUTWIDGET_H
