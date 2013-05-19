void prologue()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	glEnable(GL_DEPTH_TEST);
}

void epilogue()
{
	glPopMatrix();
	glutSwapBuffers(); 
}

void drawFloor()
{
	glBegin(GL_QUADS); {
		const GLfloat* floorC = environment_colors[_colorIndex];
		glMaterialfv(GL_FRONT, GL_EMISSION, floorC);

		static const float ydepth = -1.3f;
		float* n = normalOf(
			-playFieldWidth, ydepth, -playFieldLength,
			-playFieldWidth, ydepth, playFieldLength,
			playFieldWidth, ydepth, playFieldLength);
		glNormal3d(n[0], n[1], n[2]);

		glVertex3f(-playFieldWidth, ydepth, -playFieldLength);
		glVertex3f(-playFieldWidth, ydepth, playFieldLength);
		glVertex3f(playFieldWidth, ydepth, playFieldLength);
		glVertex3f(playFieldWidth, ydepth, -playFieldLength);
		delete[] n;

		static const float yheight = 40.f;
		n = normalOf(
			-playFieldWidth, yheight, playFieldLength,
			-playFieldWidth, yheight, -playFieldLength,
			playFieldWidth, yheight, playFieldLength);
		glNormal3d(n[0], n[1], n[2]);

		glVertex3f(-playFieldWidth, yheight, -playFieldLength);
		glVertex3f(playFieldWidth, yheight, -playFieldLength);
		glVertex3f(playFieldWidth, yheight, playFieldLength);
		glVertex3f(-playFieldWidth, yheight, playFieldLength);
		delete[] n;

		// poles
		static const float girth = 5.0f;
		float* northSouth = normalOf(
			0.f, 1.f, 0.f,
			0.f, 0.f, 0.f,
			1.f, 1.f, 0.f);
		float* westEast = normalOf(
			0.f, 0.f, 1.f,
			0.f, 0.f, 0.f,
			0.f, 1.f, 1.f);
		float* eastWest = normalOf(
			0.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 1.f, -1.f);
		float* southNorth = normalOf(
			0.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			1.f, 1.f, 0.f);
			
		for(float i = -playFieldWidth / 2.0f;
			i < (playFieldWidth - 5.0e-3) / 2.0f + girth;
			i += girth)
		{
			glNormal3d(northSouth[0], northSouth[1], northSouth[2]);
			glVertex3f(2 * i, ydepth, -playFieldLength);
			glVertex3f(2 * i + girth, ydepth, -playFieldLength);
			glVertex3f(2 * i + girth, yheight, -playFieldLength);
			glVertex3f(2 * i, yheight, -playFieldLength);

			glNormal3d(southNorth[0], southNorth[1], southNorth[2]);
			glVertex3f(2 * i, ydepth, playFieldLength);
			glVertex3f(2 * i, yheight, playFieldLength);
			glVertex3f(2 * i + girth, yheight, playFieldLength);
			glVertex3f(2 * i + girth, ydepth, playFieldLength);

			glNormal3d(westEast[0], westEast[1], westEast[2]);
			glVertex3f(2 * i + girth, ydepth, -playFieldLength);
			glVertex3f(2 * i + girth, ydepth, -playFieldLength - girth);
			glVertex3f(2 * i + girth, yheight, -playFieldLength - girth);
			glVertex3f(2 * i + girth, yheight, -playFieldLength);

			glNormal3d(eastWest[0], eastWest[1], eastWest[2]);
			glVertex3f(2 * i, ydepth, -playFieldLength);
			glVertex3f(2 * i, yheight, -playFieldLength);
			glVertex3f(2 * i, yheight, -playFieldLength - girth);
			glVertex3f(2 * i, ydepth, -playFieldLength - girth);

			glNormal3d(westEast[0], westEast[1], westEast[2]);
			glVertex3f(2 * i + girth, ydepth, playFieldLength);
			glVertex3f(2 * i + girth, yheight, playFieldLength);
			glVertex3f(2 * i + girth, yheight, playFieldLength + girth);
			glVertex3f(2 * i + girth, ydepth, playFieldLength + girth);

			glNormal3d(eastWest[0], eastWest[1], eastWest[2]);
			glVertex3f(2 * i, ydepth, playFieldLength);
			glVertex3f(2 * i, ydepth, playFieldLength + girth);
			glVertex3f(2 * i, yheight, playFieldLength + girth);
			glVertex3f(2 * i, yheight, playFieldLength);
		}
		
		for(float i = -playFieldLength / 2.0f + girth/2.0f;
			i < (playFieldLength - 5.0e-3) / 2.0f + girth/2.0f;
			i += girth)
		{
			glNormal3d(westEast[0], westEast[1], westEast[2]);
			glVertex3f(-playFieldWidth, ydepth, 2 * i);
			glVertex3f(-playFieldWidth, yheight, 2 * i);
			glVertex3f(-playFieldWidth, yheight, 2 * i + girth);
			glVertex3f(-playFieldWidth, ydepth, 2 * i + girth);

			glNormal3d(eastWest[0], eastWest[1], eastWest[2]);
			glVertex3f(playFieldWidth, ydepth, 2 * i);
			glVertex3f(playFieldWidth, ydepth, 2 * i + girth);
			glVertex3f(playFieldWidth, yheight, 2 * i + girth);
			glVertex3f(playFieldWidth, yheight, 2 * i);

			glNormal3d(northSouth[0], northSouth[1], northSouth[2]);
			glVertex3f(-playFieldWidth - girth, ydepth, 2 * i + girth);
			glVertex3f(-playFieldWidth, ydepth, 2 * i + girth);
			glVertex3f(-playFieldWidth, yheight, 2 * i + girth);
			glVertex3f(-playFieldWidth - girth, yheight, 2 * i + girth);
			glNormal3d(northSouth[0], northSouth[1], northSouth[2]);
			glVertex3f(playFieldWidth + girth, ydepth, 2 * i + girth);
			glVertex3f(playFieldWidth + girth, yheight, 2 * i + girth);
			glVertex3f(playFieldWidth, yheight, 2 * i + girth);
			glVertex3f(playFieldWidth, ydepth, 2 * i + girth);
			
			glNormal3d(southNorth[0], southNorth[1], southNorth[2]);
			glVertex3f(-playFieldWidth - girth, ydepth, 2 * i);
			glVertex3f(-playFieldWidth - girth, yheight, 2 * i);
			glVertex3f(-playFieldWidth, yheight, 2 * i);
			glVertex3f(-playFieldWidth, ydepth, 2 * i);
			glNormal3d(southNorth[0], southNorth[1], southNorth[2]);
			glVertex3f(playFieldWidth + girth, ydepth, 2 * i);
			glVertex3f(playFieldWidth, ydepth, 2 * i);
			glVertex3f(playFieldWidth, yheight, 2 * i);
			glVertex3f(playFieldWidth + girth, yheight, 2 * i);
		}
		delete[] northSouth;
		delete[] westEast;
		delete[] eastWest;
		delete[] southNorth;
	} glEnd();
}

