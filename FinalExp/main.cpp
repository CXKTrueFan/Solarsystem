#include <windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include<stdio.h>
#include<variable.h>
GLubyte*  readImg(char* filename, int * imagewidth, int * imageheight)  //读取纹理bmp文件
{
     //打开文件
      int pixellength;
      GLubyte *  pixeldata ;
      FILE* pfile=fopen(filename,"rb");
      if(pfile == 0) exit(0);
    //读取图像大小
       fseek(pfile,0x0012,SEEK_SET);
       fread(imagewidth,sizeof(*imagewidth),1,pfile);
       fread(imageheight,sizeof(*imageheight),1,pfile);
    //计算像素数据长度
       pixellength=(*imagewidth)*3;
       while(pixellength%4 != 0)pixellength++;
       pixellength *= (*imageheight);
     //读取像素数据
       pixeldata = (GLubyte*)malloc(pixellength);
       if(pixeldata == 0) exit(0);
       fseek(pfile,54,SEEK_SET);
       fread(pixeldata,pixellength,1,pfile);
        //关闭文件
       fclose(pfile);
       return pixeldata;
  }
void LoadTexture(char * filename, GLuint &texture)    //加载纹理
{
	GLubyte * data;
	GLint width, height;
	FILE * file;
    data=readImg(filename, &width, &height );//读文件
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);//绑定纹理
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//指定纹理贴图与材质的混合模式
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //线性滤图
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    currentCoeff = xequalzero;
    currentGenMode = GL_OBJECT_LINEAR;
    currentPlane = GL_OBJECT_PLANE;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	glEnable(GL_TEXTURE_2D);//打开纹理映射
	free(data); //释放纹理图像数据，纹理数据已由上一句生成并保存到纹理缓存中，使用完毕后应用glDeleteTextures释放纹理缓存
}
// 初始化函数
void init()
{   //跟据纹理文件的具体位置进行更改
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\earth_texture.bmp",earthTexture);//创建并绑定地球纹理对象
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\moon_texture.bmp",moonTexture);//创建并绑定月球纹理对象
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\sun_texture.bmp",sunTexture);//创建并绑定太阳纹理对象
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\water_texture.bmp",waterTexture);//创建并绑定水星纹理对象
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\gold_texture.bmp",goldTexture);//创建并绑定金星纹理对象
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\fire_texture.bmp",fireTexture);//创建并绑定火星纹理对象
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\background.bmp",background);//创建并绑定背景纹理对象
    // 设置清屏色
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void drawbackground()
{
    glDisable(GL_DEPTH_TEST);   //关闭深度测试;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, background);       //绑定背景纹理
    glBegin(GL_QUADS);      //将图片四个角的位置设置为正交窗口后裁剪面的四个角；
    glTexCoord2d(0.0, 0.0); glVertex3d(-zoom * win_width / win_height, -zoom, -300);
    glTexCoord2d(1.0, 0.0); glVertex3d(zoom * win_width / win_height, -zoom, -300);
    glTexCoord2d(1.0, 1.0); glVertex3d(zoom * win_width / win_height, zoom, -300);
    glTexCoord2d(0.0, 1.0); glVertex3d(-zoom * win_width / win_height, zoom, -300);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);   //开启深度测试；
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)win_width/win_height, 0.01, 3000);
}
void drawsun(GLUquadricObj*  qobj)    //渲染太阳
{
  glEnable(GL_LIGHTING); // 启用光照
  GLfloat sun_emission[] = {1.0, 1.0, 0.0, 1.0}; // 发光颜色为黄色
  glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission); // 设置材质属性
  glEnable(GL_LIGHT0);    // 启用光源0
  GLfloat position[] = {10.0, 10.0, 10.0, 0.0}; // 光源在 (10, 10, 10) 位置
  glLightfv(GL_LIGHT0, GL_POSITION, position); // 设置光源属性
  glEnable(GL_TEXTURE_2D); //启用纹理
  glRotatef(sunSelfRotationSpeed * sunOrbitAngle, 0.0, 1.0, 0.0); // 沿 Y 轴自转
  glPushMatrix();
  glColor3f(1.0, 1.0, 0.0); // 太阳颜色为黄色
  glRotatef(sunOrbitAngle, 0.0, 1.0, 0.0);
  glTranslatef(sunOrbitRadius, 0.0, 0.0);
  glBindTexture(GL_TEXTURE_2D,sunTexture);
  gluQuadricTexture(qobj, GL_TRUE);
  gluSphere(qobj, sunRadius, longitude, latitude);
  glPopMatrix();
}
void drawplanet(GLUquadricObj*  qobj)
{
  glEnable(GL_DEPTH_TEST);
    // 渲染水星
  glPushMatrix();
  glRotatef(waterOrbitAngle, 0.0, 1.0, 0.0);
  glTranslatef(waterOrbitRadius, 0.0, 0.0);
  glRotatef(waterOrbitAngle, 0.0, 1.0, 0.0); // 沿 Y 轴旋转，不需要乘公转角速度
  glTranslatef(0.0, 0.0, -sunRadius); // 移动到正确的位置
  glRotatef(waterSelfRotationSpeed * waterOrbitAngle, 0.0, 1.0, 0.0); // 沿 Y 轴自转
  glBindTexture(GL_TEXTURE_2D, waterTexture);
  gluQuadricTexture(qobj, GL_TRUE);
  gluSphere(qobj, waterRadius, longitude, latitude);//二次曲面qobj
  glPopMatrix();

// 渲染金星
  glPushMatrix();
  glRotatef(goldOrbitAngle, 0.0, 1.0, 0.0);
  glTranslatef(goldOrbitRadius, 0.0, 0.0);
  glRotatef(goldOrbitAngle, 0.0, 1.0, 0.0); // 沿 Y 轴旋转，不需要乘公转角速度
  glTranslatef(0.0, 0.0, -sunRadius); // 移动到正确的位置
  glRotatef(goldSelfRotationSpeed * goldOrbitAngle, 0.0, 1.0, 0.0); // 沿 Y 轴自转
  glBindTexture(GL_TEXTURE_2D, goldTexture);
  gluQuadricTexture(qobj, GL_TRUE);
  gluSphere(qobj, goldRadius, longitude, latitude);//二次曲面qobj
  glPopMatrix();

// 渲染地球
  glPushMatrix();
  glRotatef(earthOrbitAngle, 0.0, 1.0, 0.0);
  glTranslatef(earthOrbitRadius, 0.0, 0.0);
  glRotatef(earthOrbitAngle, 0.0, 1.0, 0.0); // 沿 Y 轴旋转，不需要乘公转角速度
  glTranslatef(0.0, 0.0, -sunRadius); // 移动到正确的位置
  glRotatef(earthSelfRotationSpeed * Angle, 0.0, 1.0, 0.0); // 自转
  glBindTexture(GL_TEXTURE_2D, earthTexture);
  gluQuadricTexture(qobj, GL_TRUE);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, earthDiffuse);
  gluSphere(qobj, earthRadius, longitude, latitude);//二次曲面qobj

