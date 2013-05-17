int main(int argc, char** argv)
{
	srand(clock());

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000); 

	glutCreateWindow("Windur");
	initRendering(); 

	glutSetCursor(GLUT_CURSOR_NONE);

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(handleMouse);
	glutPassiveMotionFunc(doRotate);
	glutMotionFunc(doRotate);
	glutReshapeFunc(handleResize);
	glutKeyboardUpFunc(handleKeyRelease);

	glutTimerFunc(25, update, 0); //Adauga timpul

	glutMainLoop(); 
	return 0;
}