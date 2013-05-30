/**
 * roteste camera pe x respective pe y si normalizeaza unghiurile
 */
void doRotate(int x, int y)
{
	if(!lastMouseF) return;
	float dx = wSize.first / 2.0f - x;
	float dy = wSize.second / 2.0f - y;

	_cameraAngle += (dx / wSize.first) * 90.0f;
	float tentativeHeadAngle = _headAngle + (dy / wSize.second) * 90.f;
	// limiteaza camera pe axa X -- nu putem sa ne sucim gaturile, totusi
	if(tentativeHeadAngle <= 90.f && tentativeHeadAngle >= -90.f) {
		_headAngle = tentativeHeadAngle;
	}

	while(_cameraAngle < -360.0f) _cameraAngle += 360.0f;
	while(_cameraAngle > 360.0f) _cameraAngle -= 360.0f;
}

/**
 * trateaza butoanele mausului
 */
void handleMouse(int button, int state, int x, int y)
{
	switch(button) {
	case GLUT_LEFT_BUTTON: {
		if(_blt.rounds) break;

		_bonus = 0u;

		_blt.rounds = sbullet_decay;
		_blt.x = p.first;
		_blt.y = 0.f;
		_blt.z = -p.second;

		float hrad = _headAngle * PI / 180.f;
		float crad = _cameraAngle * PI / 180.f;
		_blt.dx = (-1) * (cos(hrad) * sin(crad));
		_blt.dy = sin(hrad);
		_blt.dz = (-1) * (cos(hrad) * cos(crad));

		break; }
	}
}

/**
 * trateaza butoanele tastaturii
 */
void handleKeypress(unsigned char key, int x, int y)
{ 
	switch (key) {
		case 27: 
			exit(0); 
		case 32:
			_animation = !_animation;
			break;
		case 'w':
			dp.second = 1.0f;
			break;
		case 's':
			dp.second = -1.0f;
			break;
		case 'a':
			dp.first = -1.0f;
			break;
		case 'd':
			dp.first = 1.0f;
			break;
		case 13:
			spawnAllCubes();
			break;
	}
}

/**
 * trateaza butoanele tastaturii
 */
void handleKeyRelease(unsigned char key, int x, int y)
{
	switch (key) {
		case 'w':
		case 's':
			dp.second = .0f;
			break;
		case 'a':
		case 'd':
			dp.first = .0f;
			break;
	}
}

/**
 * adapteaza fereastra in urma redimensiunarii
 */
void handleResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 

	glLoadIdentity(); 
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0); 

	wSize = std::make_pair(w, h);
}