void drawBullet()
{
	glPushMatrix();
	glTranslatef(_blt.x, _blt.y, _blt.z);
	glRotatef(_cameraAngle, .0f, 1.f, .0f);
	glRotatef(_headAngle, 1.f, 0.f, 0.f);
	GLfloat c[] = {1.f, .0f, .0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_EMISSION, c);

	//glutSolidCube(sbullet_size);
	glutSolidSphere(sbullet_size, 16, 16);
	
	/*glBegin(GL_TRIANGLES); {

		static const float down = 0.15f;
		static const float left = 0.3f;

		float* n = normalOf(0, 0, 0,
			-left, -down, 0,
			+left, -down, 0);
		glNormal3d(n[0], n[1], n[2]);

		glVertex3f(0, 0, 0);
		glVertex3f(-left, -down, 0);
		glVertex3f(+left, -down, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(+left, -down, 0);
		glVertex3f(-left, -down, 0);
		delete[] n;
	} glEnd(); */
	glPopMatrix();
}

void drawDirectors()
{

	glBegin(GL_TRIANGLES); {
		GLfloat c[] = {0.f, 0.f, 0.f, .5f};
		glMaterialfv(GL_FRONT, GL_EMISSION, c);

		static const float xm = -.5f;
		static const float zm = 5.f;

		float* n = normalOf(xm + .5f, 10.0f, zm + 0.0f,
			xm + .75f, 10.0f, zm + .75f,
			xm + .25f, 10.0f, zm + 0.75f);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3f(xm + .5f, 10.0f, zm + 0.0f);
		glVertex3f(xm + .75f, 10.0f, zm + .75f);
		glVertex3f(xm + .25f, 10.0f, zm + 0.75f);
		delete[] n;
		
		n = normalOf(xm + .5f, -10.0f, zm + 0.0f,
			xm + .75f, -10.0f, zm + .75f,
			xm + .25f, -10.0f, zm + .75f);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3f(xm + .5f, -10.0f, zm + 0.0f);
		glVertex3f(xm + .75f, -10.0f, zm + .75f);
		glVertex3f(xm + .25f, -10.0f, zm + .75f);
		delete[] n;

		n = normalOf(5.f, 0.f, zm + 0.f,
			5.f, -.25f, zm + .75f,
			5.f, .25f, zm + .75f);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3f(5.f, 0.f, zm + 0.f);
		glVertex3f(5.f, -.25f, zm + .75f);
		glVertex3f(5.f, .25f, zm + .75f);
		delete[] n;
		
		n = normalOf(-5.f, 0.f, zm + 0.f,
			-5.f, .25f, zm + .75f,
			-5.f, -.25f, zm + .75f);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3f(-5.f, 0.f, zm + 0.f);
		glVertex3f(-5.f, .25f, zm + .75f);
		glVertex3f(-5.f, -.25f, zm + .75f);
		delete[] n;

		// reverse
		n = normalOf(
			xm + .75f, 10.0f, zm + .75f,
			xm + .5f, 10.0f, zm + 0.0f,
			xm + .25f, 10.0f, zm + 0.75f);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3f(xm + .5f, 10.0f, zm + 0.0f);
		glVertex3f(xm + .25f, 10.0f, zm + 0.75f);
		glVertex3f(xm + .75f, 10.0f, zm + .75f);
		delete[] n;
		
		n = normalOf(
			xm + .75f, -10.0f, zm + .75f,
			xm + .5f, -10.0f, zm + 0.0f,
			xm + .25f, -10.0f, zm + .75f);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3f(xm + .5f, -10.0f, zm + 0.0f);
		glVertex3f(xm + .25f, -10.0f, zm + .75f);
		glVertex3f(xm + .75f, -10.0f, zm + .75f);
		delete[] n;

		n = normalOf(
			5.f, -.25f, zm + .75f,
			5.f, 0.f, zm + 0.f,
			5.f, .25f, zm + .75f);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3f(5.f, 0.f, zm + 0.f);
		glVertex3f(5.f, .25f, zm + .75f);
		glVertex3f(5.f, -.25f, zm + .75f);
		delete[] n;
		
		n = normalOf(
			-5.f, .25f, zm + .75f,
			-5.f, 0.f, zm + 0.f,
			-5.f, -.25f, zm + .75f);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3f(-5.f, 0.f, zm + 0.f);
		glVertex3f(-5.f, -.25f, zm + .75f);
		glVertex3f(-5.f, .25f, zm + .75f);
		delete[] n;
	} glEnd();
}