// 渲染月球
    glPushMatrix();
    glRotatef(moonOrbitAngle, 0.0, 1.0, 0.0);
    glTranslatef(moonOrbitRadius, 0.0, 0.0);
    glRotatef(moonSelfRotationSpeed * moonOrbitAngle, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D,moonTexture);
    gluQuadricTexture(qobj, GL_TRUE);
    gluSphere(qobj, moonRadius, longitude,latitude);
    glPopMatrix();

// 渲染火星
    glPushMatrix();
    glRotatef(fireOrbitAngle, 0.0, 1.0, 0.0);
    glTranslatef(fireOrbitRadius, 0.0, 0.0);
    glRotatef(fireOrbitAngle, 0.0, 1.0, 0.0); // 沿 Y 轴旋转，不需要乘公转角速度
    glTranslatef(0.0, 0.0, -sunRadius);     // 移动到正确的位置
    glRotatef(fireSelfRotationSpeed * fireOrbitAngle, 0.0, 1.0, 0.0); // 沿 Y 轴自转
    glBindTexture(GL_TEXTURE_2D, fireTexture);
    gluQuadricTexture(qobj, GL_TRUE);
    gluSphere(qobj, fireRadius, longitude, latitude);
    glPopMatrix();

}

void display()
{
    GLUquadricObj*  qobj;
    qobj = gluNewQuadric();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //清除缓冲
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-zoom* win_width / win_height, zoom* win_width / win_height, -zoom, zoom, -300.0, 300.0);   //切换为正交视角；
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawbackground();
    switch(flag)  //切换观察视角
   {
    case(1):
    gluLookAt(20.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0);    //观察视角
    break;
    case(-1):
    gluLookAt(0.0f, 0.0f, 20.0f,0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);  //换个方向
    break;
   }
    drawsun(qobj);      //绘制太阳
    drawplanet(qobj);   //绘制其他行星
    glutSwapBuffers();  //刷新缓冲区
}

void reshape(int width, int height)   // 窗口大小变化回调函数
{
   glViewport(0, 0, width, height);   // 设置视口大小
   glMatrixMode(GL_PROJECTION);  // 设置投影矩阵
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
   glMatrixMode(GL_MODELVIEW);  // 切换到模型视图矩阵
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'e':       //地球自转
      case 'E':
         Angle += (earthSelfRotationSpeed-0.25); //增加自转的角度，逆时针旋转
         glutPostRedisplay();
         break;
      case 's':        //地球公转
      case 'S':
         earthOrbitAngle += earthOrbitSpeed;
         moonOrbitAngle += moonOrbitSpeed;   //减少自转的角度，顺时针旋转
         glutPostRedisplay();
         break;
      case 'y':        //月球公转
      case 'Y':
         moonOrbitAngle += (moonOrbitSpeed-0.7);
         glutPostRedisplay();
        break;
      case 'r':           //整个系统同步运行
      case 'R':
         earthOrbitAngle += earthOrbitSpeed;
         Angle=earthOrbitAngle;
         moonOrbitAngle += moonOrbitSpeed;
         sunOrbitAngle += sunSelfRotationSpeed;  //增加公转的角度，逆时针旋转
         waterOrbitAngle += waterSelfRotationSpeed/3;
         goldOrbitAngle += goldSelfRotationSpeed/3;
         fireOrbitAngle += fireSelfRotationSpeed/5;
         glutPostRedisplay();
         break;
	  case 'l':
		 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //使用线框模型显示物体
		 glDisable(GL_LIGHTING);
         glDisable(GL_TEXTURE_2D);
		 longitude = 20;
	     latitude=16;
		 glutPostRedisplay();
		 break;
	  case 'f':
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //使用多边形填充模型型显示物体
		 longitude = 200;
	     latitude=200;
		 glutPostRedisplay();
		 break;
      case'v':         //切换视角
        flag=-flag;
        glutPostRedisplay();
        break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}
// 主函数
int main(int argc, char **argv)
{
  // 初始化 GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(win_width, win_height);
  glutCreateWindow("吴涛 20211120234 Solar System");
  init();    // 初始化
  glutDisplayFunc(display);   //绘图
  glutReshapeFunc(reshape);   //窗口改变回调
  glutKeyboardFunc(keyboard);  //键盘回调
  glutMainLoop();   // 进入循环
  return 0;
}
