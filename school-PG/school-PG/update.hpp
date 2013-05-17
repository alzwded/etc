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
	if(m < 10.0f - sbullet_size) {
		cube.dead = true;
	}
}

void updateBullet()
{
	if(_animation && _blt.rounds) {
		_blt.x += _blt.dx * sbullet_accel_scalar;
		_blt.y += _blt.dy * sbullet_accel_scalar;
		_blt.z += _blt.dz * sbullet_accel_scalar;
		--_blt.rounds;

		std::for_each(_evilCubes.begin(), _evilCubes.end(), collision);
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
	if(newY <= 3.5 || newY >= 30.0) {
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

void update(int value)
{
	updateAnimations();
	updatePointer();
	updatePlayer();
	updateBullet();
	updateCubes();

	sprintf(_windowTitle, "JakCube - %d cubes left", _evilCubes.size());
	glutSetWindowTitle(_windowTitle);

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}