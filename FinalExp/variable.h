#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED
//太阳，地球，月亮的材质属性
float sunDiffuse[] = {1.0f, 1.0f, 0.0f, 1.0f};    // 太阳的反射率
float earthDiffuse[] = {0.2f, 0.2f, 1.0f, 1.0f};  // 地球的反射率
float moonDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};   // 月亮的反射率
//光源位置
//行星的半径
GLfloat earthRadius = 0.7;
GLfloat moonRadius = 0.1;
GLfloat sunRadius = 1.8;
GLfloat goldRadius = 0.5;
GLfloat waterRadius = 0.23;
GLfloat fireRadius = 0.35;
// 行星的自转速度
GLfloat earthSelfRotationSpeed = 2.0;
GLfloat moonSelfRotationSpeed = 1.0;
GLfloat sunSelfRotationSpeed = 0.2;
GLfloat goldSelfRotationSpeed = 2.2;
GLfloat waterSelfRotationSpeed = 3.0;
GLfloat fireSelfRotationSpeed = 1.8;
// 行星的公转速度
GLfloat earthOrbitSpeed = 0.5;
GLfloat moonOrbitSpeed = 7.0;
GLfloat goldOrbitSpeed = 0.9;
GLfloat waterOrbitSpeed = 2.0;
GLfloat fireOrbitSpeed = 0.25;
// 行星公转半径
GLfloat earthOrbitRadius = 8.0;
GLfloat moonOrbitRadius = 1.3;
GLfloat sunOrbitRadius = 0.0;
GLfloat goldOrbitRadius = 5.0;
GLfloat waterOrbitRadius = 2.5;
GLfloat fireOrbitRadius = 12.0;
// 行星当前公转角度
GLfloat earthOrbitAngle = 0.0;
GLfloat moonOrbitAngle = 0.0;
GLfloat sunOrbitAngle = 0.0;
GLfloat goldOrbitAngle = 0.0;
GLfloat waterOrbitAngle = 0.0;
GLfloat fireOrbitAngle = 0.0;
GLfloat Angle = 0.0;
//纹理贴图对象
GLuint earthTexture;
GLuint moonTexture;
GLuint sunTexture;
GLuint goldTexture;
GLuint waterTexture;
GLuint fireTexture;
GLuint background;
static int longitude = 200, latitude=200;
static GLfloat xequalzero[] = {0.0, 1.0, 0.0, 0.0};
static GLfloat yequalzero[] = {1.0, 0.0, 0.0, 0.0};
static GLfloat *currentCoeff;
static GLenum currentPlane;
static GLint currentGenMode;
float zoom = 300.f;
int win_width  = 1920;
int win_height = 1080;
int flag=1;
#endif // VARIABLE_H_INCLUDED