void drawCube(evil& cube)
{
	// remind me to fix this later
	glPushMatrix();
	glTranslatef(cube.x, cube.y, cube.z);
	gluLookAt(0, 0, 0, cube.dx, cube.dy, cube.dz, 0, 1, 0);
	/*float target[3];
	target[0] = cube.dx; target[1] = cube.dy; target[2] = cube.dz;
	normalizeVector(target);
	float z[3] = { 0, 0, 1 };
	float prod = 0;
	for(int i = 0; i < 3; ++i) {
		prod += target[i] * z[i];
	}
	float angle = acos(prod);
	if(fabs(angle) > 1.0e-5) {
		float cross[3];
		for(int i = 0; i < 3; ++i) {
			cross[i] = target[(i + 1) % 3] * z[(i + 2) % 3] - target[(i + 2) % 3] * z[(i + 1) % 3];
		}
		glRotatef(angle, cross[0], cross[1], cross[2]);
	}*/

	glutSolidCube(5.0f);

	glPopMatrix();
}

void drawCubes()
{
	GLfloat c[] = {.7f, .4f, .4f, 1.0f};
	glMaterialfv(GL_FRONT, GL_EMISSION, c);

	std::for_each(_evilCubes.begin(), _evilCubes.end(), drawCube);
	/*glTranslatef(0.f, 0.f, 0.f);
	glRotatef(45, 0.0f, 1.0f, 0.0f);
	glutSolidCube(.5f);*/
}

