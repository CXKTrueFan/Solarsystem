#include <windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include<stdio.h>
#include<variable.h>
GLubyte*  readImg(char* filename, int * imagewidth, int * imageheight)  //��ȡ����bmp�ļ�
{
     //���ļ�
      int pixellength;
      GLubyte *  pixeldata ;
      FILE* pfile=fopen(filename,"rb");
      if(pfile == 0) exit(0);
    //��ȡͼ���С
       fseek(pfile,0x0012,SEEK_SET);
       fread(imagewidth,sizeof(*imagewidth),1,pfile);
       fread(imageheight,sizeof(*imageheight),1,pfile);
    //�����������ݳ���
       pixellength=(*imagewidth)*3;
       while(pixellength%4 != 0)pixellength++;
       pixellength *= (*imageheight);
     //��ȡ��������
       pixeldata = (GLubyte*)malloc(pixellength);
       if(pixeldata == 0) exit(0);
       fseek(pfile,54,SEEK_SET);
       fread(pixeldata,pixellength,1,pfile);
        //�ر��ļ�
       fclose(pfile);
       return pixeldata;
  }
void LoadTexture(char * filename, GLuint &texture)    //��������
{
	GLubyte * data;
	GLint width, height;
	FILE * file;
    data=readImg(filename, &width, &height );//���ļ�
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);//������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//ָ��������ͼ����ʵĻ��ģʽ
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //������ͼ
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    currentCoeff = xequalzero;
    currentGenMode = GL_OBJECT_LINEAR;
    currentPlane = GL_OBJECT_PLANE;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	glEnable(GL_TEXTURE_2D);//������ӳ��
	free(data); //�ͷ�����ͼ�����ݣ���������������һ�����ɲ����浽�������У�ʹ����Ϻ�Ӧ��glDeleteTextures�ͷ�������
}
// ��ʼ������
void init()
{   //���������ļ��ľ���λ�ý��и���
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\earth_texture.bmp",earthTexture);//�������󶨵����������
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\moon_texture.bmp",moonTexture);//�������������������
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\sun_texture.bmp",sunTexture);//��������̫���������
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\water_texture.bmp",waterTexture);//��������ˮ���������
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\gold_texture.bmp",goldTexture);//�������󶨽����������
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\fire_texture.bmp",fireTexture);//�������󶨻����������
    LoadTexture("D:\\JISUANJITUXINXUESHIYAN\\FinalWork\\background.bmp",background);//�������󶨱����������
    // ��������ɫ
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void drawbackground()
{
    glDisable(GL_DEPTH_TEST);   //�ر���Ȳ���;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, background);       //�󶨱�������
    glBegin(GL_QUADS);      //��ͼƬ�ĸ��ǵ�λ������Ϊ�������ں�ü�����ĸ��ǣ�
    glTexCoord2d(0.0, 0.0); glVertex3d(-zoom * win_width / win_height, -zoom, -300);
    glTexCoord2d(1.0, 0.0); glVertex3d(zoom * win_width / win_height, -zoom, -300);
    glTexCoord2d(1.0, 1.0); glVertex3d(zoom * win_width / win_height, zoom, -300);
    glTexCoord2d(0.0, 1.0); glVertex3d(-zoom * win_width / win_height, zoom, -300);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);   //������Ȳ��ԣ�
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)win_width/win_height, 0.01, 3000);
}
void drawsun(GLUquadricObj*  qobj)    //��Ⱦ̫��
{
  glEnable(GL_LIGHTING); // ���ù���
  GLfloat sun_emission[] = {1.0, 1.0, 0.0, 1.0}; // ������ɫΪ��ɫ
  glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission); // ���ò�������
  glEnable(GL_LIGHT0);    // ���ù�Դ0
  GLfloat position[] = {10.0, 10.0, 10.0, 0.0}; // ��Դ�� (10, 10, 10) λ��
  glLightfv(GL_LIGHT0, GL_POSITION, position); // ���ù�Դ����
  glEnable(GL_TEXTURE_2D); //��������
  glRotatef(sunSelfRotationSpeed * sunOrbitAngle, 0.0, 1.0, 0.0); // �� Y ����ת
  glPushMatrix();
  glColor3f(1.0, 1.0, 0.0); // ̫����ɫΪ��ɫ
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
    // ��Ⱦˮ��
  glPushMatrix();
  glRotatef(waterOrbitAngle, 0.0, 1.0, 0.0);
  glTranslatef(waterOrbitRadius, 0.0, 0.0);
  glRotatef(waterOrbitAngle, 0.0, 1.0, 0.0); // �� Y ����ת������Ҫ�˹�ת���ٶ�
  glTranslatef(0.0, 0.0, -sunRadius); // �ƶ�����ȷ��λ��
  glRotatef(waterSelfRotationSpeed * waterOrbitAngle, 0.0, 1.0, 0.0); // �� Y ����ת
  glBindTexture(GL_TEXTURE_2D, waterTexture);
  gluQuadricTexture(qobj, GL_TRUE);
  gluSphere(qobj, waterRadius, longitude, latitude);//��������qobj
  glPopMatrix();

// ��Ⱦ����
  glPushMatrix();
  glRotatef(goldOrbitAngle, 0.0, 1.0, 0.0);
  glTranslatef(goldOrbitRadius, 0.0, 0.0);
  glRotatef(goldOrbitAngle, 0.0, 1.0, 0.0); // �� Y ����ת������Ҫ�˹�ת���ٶ�
  glTranslatef(0.0, 0.0, -sunRadius); // �ƶ�����ȷ��λ��
  glRotatef(goldSelfRotationSpeed * goldOrbitAngle, 0.0, 1.0, 0.0); // �� Y ����ת
  glBindTexture(GL_TEXTURE_2D, goldTexture);
  gluQuadricTexture(qobj, GL_TRUE);
  gluSphere(qobj, goldRadius, longitude, latitude);//��������qobj
  glPopMatrix();

