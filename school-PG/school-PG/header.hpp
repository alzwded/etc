
static char _windowTitle[40];
static const float MAX_CUBES = 42;

#define LIMIT(REF, CONST, OP) do { \
	if((REF) OP (CONST)) { \
		REF = (CONST); \
	} \
}while(0)

static const float sbullet_accel_scalar = 2.0;
static const int sbullet_decay = 16;
static const float sbullet_size = 2.0f;
struct sbullet {
	int rounds;
	float x, y, z;
	float dx, dy, dz;
} _blt;

struct evil {
	float x, y, z;
	float dx, dy, dz;
	bool dead;
};
std::list<evil> _evilCubes;

static const float explosion_orange[] = { 1.0f, 0.5f, 0.0f, 0.8f };
static const float explosion_yellow[] = { 0.8f, 0.8f, 0.0f, 0.8f };
static const float explosion_basic_radius = 0.13f;
static const int explosion_decay = 25;
struct explosion {
	float x, y, z;
	int decay;
};
std::list<explosion> _explosions;

const float PI = atanf(1.0) * 4;
const float PI_2 = PI / 2.0f;

static const float playFieldWidth = 40.0f;
static const float playFieldLength = 40.0f;

bool _animation = true;
float _angle = 0.0f;
float _cameraAngle = 0.0f;
float _headAngle = 0.0f;

bool lastMouseF = false;
std::pair<int, int> wSize = std::make_pair(400, 400);

std::pair<float, float> dp = std::make_pair(0.f, 0.f);
std::pair<float, float> p = std::make_pair(0.f, -5.f);

void normalizeVector(float* v)
{
	float l = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if(l > -1.0e-5 && l < 1.0e-5) return;
	for(int i = 0; i < 3; ++i) {
		v[i] /= l;
	}
}

float* normalOf(float p01, float p02, float p03, float p11, float p12, float p13, float p21, float p22, float p23)
{
	float* ret = (float*)calloc(3, sizeof(float));
	float p1[3];// = { first, va_arg(p, float), va_arg(p, float) };
	float p2[3];// = { va_arg(p, float), va_arg(p, float), va_arg(p, float) };
	float p3[3];// = { va_arg(p, float), va_arg(p, float), va_arg(p, float) };
	p1[0] = p01; p1[1] = p02; p1[2] = p03;
	p2[0] = p11; p2[1] = p12; p2[2] = p13;
	p3[0] = p21; p3[1] = p22; p3[2] = p23;

	normalizeVector(p1);
	normalizeVector(p2);
	normalizeVector(p3);

	for(int i = 0 ; i < 3 ; ++i) {
		p2[i] -= p1[i];
		p3[i] -= p1[i];
	}

	for(int i = 0 ; i < 3 ; ++i) {
		ret[i] = p2[(i + 1) % 3] * p3[(i + 2) % 3] - p2[(i + 2) % 3] * p3[(i + 1) % 3];
	}

	normalizeVector(ret);

	return ret;
}

inline float getRandomFloat()
{
	return (float)(rand()) / (float)RAND_MAX;
}

void spawnCube()
{
	evil cube;

	cube.dead = false;

	cube.x = getRandomFloat() * 2.f * playFieldWidth - playFieldWidth;
	cube.z = getRandomFloat() * 2.f * playFieldLength - playFieldLength;
	cube.y = getRandomFloat() * (30.f - 3.5f) + 3.5f;

	cube.dx = getRandomFloat() * 2.0f - 1.0f;
	cube.dy = getRandomFloat() * 2.0f - 1.0f;
	cube.dz = getRandomFloat() * 2.0f - 1.0f;

	_evilCubes.push_back(cube);
}

void spawnAllCubes()
{
	_evilCubes.clear();
	for(int i = 0; i < MAX_CUBES; ++i) spawnCube();
}

void spawnExplosion(evil& cube)
{
	explosion x;
	x.x = cube.x;
	x.y = cube.y;
	x.z = cube.z;
	x.decay = explosion_decay;
	_explosions.push_back(x);
}