
#include <cstdio>
#include <cstdlib>

#include <GL/glut.h>

#include <GL/qglut.h>

#include <QtCore/QObject>

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMainWindow>
#include <QtGui/QApplication>

using namespace std;

void glutKeyboardCallback(unsigned char key, int x, int y)
{
	printf( "keydn=%i\n", key );

	if (key == 27) {
		exit(0);
	}
}

void glutKeyboardUpCallback(unsigned char key, int x, int y)
{
	printf( "keyup=%i\n", key );
}

void glutDisplayCallback(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0.5, 0.5);
	glEnd();

	glutSwapBuffers();
}

void glutIdleCallback(void)
{
	//glutPostRedisplay();
}

void selectMessage(int value)
{
}
void selectColor(int value)
{
}
void selectFont(int value)
{
}

int main(int argc, char *argv[])
{
	// Use GLUT to create window

	glutInit(&argc, argv);
	//glutInitDisplayString("rgb depth double samples=4");
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 300, 300 );
	glutCreateWindow( "test" );
	glutKeyboardFunc( glutKeyboardCallback );
	glutKeyboardUpFunc( glutKeyboardUpCallback );
	glutDisplayFunc( glutDisplayCallback );
	glutIdleFunc( glutIdleCallback );

	// Use Qt to setup main window menu bar
	
	QApplication *application = qglutGetApplication();
	QMainWindow  *mainWindow  = qglutGetMainWindow(glutGetWindow());
	QMenuBar     *menuBar     = mainWindow ? mainWindow->menuBar() : NULL;

	if (application && mainWindow && menuBar)
	{
		QAction *action = NULL;

	#ifndef __APPLE__
		QMenu *fileMenu = menuBar->addMenu(QObject::tr("&File"));

		action = new QAction(QObject::tr("E&xit"),application);
		action->setShortcut(QObject::tr("Ctrl+Q"));
		action->setStatusTip(QObject::tr("Exit the application"));
		QObject::connect(action, SIGNAL(triggered()), mainWindow, SLOT(close()));
		fileMenu->addAction(action);
		mainWindow->addAction(action);
	#endif

		QMenu *settingsMenu = menuBar->addMenu(QObject::tr("&Settings"));

		action = new QAction(QObject::tr("&Toggle fullscreen"),application);
		action->setShortcut(QObject::tr("Ctrl+F"));
		action->setCheckable(true);
		action->setChecked(false);
		QObject::connect(action, SIGNAL(toggled(bool)), mainWindow, SLOT(toggleFullScreen(bool)));
		settingsMenu->addAction(action);
		mainWindow->addAction(action);

	#ifndef __APPLE__
		action = new QAction(QObject::tr("&Show menu bar"),application);
		action->setShortcut(QObject::tr("Ctrl+M"));
		action->setCheckable(true);
		action->setChecked(true);
		QObject::connect(action, SIGNAL(toggled(bool)), mainWindow, SLOT(toggleShowMenu(bool)));
		settingsMenu->addAction(action);
		mainWindow->addAction(action);
	#endif

		QMenu * helpMenu = menuBar->addMenu(QObject::tr("&Help"));

		action = new QAction(QObject::tr("About &Qt"),application);
		action->setStatusTip(QObject::tr("Show the Qt library's about box"));
		QObject::connect(action, SIGNAL(triggered()), application, SLOT(aboutQt()));
		helpMenu->addAction(action);

		action = new QAction(QObject::tr("About Q&GLUT"),application);
		action->setStatusTip(QObject::tr("Show the QGLUT library's about box"));
		QObject::connect(action, SIGNAL(triggered()), application, SLOT(aboutQGlut()));
		helpMenu->addAction(action);
	}

	int submenu1 = glutCreateMenu(selectMessage);
	glutAddMenuEntry("abc", 1);
	glutAddMenuEntry("ABC", 2);
	int submenu2 = glutCreateMenu(selectColor);
	glutAddMenuEntry("Green", 1);
	glutAddMenuEntry("Red", 2);
	glutAddMenuEntry("White", 3);
	glutCreateMenu(selectFont);
	glutAddMenuEntry("9 by 15", 0);
	glutAddMenuEntry("Times Roman 10", 1);
	glutAddMenuEntry("Times Roman 24", 2);
	glutAddSubMenu("Messages", submenu1);
	glutAddSubMenu("Color", submenu2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Use GLUT main loop

	glutMainLoop();

	return 0;
}
