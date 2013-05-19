int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	srand(clock());

	initGL();
	initRendering(); 
	initGL2();

	glutMainLoop(); 
	return 0;
}