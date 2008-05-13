
#define glutAssert(expr)

#include <string.h>	// strstr

#include <QtGui/QApplication>
#include <QtGui/QCursor>

#include <GL/glut.h>

#include "qglutapp.h"
#include "qglutwidget.h"

namespace
{
	struct Private
	{
		Private() : app(NULL), window(NULL), sizeSet(false), posSet(false)
		{
			width = 300;
			height = 300;
			x = 100;
			y = 100;
		}
		~Private()
		{
		}
		
		QGlutApplication * app;
		QGlutMainWindow * mainWindow;
		QGlutWidget * window;
		
		bool sizeSet;
		bool posSet;
		
		QGLFormat format;
		int width, height;
		int x, y;
	};
	
	static Private s;

} // namespace

static void cleanup()
{
	if (s.window != NULL) {
		delete s.window;
		s.window = NULL;
	}
	if (s.app != NULL) {
		delete s.app;
		s.app = NULL;
	}
}

/** Init GLUT application. */
void glutInit(int *argc, char **argv)
{
	s.app = new QGlutApplication(*argc, argv);
	//atexit(cleanup);
}

void glutInitWindowSize(int width, int height)
{
	s.sizeSet = true;
	s.width = width;
	s.height = height;
}

void glutInitWindowPosition(int x, int y)
{
	s.posSet = true;
	s.x = x;
	s.y = y;
}

void glutInitDisplayMode(unsigned int mode)
{
	s.format = QGLFormat::defaultFormat();
	
	bool indexed = (mode & GLUT_INDEX) != 0;
	s.format.setRgba(!indexed);
	
	bool doubleBuffered = (mode & GLUT_DOUBLE) != 0;
	s.format.setDoubleBuffer(doubleBuffered);
	
	s.format.setAccum((mode & GLUT_ACCUM) != 0);
	s.format.setAlpha((mode & GLUT_ALPHA) != 0);
	s.format.setDepth((mode & GLUT_DEPTH) != 0);
	s.format.setStencil((mode & GLUT_STENCIL) != 0);
	s.format.setSampleBuffers((mode & GLUT_MULTISAMPLE) != 0);
	s.format.setStereo((mode & GLUT_STEREO) != 0);
	// GLUT_LUMINANCE
	//     Bit mask to select a window with a ``luminance'' color model. This model provides the functionality of OpenGL's RGBA color model, but the green and blue components are not maintained in the frame buffer. Instead each pixel's red component is converted to an index between zero and glutGet(GLUT_WINDOW_COLORMAP_SIZE)-1 and looked up in a per-window color map to determine the color of pixels within the window. The initial colormap of GLUT_LUMINANCE windows is initialized to be a linear gray ramp, but can be modified with GLUT's colormap routines.
}


