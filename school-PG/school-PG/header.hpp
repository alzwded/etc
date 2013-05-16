

#define LIMIT(REF, CONST, OP) do { \
	if((REF) OP (CONST)) { \
		REF = (CONST); \
	} \
}while(0)

static const float sbullet_accel_scalar = 0.9;
static const float sbullet_decay = 16;
struct sbullet {
	int rounds;
	float x, y, z;
	float dx, dy, dz;
} _blt;

const float PI = atan(1.0) * 4;
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
	for(int i = 0; i < 3; ++i) {
		v[i] /= l;
	}
}

float* normalOf(float first, ...)
{
	va_list p;
	va_start(p, first);

	float* ret = (float*)calloc(3, sizeof(float));
	float p1[3] = { first, va_arg(p, float), va_arg(p, float) };
	float p2[3] = { va_arg(p, float), va_arg(p, float), va_arg(p, float) };
	float p3[3] = { va_arg(p, float), va_arg(p, float), va_arg(p, float) };

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

	va_end(p);

	return ret;
}