// ��Ⱦ����
  glPushMatrix();
  glRotatef(earthOrbitAngle, 0.0, 1.0, 0.0);
  glTranslatef(earthOrbitRadius, 0.0, 0.0);
  glRotatef(earthOrbitAngle, 0.0, 1.0, 0.0); // �� Y ����ת������Ҫ�˹�ת���ٶ�
  glTranslatef(0.0, 0.0, -sunRadius); // �ƶ�����ȷ��λ��
  glRotatef(earthSelfRotationSpeed * Angle, 0.0, 1.0, 0.0); // ��ת
  glBindTexture(GL_TEXTURE_2D, earthTexture);
  gluQuadricTexture(qobj, GL_TRUE);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, earthDiffuse);
  gluSphere(qobj, earthRadius, longitude, latitude);//��������qobj

// ��Ⱦ����
    glPushMatrix();
    glRotatef(moonOrbitAngle, 0.0, 1.0, 0.0);
    glTranslatef(moonOrbitRadius, 0.0, 0.0);
    glRotatef(moonSelfRotationSpeed * moonOrbitAngle, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D,moonTexture);
    gluQuadricTexture(qobj, GL_TRUE);
    gluSphere(qobj, moonRadius, longitude,latitude);
    glPopMatrix();

// ��Ⱦ����
    glPushMatrix();
    glRotatef(fireOrbitAngle, 0.0, 1.0, 0.0);
    glTranslatef(fireOrbitRadius, 0.0, 0.0);
    glRotatef(fireOrbitAngle, 0.0, 1.0, 0.0); // �� Y ����ת������Ҫ�˹�ת���ٶ�
    glTranslatef(0.0, 0.0, -sunRadius);     // �ƶ�����ȷ��λ��
    glRotatef(fireSelfRotationSpeed * fireOrbitAngle, 0.0, 1.0, 0.0); // �� Y ����ת
    glBindTexture(GL_TEXTURE_2D, fireTexture);
    gluQuadricTexture(qobj, GL_TRUE);
    gluSphere(qobj, fireRadius, longitude, latitude);
    glPopMatrix();

}

void display()
{
    GLUquadricObj*  qobj;
    qobj = gluNewQuadric();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //�������
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-zoom* win_width / win_height, zoom* win_width / win_height, -zoom, zoom, -300.0, 300.0);   //�л�Ϊ�����ӽǣ�
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawbackground();
    switch(flag)  //�л��۲��ӽ�
   {
    case(1):
    gluLookAt(20.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0);    //�۲��ӽ�
    break;
    case(-1):
    gluLookAt(0.0f, 0.0f, 20.0f,0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);  //��������
    break;
   }
    drawsun(qobj);      //����̫��
    drawplanet(qobj);   //������������
    glutSwapBuffers();  //ˢ�»�����
}

void reshape(int width, int height)   // ���ڴ�С�仯�ص�����
{
   glViewport(0, 0, width, height);   // �����ӿڴ�С
   glMatrixMode(GL_PROJECTION);  // ����ͶӰ����
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
   glMatrixMode(GL_MODELVIEW);  // �л���ģ����ͼ����
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'e':       //������ת
      case 'E':
         Angle += (earthSelfRotationSpeed-0.25); //������ת�ĽǶȣ���ʱ����ת
         glutPostRedisplay();
         break;
      case 's':        //����ת
      case 'S':
         earthOrbitAngle += earthOrbitSpeed;
         moonOrbitAngle += moonOrbitSpeed;   //������ת�ĽǶȣ�˳ʱ����ת
         glutPostRedisplay();
         break;
      case 'y':        //����ת
      case 'Y':
         moonOrbitAngle += (moonOrbitSpeed-0.7);
         glutPostRedisplay();
        break;
      case 'r':           //����ϵͳͬ������
      case 'R':
         earthOrbitAngle += earthOrbitSpeed;
         Angle=earthOrbitAngle;
         moonOrbitAngle += moonOrbitSpeed;
         sunOrbitAngle += sunSelfRotationSpeed;  //���ӹ�ת�ĽǶȣ���ʱ����ת
         waterOrbitAngle += waterSelfRotationSpeed/3;
         goldOrbitAngle += goldSelfRotationSpeed/3;
         fireOrbitAngle += fireSelfRotationSpeed/5;
         glutPostRedisplay();
         break;
	  case 'l':
		 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //ʹ���߿�ģ����ʾ����
		 glDisable(GL_LIGHTING);
         glDisable(GL_TEXTURE_2D);
		 longitude = 20;
	     latitude=16;
		 glutPostRedisplay();
		 break;
	  case 'f':
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //ʹ�ö�������ģ������ʾ����
		 longitude = 200;
	     latitude=200;
		 glutPostRedisplay();
		 break;
      case'v':         //�л��ӽ�
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
// ������
int main(int argc, char **argv)
{
  // ��ʼ�� GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(win_width, win_height);
  glutCreateWindow("���� 20211120234 Solar System");
  init();    // ��ʼ��
  glutDisplayFunc(display);   //��ͼ
  glutReshapeFunc(reshape);   //���ڸı�ص�
  glutKeyboardFunc(keyboard);  //���̻ص�
  glutMainLoop();   // ����ѭ��
  return 0;
}