void glutInitDisplayString(const char * str)
{
	s.format = QGLFormat::defaultFormat();
	
	QStringList list = QString(str).split(QRegExp("\\s+"));
	
	// @@ To implement this correctly we have to traverse all gl formats until we find one that matches.
	
	foreach(QString str, list)
	{
		QRegExp rx("(\\w+)(?:(=|!=|<|>|<=|>=|~)(\\d))?");
		rx.indexIn(str);
		
		int numCaptures = rx.numCaptures();
		
		if (numCaptures == 0) {
			// @@ Invalid token!
			continue;
		}
		
		bool hasRightHandSide = numCaptures == 3;
		
		// get capability name.
		QString capability = rx.cap(1);
		
		// get comparator.
		QString comparator = rx.cap(2);
		
		// get numeric value.
		int value = rx.cap(3).toInt();
		
		if (capability == "index") {
			s.format.setRgba(false);
		}
		else if (capability == "alpha") {
			s.format.setAlpha(true);
			if (hasRightHandSide) {
				s.format.setAlphaBufferSize(value);
			}
		}
		else if (capability == "acca" || capability == "acc") {
			s.format.setAccum(true);
		}
		else if (capability == "depth") {
			s.format.setDepth(true);
			if (hasRightHandSide) {
				s.format.setDepthBufferSize(value);
			}
		}
		else if (capability == "stencil") {
			s.format.setStencil(true);
			if (hasRightHandSide) {
				s.format.setStencilBufferSize(value);
			}
		}
		else if (capability == "double") {
			s.format.setDoubleBuffer(true);
		}
		else if (str.startsWith("samples")) {
			s.format.setSampleBuffers(true);
			if (hasRightHandSide) {
				s.format.setSamples(value);
			}
		}
		else if (str.startsWith("stereo")) {
			s.format.setStereo(true);
		}
	}
	
	/*
	alpha // Alpha color buffer precision in bits. Default is ">=1". 
 	acca // Red, green, blue, and alpha accumulation buffer precision in bits. Default is ">=1" for red, green, blue, and alpha capabilities. 
 	acc // Red, green, and green accumulation buffer precision in bits and zero bits of alpha accumulation buffer precision. Default is ">=1" for red, green, and blue capabilities, and "~0" for the alpha capability. 
 	blue // Blue color buffer precision in bits. Default is ">=1". 
 	buffer // Number of bits in the color index color buffer. Default is ">=1". 
 	conformant // Boolean indicating if the frame buffer configuration is conformant or not. Conformance information is based on GLX's EXT_visual_rating extension if supported. If the extension is not supported, all visuals are assumed conformat. Default is "=1". 
 	depth // Number of bits of precsion in the depth buffer. Default is ">=12". 
 	double // Boolean indicating if the color buffer is double buffered. Default is "=1". 
 	green // Green color buffer precision in bits. Default is ">=1". 
 	index // Boolean if the color model is color index or not. True is color index. Default is ">=1". 
 	num // A special capability name indicating where the value represents the Nth frame buffer configuration matching the description string. When not specified, glutInitDisplayString also returns the first (best matching) configuration. num requires a compartor and numeric value. 
 	red // Red color buffer precision in bits. Default is ">=1". 
 	rgba // Number of bits of red, green, blue, and alpha in the RGBA color buffer. Default is ">=1" for red, green, blue, and alpha capabilities, and "=1" for the RGBA color model capability. 
 	rgb // Number of bits of red, green, and blue in the RGBA color buffer and zero bits of alpha color buffer precision. Default is ">=1" for the red, green, and blue capabilities, and "~0" for alpha capability, and "=1" for the RGBA color model capability. 
 	luminance // Number of bits of red in the RGBA and zero bits of green, blue (alpha not specified) of color buffer precision. Default is ">=1" for the red capabilitis, and "=0" for the green and blue capabilities, and "=1" for the RGBA color model capability, and, for X11, "=1" for the StaticGray ("xstaticgray") capability. 
 	stencil // Number of bits in the stencil buffer. 
 	single // Boolean indicate the color buffer is single buffered. Double buffer capability "=1". 
 	stereo // Boolean indicating the color buffer is supports OpenGL-style stereo. Default is "=1". 
 	samples // Indicates the number of multisamples to use based on GLX's SGIS_multisample extension (for antialiasing). Default is "<=4". This default means that a GLUT application can request multipsampling if available by simply specifying "samples". 
 	slow // Boolean indicating if the frame buffer configuration is slow or not. For the X11 implementation of GLUT, slowness information is based on GLX's EXT_visual_rating extension if supported. If the EXT_visual_rating extension is not supported, all visuals are assumed fast. For the Win32 implementation of GLUT, slowness is based on if the underlying Pixel Format Descriptor (PFD) is marked "generic" and not "accelerated". This implies that Microsoft's relatively slow software OpenGL implementation is used by this PFD. Note that slowness is a relative designation relative to other frame buffer configurations available. The intent of the slow capability is to help programs avoid frame buffer configurations that are slower (but perhaps higher precision) for the current machine. Default is ">=0" if not comparator and numeric value are provided. This default means that slow visuals are used in preference to fast visuals, but fast visuals will still be allowed. 
	win32pfd // Only recognized on GLUT implementations for Win32, this capability name matches the Win32 Pixel Format Descriptor by numer. win32pfd requires a compartor and numeric value. 
 	xvisual // Only recongized on GLUT implementations for the X Window System, this capability name matches the X visual ID by number. xvisual requires a compartor and numeric value. 
 	xstaticgray // Only recongized on GLUT implementations for the X Window System, boolean indicating if the frame buffer configuration's X visual is of type StaticGray. Default is "=1". 
 	xgrayscale // Only recongized on GLUT implementations for the X Window System, boolean indicating if the frame buffer configuration's X visual is of type GrayScale. Default is "=1". 
 	xstaticcolor // Only recongized on GLUT implementations for the X Window System, boolean indicating if the frame buffer configuration's X visual is of type StaticColor. Default is "=1". 
 	xpseudocolor // Only recongized on GLUT implementations for the X Window System, boolean indicating if the frame buffer configuration's X visual is of type PsuedoColor. Default is "=1". 
 	xtruecolor // Only recongized on GLUT implementations for the X Window System, boolean indicating if the frame buffer configuration's X visual is of type TrueColor. Default is "=1". 
 	xdirectcolor // Only recongized on GLUT implementations for the X Window System, boolean indicating if the frame buffer configuration's X visual is of type DirectColor. Default is "=1".			
	*/
}


