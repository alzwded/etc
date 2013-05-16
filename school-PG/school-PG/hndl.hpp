inline void doRotate(int x, int y)
{
	if(!lastMouseF) return;
	float dx = wSize.first / 2 - x;
	float dy = wSize.second / 2 - y;

	_cameraAngle += (dx / wSize.first) * 90.0f;
	float tentativeHeadAngle = _headAngle + (dy / wSize.second) * 90.f;
	if(tentativeHeadAngle <= 90.f && tentativeHeadAngle >= -90.f) {
		_headAngle = tentativeHeadAngle;
	}

	while(_cameraAngle < -360.0f) _cameraAngle += 360.0f;
	while(_cameraAngle > 460.0f) _cameraAngle -= 360.0f;
}

void handleMouse(int button, int state, int x, int y)
{
	switch(button) {
	case GLUT_LEFT_BUTTON: {
		if(_blt.rounds) break;

		_blt.rounds = sbullet_decay;
		_blt.x = p.first;
		_blt.y = 0.f;
		_blt.z = -p.second;

		// default direction is 0, 0, -1
/*translationVector.x :=
          x * cosY * cosZ
        - y * (cosX * sinZ - sinX * sinY * cosZ)
        + z * (sinX * sinZ + cosX * sinY * cosZ);
  translationVector.y :=
          x * (cosY * sinZ)
        + y * (cosX * cosZ + sinX * sinY * sinZ)
        - z * (sinX * cosZ - cosX * sinY * sinZ);
  translationVector.z :=
        - x * sinY
        + y * sinX * cosY
        + z * cosX * cosY;*/
		float hrad = _headAngle * PI / 180.f;
		float crad = _cameraAngle * PI / 180.f;
		_blt.dx = (-1) * (cos(hrad) * sin(crad));
		_blt.dy = sin(hrad);
		_blt.dz = (-1) * (cos(hrad) * cos(crad));

		break; }
	}
}

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
	}
}

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

void handleResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 

	glLoadIdentity(); 
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0); 

	wSize = std::make_pair(w, h);
}