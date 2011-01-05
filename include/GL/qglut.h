#ifndef QGLUT_H
#define QGLUT_H

#include <QtGui/QWidget>

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
// Get the window identifier (HWND on windows) for a GLUT window
//

extern WId QGLUT_APIENTRY qglutGetWindowID(int window);

};

#endif