/** glutMainLoop enters the GLUT event processing loop. */
void glutMainLoop(void)
{
	glutAssert(app != NULL);
	s.app->exec();
}


/** glutCreateWindow creates a top-level window. */
int glutCreateWindow(const char * name)
{
	s.mainWindow = new QGlutMainWindow(s.format);
	s.mainWindow->setWindowTitle(name);
	
	s.window = s.mainWindow->glutWidget();
	
	if (s.sizeSet) {
		s.mainWindow->resize(s.width, s.height);
		//s.window->setSize(s.width, s.height);
	}
	if (s.posSet) {
		s.mainWindow->move(s.x, s.y);
	}
	
	s.mainWindow->show();
	
	// @@ Add support for multiple windows.
	return 0;
}

void glutSetWindow(int win)
{
	// @@ Add support for multiple windows.
}

int glutGetWindow(void)
{
	// @@ Add support for multiple windows.
	return 0;
}


void glutDestroyWindow(int win)
{
	// @@ Add support for multiple windows.
	if (win == 0) {
		glutAssert(s.window != NULL);
		delete s.mainWindow;
		s.mainWindow = NULL;
		s.window = NULL;
	}
}


void glutPostRedisplay(void)
{
	glutAssert(s.window != NULL);
	s.window->updateGL();
}

void glutSwapBuffers(void)
{
	glutAssert(s.window != NULL);
	s.window->swapBuffers();
}

void glutPositionWindow(int x, int y)
{
	glutAssert(s.mainWindow != NULL);
	s.mainWindow->toggleFullScreen(false);
	s.mainWindow->move(x, y);
}

void glutReshapeWindow(int width, int height)
{
	glutAssert(s.mainWindow != NULL);
	s.mainWindow->toggleFullScreen(false);
	s.mainWindow->resize(width, height);
}

void glutFullScreen(void)
{
	glutAssert(s.mainWindow != NULL);
	s.mainWindow->toggleFullScreen(true);
}

void glutPopWindow(void)
{
	glutAssert(false);
	// @@ Add support for multiple windows.
}

void glutPushWindow(void)
{
	glutAssert(false);
	// @@ Add support for multiple windows.
}

void glutShowWindow(void)
{
	glutAssert(s.window != NULL);
	s.mainWindow->show();
}

void glutHideWindow(void)
{
	glutAssert(s.window != NULL);
	s.mainWindow->hide();
}

void glutIconifyWindow(void)
{
	glutAssert(s.window != NULL);
//	s.mainWindow->showMinimized();
	s.mainWindow->setWindowState(s.mainWindow->windowState() | Qt::WindowMinimized);
}


void glutSetWindowTitle(char *name)
{
	glutAssert(s.window != NULL);
	s.mainWindow->setWindowTitle(name);
}

void glutSetIconTitle(char *name)
{
	glutAssert(s.window != NULL);
	s.mainWindow->setWindowIconText(name);
}


