void initLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	
	glShadeModel(GL_SMOOTH);

	GLfloat lightpos[] = { 5.f, 40.f, 10.f, 0.f };
	GLfloat ambient[] = { 0.1f, .1f, .1f, 1.f};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat shiny[] = {50};
	GLfloat diffuse[] = { .5f, .5f, .5f, 1.f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	GLfloat l2[] = { -20.f, 40.f, -20.f, 0.f };
	glLightfv(GL_LIGHT1, GL_POSITION, l2);
	GLfloat diff2[] = {.5f, .2f, .2f, 1.f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff2);
	
	GLfloat l3[] = { 20.f, 40.f, -20.f, 0.f };
	glLightfv(GL_LIGHT2, GL_POSITION, l3);
	GLfloat diff3[] = {.0f, .0f, .3f, 1.f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diff3);

	GLfloat l4[] = { 20.f, 40.f, 20.f, 0.f };
	glLightfv(GL_LIGHT3, GL_POSITION, l4);
	GLfloat diff4[] = {.1f, .1f, .0f, .3f};
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diff4);

	// set materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
}

void initModel()
{
	_blt.rounds = 0;
}

void initRendering()
{
	initModel();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);

	// lighting
	initLight();

	glClearColor(.25f, .25f, 1.f, 1.f);
}