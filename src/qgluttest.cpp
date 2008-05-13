
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

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
	glutInit(&argc, argv);
	
	//glutInitDisplayString("rgb depth double samples=4");
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 300, 300 );
	glutCreateWindow( "test" );
	glutKeyboardFunc( glutKeyboardCallback );
	glutKeyboardUpFunc( glutKeyboardUpCallback );
	glutDisplayFunc( glutDisplayCallback );
	glutIdleFunc( glutIdleCallback );
	
	
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
	
	glutMainLoop();
	
	return 0;
}