void glutWarpPointer(int x, int y)
{
	glutAssert(s.window != NULL);
	QPoint pos = s.window->mapToGlobal(QPoint(x, y));
	QCursor::setPos(pos);
}

void glutSetCursor(int shape)
{
	QCursor cursor;
	switch(shape) {
		case GLUT_CURSOR_RIGHT_ARROW:
			cursor.setShape(Qt::ArrowCursor);
			break;
		case GLUT_CURSOR_LEFT_ARROW:
			cursor.setShape(Qt::ArrowCursor);
			break;
		case GLUT_CURSOR_INFO:
			cursor.setShape(Qt::PointingHandCursor);
			break;
		case GLUT_CURSOR_DESTROY:
			// Skull & cross bones.
			cursor.setShape(Qt::ForbiddenCursor);
			break;
		case GLUT_CURSOR_HELP:
			cursor.setShape(Qt::WhatsThisCursor);
			break;
		// GLUT_CURSOR_CYCLE
		//     Arrows rotating in a circle. 
		// GLUT_CURSOR_SPRAY
		//     Spray can. 
		case GLUT_CURSOR_WAIT:
			cursor.setShape(Qt::WaitCursor);
			break;
		case GLUT_CURSOR_TEXT:
			cursor.setShape(Qt::IBeamCursor);
			break;
		case GLUT_CURSOR_CROSSHAIR:
			cursor.setShape(Qt::CrossCursor);
			break;
		case GLUT_CURSOR_UP_DOWN:
			cursor.setShape(Qt::SizeVerCursor);
			break;
		case GLUT_CURSOR_LEFT_RIGHT:
			cursor.setShape(Qt::SizeHorCursor);
			break;
		// GLUT_CURSOR_TOP_SIDE
		//     Arrow pointing to top side. 
		// GLUT_CURSOR_BOTTOM_SIDE
		//     Arrow pointing to bottom side. 
		// GLUT_CURSOR_LEFT_SIDE
		//     Arrow pointing to left side. 
		// GLUT_CURSOR_RIGHT_SIDE
		//     Arrow pointing to right side. 
		// GLUT_CURSOR_TOP_LEFT_CORNER
		//     Arrow pointing to top-left corner. 
		// GLUT_CURSOR_TOP_RIGHT_CORNER
		//     Arrow pointing to top-right corner. 
		// GLUT_CURSOR_BOTTOM_RIGHT_CORNER
		//     Arrow pointing to bottom-left corner. 
		// GLUT_CURSOR_BOTTOM_LEFT_CORNER
		//     Arrow pointing to bottom-right corner. 
		case GLUT_CURSOR_FULL_CROSSHAIR:
			cursor.setShape(Qt::CrossCursor);
		case GLUT_CURSOR_NONE:
			cursor.setShape(Qt::BlankCursor);
			break;
		// GLUT_CURSOR_INHERIT
		//     Use parent's cursor.
	}
	
	glutAssert(s.window != NULL);
	s.window->setCursor(cursor);
}


