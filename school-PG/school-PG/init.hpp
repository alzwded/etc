void initLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	//glEnable(GL_LIGHT3);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glEnableClientState(GL_COLOR_ARRAY);
	
	glShadeModel(GL_SMOOTH);
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);

	GLfloat lightpos[] = { 5.f, 20.f, 10.f, 0.f };
	GLfloat ambient[] = { 0.2f, .2f, .2f, 1.f};
	GLfloat specular[] = {8.0, 8.0, 8.0, 1.0f};
	//GLfloat specular[] = {.0, .0, .0, 1.0f};
	GLfloat shiny[] = {50.0};
	GLfloat diffuse[] = { .5f, .5f, .5f, 1.f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.4f);

	GLfloat l2[] = { -20.f, 20.f, -20.f, 0.f };
	glLightfv(GL_LIGHT1, GL_POSITION, l2);
	GLfloat diff2[] = {.5f, .2f, .2f, 1.f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	
	GLfloat l3[] = { 20.f, 20.f, -20.f, 0.f };
	glLightfv(GL_LIGHT2, GL_POSITION, l3);
	GLfloat diff3[] = {.0f, .0f, .3f, 1.f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diff3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5f);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.4f);

	//GLfloat l4[] = { 20.f, 40.f, 20.f, 0.f };
	GLfloat l4[] = { 30.f, 20.f, -30.f, 0.f };
	glLightfv(GL_LIGHT3, GL_POSITION, l4);
	GLfloat diff4[] = {.1f, .1f, .0f, 1.f};
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diff4);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular);

	// set materials
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
	glMateriali(GL_FRONT, GL_SHININESS, 0);
}

void initModel()
{
	_blt.rounds = 0;
	_score = score_start;
	_framesSinceLastShot = 0;
}

void initBlending()
{
	glEnable(GL_BLEND);
}

void initRendering()
{
	initModel();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// lighting
	initLight();

	// blending
	initBlending();

	glClearColor(.25f, .25f, 1.f, 1.f);
}