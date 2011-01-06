#ifndef QGLUT_H
#define QGLUT_H

#include <QtGui/QWidget>

class QMenuBar;
class QMainWindow;
class QApplication;

#ifdef WIN32
#  define QGLUT_APIENTRY __stdcall
#  define QGLUT_CALLBACK __stdcall
#else
#  define QGLUT_APIENTRY
#  define QGLUT_CALLBACK
#endif

extern "C"
{

//
// QGLUT API
//

//
// Get the QApplication singleton
//

extern QApplication * QGLUT_APIENTRY qglutGetApplication();

//
// Get the Qt main window of a GLUT window
//
// Note: There can be multiple GLUT windows within a main window)
//

extern QMainWindow * QGLUT_APIENTRY qglutGetMainWindow(int window);

//
// Get the window identifier (HWND on windows) of a GLUT window
//

extern WId QGLUT_APIENTRY qglutGetWindowID(int window);

};

#endif