void crossHairs()
{
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 100.0f, 100.0f, 0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);

	GLfloat c[] = { 1.0f, 1.0f, 1.0f, 0.5f };
	glMaterialfv(GL_FRONT, GL_EMISSION, c);
	glLineWidth(1.0f);
	glPointSize(2.0f);
	glBegin(GL_POINTS); {
		glVertex2f(50.0f, 50.0f);
	} glEnd();

	// hi score
	char s[6];
	sprintf_s(s, 6, "%5d", _score);
	glPushMatrix();
	glTranslatef(score_leftMost, score_cellSize, 0.0f);
	glLineWidth(3.0f);
	glScalef(0.09f, -0.09f, 0.0f);
	for(int i = 0; s[i] != '\0'; ++i) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
	}
	glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawExplosion(explosion& e)
{
	glPushMatrix();
	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);

	switch(e.decay % 6) {
		case 0:
		case 1:
		case 2:
			glMaterialfv(GL_FRONT, GL_EMISSION, explosion_orange);
			break;
		case 3:
		case 4:
		case 5:
			glMaterialfv(GL_FRONT, GL_EMISSION, explosion_yellow);
			break;
	}

	glTranslatef(e.x, e.y, e.z);
	float axis[3];
	axis[0] = getRandomFloat() * 2.0f - 1.0f;
	axis[1] = getRandomFloat() * 2.0f - 1.0f;
	axis[2] = getRandomFloat() * 2.0f - 1.0f;
	normalizeVector(axis);
	glRotatef(
		getRandomFloat() * 180.0f,
		axis[0], axis[1], axis[2]);
	glutSolidSphere(explosion_basic_radius * (explosion_decay - e.decay), 16, 16);
	

	switch(e.decay % 6) {
		case 0:
		case 1:
		case 2:
			glMaterialfv(GL_FRONT, GL_EMISSION, explosion_yellow);
			break;
		case 3:
		case 4:
		case 5:
			glMaterialfv(GL_FRONT, GL_EMISSION, explosion_orange);
			break;
	}
	glutSolidCube(PI_2 * 0.86f * explosion_basic_radius * (explosion_decay - e.decay));

	glBlendFunc(GL_ONE, GL_ZERO);

	glPopMatrix();
}

void drawExplosions()
{
	std::for_each(_explosions.begin(), _explosions.end(), drawExplosion);
}

void moveWorldAccordingToMe()
{
	glRotatef(-_headAngle, 1.0f, 0.0f, 0.0f); // yCamera
	glRotatef(-_cameraAngle, 0.0f, 1.0f, 0.0f); //Roteste camera
	glTranslatef(-p.first, 0.0f, p.second);
}

void drawScene()
{
	prologue();

	moveWorldAccordingToMe();

	drawFloor();

	if(_blt.rounds) {
		drawBullet();
	}

	drawDirectors();

	drawCubes();

	drawExplosions();

	crossHairs();

	epilogue();
}