int glutGet(GLenum state)
{
	glutAssert(false);

	switch(state) {
		case GLUT_WINDOW_X:
			return s.mainWindow->x();
		case GLUT_WINDOW_Y:
			return s.mainWindow->y();
		case GLUT_WINDOW_WIDTH:
			return s.mainWindow->width();
		case GLUT_WINDOW_HEIGHT:
			return s.mainWindow->height();
		// GLUT_WINDOW_BUFFER_SIZE
		//     Total number of bits for current window's color buffer. For an RGBA window, this is the sum of GLUT_WINDOW_RED_SIZE, GLUT_WINDOW_GREEN_SIZE, GLUT_WINDOW_BLUE_SIZE, and GLUT_WINDOW_ALPHA_SIZE. For color index windows, this is the size of the color indexes. 
		// GLUT_WINDOW_STENCIL_SIZE
		//     Number of bits in the current window's stencil buffer. 
		// GLUT_WINDOW_DEPTH_SIZE
		//     Number of bits in the current window's depth buffer. 
		// GLUT_WINDOW_RED_SIZE
		//     Number of bits of red stored the current window's color buffer. Zero if the window is color index. 
		// GLUT_WINDOW_GREEN_SIZE
		//     Number of bits of green stored the current window's color buffer. Zero if the window is color index. 
		// GLUT_WINDOW_BLUE_SIZE
		//     Number of bits of blue stored the current window's color buffer. Zero if the window is color index. 
		// GLUT_WINDOW_ALPHA_SIZE
		//     Number of bits of alpha stored the current window's color buffer. Zero if the window is color index. 
		// GLUT_WINDOW_ACCUM_RED_SIZE
		//     Number of bits of red stored in the current window's accumulation buffer. Zero if the window is color index. 
		// GLUT_WINDOW_ACCUM_GREEN_SIZE
		//     Number of bits of green stored in the current window's accumulation buffer. Zero if the window is color index. 
		// GLUT_WINDOW_ACCUM_BLUE_SIZE
		//     Number of bits of blue stored in the current window's accumulation buffer. Zero if the window is color index. 
		// GLUT_WINDOW_ACCUM_ALPHA_SIZE
		//     Number of bits of alpha stored in the current window's accumulation buffer. Zero if the window is color index. 
		// GLUT_WINDOW_DOUBLEBUFFER
		//     One if the current window is double buffered, zero otherwise. 
		// GLUT_WINDOW_RGBA
		//     One if the current window is RGBA mode, zero otherwise (i.e., color index). 
		// GLUT_WINDOW_PARENT
		//     The window number of the current window's parent; zero if the window is a top-level window. 
		// GLUT_WINDOW_NUM_CHILDREN
		//     The number of subwindows the current window has (not counting children of children). 
		// GLUT_WINDOW_COLORMAP_SIZE
		//     Size of current window's color index colormap; zero for RGBA color model windows. 
		// GLUT_WINDOW_NUM_SAMPLES
		//     Number of samples for multisampling for the current window. 
		// GLUT_WINDOW_STEREO
		//     One if the current window is stereo, zero otherwise. 
		// GLUT_WINDOW_CURSOR
		//     Current cursor for the current window. 
		// GLUT_SCREEN_WIDTH
		//     Width of the screen in pixels. Zero indicates the width is unknown or not available. 
		// GLUT_SCREEN_HEIGHT
		//     Height of the screen in pixels. Zero indicates the height is unknown or not available. 
		case GLUT_SCREEN_WIDTH_MM:
			return 0;	// unknown
		case GLUT_SCREEN_HEIGHT_MM:
			return 0;	// unknown
		// GLUT_MENU_NUM_ITEMS
		//     Number of menu items in the current menu. 
		// GLUT_DISPLAY_MODE_POSSIBLE
		//     Whether the current display mode is supported or not. 
		// GLUT_INIT_DISPLAY_MODE
		//     The initial display mode bit mask. 
		case GLUT_INIT_WINDOW_X:
			return s.x;
		case GLUT_INIT_WINDOW_Y:
			return s.y;
		case GLUT_INIT_WINDOW_WIDTH:
			return s.width;
		case GLUT_INIT_WINDOW_HEIGHT:
			return s.height;
		// GLUT_ELAPSED_TIME
		//     Number of milliseconds since glutInit called (or first call to glutGet(GLUT_ELAPSED_TIME)).
	}
	
	return 0;
}


int glutGetModifiers(void)
{
	// @@ Ask the current window instead of the application.
	int glutModifier = 0;
	Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
	
	if (modifiers & Qt::ShiftModifier) {
		glutModifier |= GLUT_ACTIVE_SHIFT;
	}
	if (modifiers & Qt::ControlModifier) {
		glutModifier |= GLUT_ACTIVE_CTRL;
	}
	if (modifiers & Qt::AltModifier) {
		glutModifier |= GLUT_ACTIVE_ALT;
	}

	return 0;
}


int glutExtensionSupported(char *extension)
{
	const char * extensions = (const char *)glGetString(GL_EXTENSIONS);
	return strstr(extensions, extension) != NULL;
}

