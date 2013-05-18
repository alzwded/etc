void updateAnimations()
{
	if(_animation) {
		_angle += 2.0f;
		if (_angle > 360) {
			_angle -= 360;
		}
	}
}

void updatePointer()
{
	lastMouseF = false;
	glutWarpPointer(wSize.first / 2, wSize.second / 2);
	lastMouseF = true;
}

void updatePlayer()
{
	static const float scaleM = 0.2f;
	std::pair<float, float> ldp;
	float anglInRads = _cameraAngle * PI / 180.0f;
	float _sin = sin(anglInRads);
	float _cos = cos(anglInRads);
	ldp.first = dp.first * _cos - dp.second * _sin;
	ldp.second = dp.first * _sin + dp.second * _cos;
	p.first = p.first + scaleM * ldp.first;
	p.second = p.second + scaleM * ldp.second;
	LIMIT(p.first, -playFieldWidth, <);
	LIMIT(p.first, playFieldWidth, >);
	LIMIT(p.second, -playFieldLength, <);
	LIMIT(p.second, playFieldLength, >);
}

void collision(evil& cube)
{
	float v[3];
	v[0] = cube.x - _blt.x;
	v[1] = cube.y - _blt.y;
	v[2] = cube.z - _blt.z;
	float m = 0;
	for(int i = 0; i < 3; ++i) m += v[i] * v[i];
	if(m < 10.0f * sqrtf(3.0f) + sbullet_size) {
		cube.dead = true;
		spawnExplosion(cube);
	}
}

void updateBullet()
{
	if(!_animation) return;
	if(_blt.rounds) {
		_blt.x += _blt.dx * sbullet_accel_scalar;
		_blt.y += _blt.dy * sbullet_accel_scalar;
		_blt.z += _blt.dz * sbullet_accel_scalar;
		--_blt.rounds;

		std::for_each(_evilCubes.begin(), _evilCubes.end(), collision);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	} else {
		glClearColor(.25f, .25f, 1.f, 1.f);
	}
}

void updateOneCube(evil& cube)
{
	float newX = cube.x + cube.dx;
	float newY = cube.y + cube.dy;
	float newZ = cube.z + cube.dz;

	if(newX <= -playFieldWidth || newX >= playFieldWidth) {
		cube.dx = -cube.dx;
	}
	if(newY <= cube_min_Y || newY >= cube_max_Y) {
		cube.dy = -cube.dy;
	}
	if(newZ <= -playFieldLength || newZ >= playFieldLength) {
		cube.dz = -cube.dz;
	}

	static const float scale = 0.3f;

	cube.x += cube.dx * scale;
	cube.y += cube.dy * scale;
	cube.z += cube.dz * scale;
}

inline bool isDead(evil& cube)
{
	return cube.dead;
}

void updateCubes()
{
	if(!_animation) return;
	std::for_each(_evilCubes.begin(), _evilCubes.end(), updateOneCube);
	_evilCubes.remove_if(isDead);
	if(_evilCubes.empty()) spawnAllCubes();
}

void decreaseDecay(explosion& e)
{
	e.decay--;
}

inline bool isExplosionDone(explosion& e)
{
	return e.decay <= 0;
}

void updateExplosions()
{
	if(!_animation) return;
	std::for_each(_explosions.begin(), _explosions.end(), decreaseDecay);
	_explosions.remove_if(isExplosionDone);
}

void update(int value)
{
	updateAnimations();
	updatePointer();
	updatePlayer();
	updateBullet();
	updateCubes();
	updateExplosions();

	sprintf_s(_windowTitle, 39, "JakCube - %d cubes left", _evilCubes.size());
	_windowTitle[39] = NULL;
	glutSetWindowTitle(_windowTitle);

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}