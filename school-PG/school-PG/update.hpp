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

void updateBullet()
{
	if(_animation && _blt.rounds) {
		_blt.x += _blt.dx * sbullet_accel_scalar;
		_blt.y += _blt.dy * sbullet_accel_scalar;
		_blt.z += _blt.dz * sbullet_accel_scalar;
		--_blt.rounds;
	}
}

void update(int value) {
	updateAnimations();
	updatePointer();
	updatePlayer();
	updateBullet();

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}