void glutReportErrors()
{
	while(true)
	{
		GLenum error = glGetError();
		
		if (error != GL_NO_ERROR)
		{
			fprintf(stderr, "OpenGL error: %s\n", gluErrorString(error));
		}
		else
		{
			break;
		}
	};
}


void glutDisplayFunc(void (*func)(void))
{
	glutAssert(s.window != NULL);
	s.window->setDisplayFunc(func);
}

void glutOverlayDisplayFunc(void (*func)(void))
{
	glutAssert(s.window != NULL);
	s.window->setOverlayDisplayFunc(func);
}

void glutReshapeFunc(void (*func)(int width, int height))
{
	glutAssert(s.window != NULL);
	s.window->setReshapeFunc(func);
}

void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
	glutAssert(s.window != NULL);
	s.window->setKeyboardFunc(func);
}

void glutKeyboardUpFunc(void (*func)(unsigned char key, int x, int y))
{
	glutAssert(s.window != NULL);
	s.window->setKeyboardUpFunc(func);
}

void glutMouseFunc(void (*func)(int button, int state, int x, int y))
{
	glutAssert(s.window != NULL);
	s.window->setMouseFunc(func);
}

void glutMotionFunc(void (*func)(int x, int y))
{
	glutAssert(s.window != NULL);
	s.window->setMotionFunc(func);
}

void glutPassiveMotionFunc(void (*func)(int x, int y))
{
	glutAssert(s.window != NULL);
	s.window->setPassiveMotionFunc(func);
}

void glutVisibilityFunc(void (*func)(int state))
{
	glutAssert(s.window != NULL);
	s.window->setVisibilityFunc(func);
}

void glutWindowStatusFunc(void (*func)(int state))
{
	glutAssert(s.window != NULL);
	s.window->setStatusFunc(func);
}

void glutEntryFunc(void (*func)(int state))
{
	glutAssert(s.window != NULL);
	s.window->setEntryFunc(func);
}

void glutSpecialFunc(void (*func)(int key, int x, int y))
{
	glutAssert(s.window != NULL);
	s.window->setSpecialFunc(func);
}

void glutSpecialUpFunc(void (*func)(int key, int x, int y))
{
	glutAssert(s.window != NULL);
	s.window->setSpecialUpFunc(func);
}

void glutMenuStatusFunc(void (*func)(int status, int x, int y))
{
	// @@ 
}

void glutMenuStateFunc(void (*func)(int status))
{
	// @@ 
}

void glutIdleFunc(void (*func)(void))
{
	glutAssert(s.window != NULL);
	s.app->setIdleFunc(func);
}

void glutTimerFunc(unsigned int msecs, void (*func)(int value), int value)
{
	glutAssert(s.window != NULL);
	s.app->setTimerFunc(msecs, func, value);
}

void glutWMCloseFunc(void (*func)(void))
{
	glutAssert(s.window != NULL);
	s.window->setCloseFunc(func);
}


// Menu functions

int glutCreateMenu(void (*func)(int value))
{
	// @@ 
}

void glutSetMenu(int menu)
{
	// @@ 
}

int glutGetMenu(void)
{
	// @@ 
}

void glutDestroyMenu(int menu)
{
	// @@ 
}

void glutAddMenuEntry(const char *name, int value)
{
	Q_UNUSED(name);
	Q_UNUSED(value);
	// @@ 
}

void glutAddSubMenu(const char *name, int menu)
{
	Q_UNUSED(name);
	Q_UNUSED(menu);
	// @@ 
}

void glutChangeToMenuEntry(int entry, const char *name, int value)
{
	// @@ 
}

void glutChangeToSubMenu(int entry, const char *name, int menu)
{
	// @@ 
}

void glutRemoveMenuItem(int entry)
{
	// @@ 
}

void glutAttachMenu(int button)
{
	// @@ 
}

void glutDetachMenu(int button)
{
	// @@ 
}

void glutIgnoreKeyRepeat(int ignore)
{
	glutAssert(s.window != NULL);
	s.window->ignoreKeyRepeat(ignore != 0);
}

