//
//  main.cpp
//  animation
//
//  Created by Hindupur Kedar on 5/17/15.
//  Copyright (c) 2015 Hindupur Kedar. All rights reserved.
//
//
//  main.cpp
//  let us try
//
//  Created by Hindupur Kedar on 10/24/14.
//  Copyright (c) 2014 Hindupur Kedar. All rights reserved.
//

#include <OpenGL/gl.h>//
#include <OpenGL/glu.h>//
#include <GLUT/glut.h>


#include <stdlib.h>
#include <GLUT/glut.h>
#include<math.h>
#include<stdio.h>

//defining the bot parts
#define torso1 0;
#define head1 1;
#define lupa 2;
#define lela 3;
#define rupa 4;
#define rila 5;
#define leul 6;
#define lell 7;
#define riul 8;
#define rill 9;
#define ball1 10;


#define xdir 1;
#define ydir 2;
#define zdir 3;

#define trans 1;
#define rot 2;

#define maxevents 1000        // For Playback Function
#define recordsize 4
#define TORSO_HEIGHT 5.0     // For Robot
#define UPPER_ARM_HEIGHT 3.0
#define LOWER_ARM_HEIGHT 2.0
#define UPPER_LEG_RADIUS  0.5
#define LOWER_LEG_RADIUS  0.5
#define LOWER_LEG_HEIGHT 2.0
#define UPPER_LEG_HEIGHT 3.0
#define UPPER_LEG_RADIUS  0.5
#define TORSO_RADIUS 1.0
#define UPPER_ARM_RADIUS  0.5
#define LOWER_ARM_RADIUS  0.5
#define HEAD_HEIGHT 1.5
#define HEAD_RADIUS 1.0
#define pi 3.14159265

GLfloat xpos = 0.0;         // For Translation & Rotation of Robot
GLfloat ypos = 0.0;
GLfloat zpos = 0.0;
GLfloat radius = 1;        // For Football
GLfloat x1 = 2.7;
GLfloat y2 = -4;
GLfloat angle1;

typedef float point[3];

// tree node typedef
typedef struct treenode
{
    GLfloat m[16];
    void (*f)();
    struct treenode *sibling;
    struct treenode *child;
}treenode;

typedef treenode* t_ptr;

static GLfloat theta[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    180.0, 0.0, 180.0, 0.0 }; /* initial joint angles */
static GLint angle = 2;

GLUquadricObj *t, *h, *lua, *lla, *rua, *rla, *lll, *rll, *rul, *lul;

double size = 1.0;
int event_ptr = 0;
int playback_ptr = 0;
int recordmode = 0;
int playbackmode = 0;

GLfloat ball = 6.0;

FILE *jFile = NULL;
char *fileName = "khindupur.txt";

int event_buffer[maxevents*recordsize];



treenode torso_node, head_node, lua_node, rua_node, lll_node, rll_node,
lla_node, rla_node, rul_node, lul_node;


// traversing the tree



void traverse(treenode* root)
{
    if(root==NULL) return;
    glPushMatrix();
    glMultMatrixf(root->m);
    root->f();
    if(root->child!=NULL) traverse(root->child);
    glPopMatrix();
    if(root->sibling!=NULL) traverse(root->sibling);
}





// Timer Function for playback
void timerFunc(int val)
{
    int id, value, dir, tr;
    
    if (playback_ptr < event_ptr)
    {
        id = event_buffer[playback_ptr++];
        value = event_buffer[playback_ptr++];
        dir = event_buffer[playback_ptr++];
        tr = event_buffer[playback_ptr++];
        
        switch (id){
                
                //Torso
            case 0:
                
                if (tr == 1)
                {
                    if (dir == 1){
                        
                        xpos = value;
                        
                        if (xpos > 10)
                            xpos = 10;
                        
                        if (xpos < -10)
                            xpos = -10;
                        
                    }
                    
                    else if (dir == 3)
                    {
                        
                        zpos = value;
                        
                        if (zpos > 10)
                            zpos = 10;
                        
                        if (zpos < -10)
                            zpos = -10;
                    }
                    
                    else if (dir == 2)
                    {
                        ypos = value;
                    }
                    
                }
                else
                {
                    
                    theta[id] = value;
                    
                }
                
                break;
                
                //Head
            case 1:
                
                theta[id] = value;
                break;
                
                //Left Uper Arm
            case 2:
                theta[id] = value;
                break;
                
                //Left Lower Arm
            case 3:
                theta[id] = value;
                break;
                
                //Right Uper Arm
            case 4:
                theta[id] = value;
                break;
                
                //Right Lower Arm
            case 5:
                theta[id] = value;
                break;
                
                //Left Uper Leg
            case 6:
                theta[id] = value;
                break;
                
                //Left Lower Leg
            case 7:
                theta[id] = value;
                break;
                
                //Right Uper Leg
            case 8:
                theta[id] = value;
                break;
                
                //Right Lower Leg
            case 9:
                theta[id] = value;
                break;
                
                //Ball
            case 10:
                x1 = value;
                break;
                
                
                
                
                
        }
        
        glutPostRedisplay();
        glutTimerFunc(50, timerFunc, 1);
    }
    
    else{
        playback_ptr = 0;
        printf("\nPlayback has ended");
        
    }
}

//Drawing the Robot
void torso()
{
    glPushMatrix();
    glColor3f(1, 1, 1);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(t, TORSO_RADIUS, TORSO_RADIUS, TORSO_HEIGHT, 10, 10);
    
    glPopMatrix();
}

void head()
{
    glPushMatrix();
    glTranslatef(0.0, 0.5*HEAD_HEIGHT, 0.0);
    glScalef(HEAD_RADIUS, HEAD_HEIGHT, HEAD_RADIUS);
    gluSphere(h, 1.0, 10, 10);
    glPopMatrix();
}

void left_upper_arm()
{
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(lua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_HEIGHT, 10, 10);
    glPopMatrix();
}

void left_lower_arm()
{
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(lla, LOWER_ARM_RADIUS, LOWER_ARM_RADIUS, LOWER_ARM_HEIGHT, 10, 10);
    glPopMatrix();
}

void right_upper_arm()
{
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(rua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_HEIGHT, 10, 10);
    glPopMatrix();
}

void right_lower_arm()
{
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(rla, LOWER_ARM_RADIUS, LOWER_ARM_RADIUS, LOWER_ARM_HEIGHT, 10, 10);
    glPopMatrix();
}

void left_upper_leg()
{
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(lul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
    glPopMatrix();
}

void left_lower_leg()
{
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(lll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
    glPopMatrix();
}

void right_upper_leg()
{
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(rul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 10, 10);
    glPopMatrix();
}

void right_lower_leg()
{
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(rll, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 10, 10);
    glPopMatrix();
}



void drawObjects(GLenum mode)
{
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    //Drawing goalpost
    GLfloat mat_diffuse2[] = { 1.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
    glBegin(GL_LINES);
    glVertex2f(13.5, 5.5);
    glVertex2f(13.5, -5.5);
    glVertex2f(9, 5.5);
    glVertex2f(13.5, 5.5);
    glVertex2f(9, -5.5);
    glVertex2f(13.5, -5.5);
    glVertex2f(13, 5.5);
    glVertex2f(13, -5.5);
    glVertex2f(12.5, 5.5);
    glVertex2f(12.5, -5.5);
    
    glEnd();
    
    
    GLfloat mat_diffuse3[] = { 1.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);
    glBegin(GL_LINES);
    glVertex2f(-13.5, 5.5);
    glVertex2f(-13.5, -5.5);
    glVertex2f(-9, 5.5);
    glVertex2f(-13.5, 5.5);
    glVertex2f(-9, -5.5);
    glVertex2f(-13.5, -5.5);
    glVertex2f(-13, 5.5);
    glVertex2f(-13, -5.5);
    glVertex2f(-12.5, 5.5);
    glVertex2f(-12.5, -5.5);
    
    glEnd();
    
    // lighting
    
    GLfloat mat_diffuse[] = { 1.0, 1.0, 0.0, 0.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess = { 100.0 };
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    
    if (mode == GL_SELECT) glLoadName(1);
    glTranslatef(xpos, ypos, zpos);
    glRotatef(theta[0], 0.0, 1.0, 0.0);
    glColor3d(1, 0, 0);
    torso();
    glPushMatrix();
    
    if (mode == GL_SELECT) glLoadName(2);
    glTranslatef(0.0, TORSO_HEIGHT + 0.5*HEAD_HEIGHT, 0.0);
    glRotatef(theta[1], 1.0, 0.0, 0.0);
    
    glTranslatef(0.0, -0.5*HEAD_HEIGHT, 0.0);
    head();
    
    glPopMatrix();
    if (mode == GL_SELECT) glLoadName(3);
    glPushMatrix();
    glTranslatef(-(TORSO_RADIUS + UPPER_ARM_RADIUS), 0.9*TORSO_HEIGHT, 0.0);
    glRotatef(theta[2], 1.0, 0.0, 0.0);
    left_upper_arm();
    
    if (mode == GL_SELECT) glLoadName(4);
    glTranslatef(0.0, UPPER_ARM_HEIGHT, 0.0);
    glRotatef(theta[3], 1.0, 0.0, 0.0);
    left_lower_arm();
    
    glPopMatrix();
    if (mode == GL_SELECT) glLoadName(5);
    glPushMatrix();
    glTranslatef(TORSO_RADIUS + UPPER_ARM_RADIUS, 0.9*TORSO_HEIGHT, 0.0);
    glRotatef(theta[4], 1.0, 0.0, 0.0);
    right_upper_arm();
    
    if (mode == GL_SELECT) glLoadName(6);
    glTranslatef(0.0, UPPER_ARM_HEIGHT, 0.0);
    glRotatef(theta[5], 1.0, 0.0, 0.0);
    right_lower_arm()
    ;
    
    glPopMatrix();
    if (mode == GL_SELECT) glLoadName(7);
    glPushMatrix();
    glTranslatef(-(TORSO_RADIUS + UPPER_LEG_RADIUS), 0.1*UPPER_LEG_HEIGHT, 0.0);
    glRotatef(theta[6], 1.0, 0.0, 0.0);
    left_upper_leg();
    
    if (mode == GL_SELECT) glLoadName(8);
    glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
    glRotatef(theta[7], 1.0, 0.0, 0.0);
    left_lower_leg();
    
    glPopMatrix();
    if (mode == GL_SELECT) glLoadName(9);
    glPushMatrix();
    glTranslatef(TORSO_RADIUS + UPPER_LEG_RADIUS, 0.1*UPPER_LEG_HEIGHT, 0.0);
    glRotatef(theta[8], 1.0, 0.0, 0.0);
    right_upper_leg();
    
    if (mode == GL_SELECT) glLoadName(10);
    glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
    glRotatef(theta[9], 1.0, 0.0, 0.0);
    right_lower_leg();
    
    glPopMatrix();
    
    
    
    //Drawing the football and its lighting
    GLfloat mat_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
    GLfloat mat_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
    
    GLfloat mat_ambient1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess1 = { 100.0 };
    GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position1[] = { 10.0, 10.0, 10.0, 0.0 };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular1);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess1);
    
    glTranslatef(x1, y2, 0.0);
    glutSolidSphere(1.25, 50, 50);
    
    
    
    glFlush();
    glutSwapBuffers();
}

//Display Function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawObjects(GL_RENDER);
    glFlush();
}

//For translation and rotation of robot
void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            xpos -= cos(90 * pi / 180 + theta[0] * pi / 180);
            zpos += sin(90 * pi / 180 + theta[0] * pi / 180);
            if (xpos > 10)
                xpos = 10;
            if (zpos > 10)
                zpos = 10;
            if (xpos < -10)
                xpos = -10;
            if (zpos < -10)
                zpos = -10;
            ball += 0.000005;
            if (recordmode == 1){
                event_buffer[event_ptr++] = torso1;
                event_buffer[event_ptr++] = xpos;
                event_buffer[event_ptr++] = xdir;
                event_buffer[event_ptr++] = trans;
                
                event_buffer[event_ptr++] = torso1;
                event_buffer[event_ptr++] = zpos;
                event_buffer[event_ptr++] = zdir;
                event_buffer[event_ptr++] = trans;
            }
            break;
        case GLUT_KEY_LEFT:
            xpos += cos(90 * pi / 180 + theta[0] * pi / 180);
            zpos -= sin(90 * pi / 180 + theta[0] * pi / 180);
            if (xpos > 10)
                xpos = 10;
            if (zpos > 10)
                zpos = 10;
            if (xpos < -10)
                xpos = -10;
            if (zpos < -10)
                zpos = -10;
            ball -= 0.0000005;
            if (recordmode == 1){
                event_buffer[event_ptr++] = torso1;
                event_buffer[event_ptr++] = xpos;
                event_buffer[event_ptr++] = xdir;
                event_buffer[event_ptr++] = trans;
                
                event_buffer[event_ptr++] = torso1;
                event_buffer[event_ptr++] = zpos;
                event_buffer[event_ptr++] = zdir;
                event_buffer[event_ptr++] = trans;
            }
            break;
            
        case GLUT_KEY_UP: ypos += 1.0;
            if (recordmode == 1){
                
                
                event_buffer[event_ptr++] = torso1;
                event_buffer[event_ptr++] = ypos;
                event_buffer[event_ptr++] = ydir;
                event_buffer[event_ptr++] = trans;
            }
            break;
        case GLUT_KEY_DOWN: ypos -= 1.0;
            if (recordmode == 1){
                
                
                event_buffer[event_ptr++] = torso1;
                event_buffer[event_ptr++] = ypos;
                event_buffer[event_ptr++] = ydir;
                event_buffer[event_ptr++] = trans;
                
            }
            break;
    }
    glutPostRedisplay();
}

// For Picking
void processHits(GLint hits, GLuint buffer[])
{
    unsigned int i, j;
    GLuint ii, jj, names, *ptr;
    
    printf("hits = %d\n", hits);
    ptr = (GLuint *)buffer;
    for (i = 0; i < hits; i++) {
        names = *ptr;
        ptr += 3;
        for (j = 0; j < names; j++) {
            if (*ptr == 1){
                printf("torso\n");
                glClearColor(0, 0, 0, 1);
                glColor3f(1.0, 0.0, 0.0);
                angle = 0;
            }
            else if (*ptr == 2){
                printf("head\n");
                
                glClearColor(0, 0, 1, 1);
                glColor3f(1.0, 0.0, 0.0);
                angle = 0;
                angle = 1;
            }
            else if (*ptr == 3){
                printf("left upper arm\n");
                
                glClearColor(0, 1, 1, 1);
                glColor3f(1.0, 0.0, 0.0);
                angle = 2;
            }
            else if (*ptr == 4){
                printf("left lower arm\n");
                glClearColor(1, 1, 1, 1);
                glColor3f(1.0, 0.0, 0.0);
                angle = 3;
            }
            
            else if (*ptr == 5){
                printf("right upper arm\n");
                glClearColor(1, 0, 1, 1);
                glColor3f(1.0, 0.0, 0.0);
                angle = 4;
            }
            else if (*ptr == 6){
                printf("right lower arm\n");
                glClearColor(1, 0, 0, 1);
                glColor3f(1.0, 0.0, 0.0);
                angle = 5;
            }
            else if (*ptr == 7){
                printf("left upper leg\n");
                glClearColor(0, 1, 0, 1);
                glColor3f(1.0, 0.0, 0.0);
                angle = 6;
            }
            else if (*ptr == 8){
                printf("left lower leg\n");
                angle = 7;
            }
            
            else if (*ptr == 9){
                printf("right upper leg\n");
                angle = 8;
            }
            
            else {
                printf("right lower leg\n");
                angle = 9;
            }
            
            ptr++;
        }
        printf("\n");
    }
}
#define SIZE 512

//Mouse Function for Rotation of Robot parts
void mouse(int btn, int state, int x, int y)
{
    GLuint selectBuf[SIZE];
    GLint hits;
    GLint viewport[11];
    
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        glSelectBuffer(SIZE, selectBuf);
        glRenderMode(GL_SELECT);
        
        glInitNames();
        glPushName(0);
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        /*  create 5x5 pixel picking region near cursor location	*/
        gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y),
                      5.0, 5.0, viewport);
        
        //gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
        GLfloat w, h;
        w = viewport[2];
        h = viewport[3];
        if (w <= h)
            glOrtho(-15.0, 15.0, -15.0 * (GLfloat)h / (GLfloat)w,
                    15.0 * (GLfloat)h / (GLfloat)w, -15.0, 15.0);
        else
            glOrtho(-15.0 * (GLfloat)w / (GLfloat)h,
                    15.0 * (GLfloat)w / (GLfloat)h, 0.0, 15.0, -15.0, 15.0);
        drawObjects(GL_SELECT);
        
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glFlush();
        
        hits = glRenderMode(GL_RENDER);
        processHits(hits, selectBuf);
        
        glutPostRedisplay();
        
        
    }
    
    
}

//Function to reshape window size
void
myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-15.0, 15.0, -15.0 * (GLfloat)h / (GLfloat)w,
                15.0 * (GLfloat)h / (GLfloat)w, -15.0, 15.0);
    else
        glOrtho(-15.0 * (GLfloat)w / (GLfloat)h,
                15.0 * (GLfloat)w / (GLfloat)h, 0.0, 15.0, -15.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard Function with keys for Playback
void keyboard(unsigned char key, int x, int y){
    
    switch (key){
            
            
        case 'q':
            theta[angle] += 5.0;
            if (theta[angle] > 360.0) theta[angle] -= 360.0;
            if (recordmode == 1)
            {
                event_buffer[event_ptr++] = angle;
                event_buffer[event_ptr++] = theta[angle];
                event_buffer[event_ptr++] = -1;
                event_buffer[event_ptr++] = rot;
            }
            display();
            break;
            
        case 'w':
            theta[angle] -= 5.0;
            if (theta[angle] < 360.0) theta[angle] += 360.0;
            if (recordmode == 1)
            {
                event_buffer[event_ptr++] = angle;
                event_buffer[event_ptr++] = theta[angle];
                event_buffer[event_ptr++] = -1;
                event_buffer[event_ptr++] = rot;
            }
            display();
            break;
            
        case 'k':
            x1 += ball;
            if (recordmode == 1)
            {
                event_buffer[event_ptr++] = ball1;
                event_buffer[event_ptr++] = x1;
                event_buffer[event_ptr++] = xdir;
                event_buffer[event_ptr++] = trans;
            }
            glutPostRedisplay();
            break;
            
        case 'j':
            x1 -= ball;
            x1 -= 4;
            if (recordmode == 1)
            {
                event_buffer[event_ptr++] = ball1;
                event_buffer[event_ptr++] = x1;
                event_buffer[event_ptr++] = xdir;
                event_buffer[event_ptr++] = trans;
            }
            
            glutPostRedisplay();
            break;
            
        case 'c':
            x1 -= 6;
            if (recordmode == 1)
            {
                event_buffer[event_ptr++] = ball1;
                event_buffer[event_ptr++] = x1;
                event_buffer[event_ptr++] = xdir;
                event_buffer[event_ptr++] = trans;
            }
            glutPostRedisplay();
            break;
            
        case 'x':
            x1 += 6;
            if (recordmode == 1)
            {
                event_buffer[event_ptr++] = ball1;
                event_buffer[event_ptr++] = x1;
                event_buffer[event_ptr++] = xdir;
                event_buffer[event_ptr++] = trans;
            }
            glutPostRedisplay();
            break;
            
        case 'b':
            recordmode = 1;
            playbackmode = 0;
            printf("\nRecording....Press 'E' to end");
            event_ptr = 0;
            break;
            
        case 'E':
            if (recordmode == 1){
                recordmode = 0;
                printf("\nRecording stopped. Press 's' to save playback file");
            }
            break;
            
        case 'l':
            recordmode = 0;
            playbackmode = 0;
            event_ptr = 0;
            playback_ptr = 0;
            printf("\nLoading file %s ", fileName);
            
            jFile = fopen(fileName, "r");
            if (jFile == NULL){
                printf("\nWarning : Could not open %s", fileName);
            }
            
            else{
                
                while ((fscanf(jFile, "%d ", &event_buffer[event_ptr])) != EOF){
                    event_ptr++;
                }
                fclose(jFile);
                playbackmode = 1;
                //printf(" %d", event_ptr);
            }
            
            break;
            
        case 'r':
            recordmode = 0;
            playbackmode = 0;
            event_ptr = 0;
            playback_ptr = 0;
            break;
            
        case 'p':
            if (playbackmode == 1){
                glutTimerFunc(4, timerFunc, 1);
                
            }
            
            break;
            
        case 's':
            
            recordmode = 0;
            playbackmode = 0;
            jFile = fopen(fileName, "w");
            
            if (jFile == NULL)
                printf("\nWarning : Could not open %s", fileName);
            
            else{
                
                for (int j = 0; j < event_ptr; j++)
                    fprintf(jFile, "%d ", event_buffer[j]);
                fclose(jFile);
                printf("\nEvents saved in %s", fileName);
                
            }
            
            playback_ptr = 0;
            break;
    }
}

//Lighting and Shading
void myinit()
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 0.0, 0.0 };
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess = { 100.0 };
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LEQUAL);  // value used for depth buffer comparison
    glEnable(GL_DEPTH_TEST); // passes if the incoming value is <= the stored depth value
    
    glClearColor(0.0, 1.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);
    
    /* allocate quadrics with filled drawing style */
    
    h = gluNewQuadric();
    gluQuadricDrawStyle(h, GLU_FILL);
    t = gluNewQuadric();
    gluQuadricDrawStyle(t, GLU_FILL);
    lua = gluNewQuadric();
    gluQuadricDrawStyle(lua, GLU_FILL);
    lla = gluNewQuadric();
    gluQuadricDrawStyle(lla, GLU_FILL);
    rua = gluNewQuadric();
    gluQuadricDrawStyle(rua, GLU_FILL);
    rla = gluNewQuadric();
    gluQuadricDrawStyle(rla, GLU_FILL);
    lul = gluNewQuadric();
    gluQuadricDrawStyle(lul, GLU_FILL);
    lll = gluNewQuadric();
    gluQuadricDrawStyle(lll, GLU_FILL);
    rul = gluNewQuadric();
    gluQuadricDrawStyle(rul, GLU_FILL);
    rll = gluNewQuadric();
    gluQuadricDrawStyle(rll, GLU_FILL);
    
    
    
    
    
    // setting up the tree
    
    
    glLoadIdentity();
    glRotatef(theta[0], 0.0, 1.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,torso_node.m);
    torso_node.f = torso;
    torso_node.sibling = NULL;
    torso_node.child =  &head_node;
    
    glLoadIdentity();
    glTranslatef(0.0, TORSO_HEIGHT+0.5*HEAD_HEIGHT, 0.0);
    glRotatef(theta[1], 1.0, 0.0, 0.0);
    glRotatef(theta[2], 0.0, 1.0, 0.0);
    glTranslatef(0.0, -0.5*HEAD_HEIGHT, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,head_node.m);
    head_node.f = head;
    head_node.sibling = &lua_node;
    head_node.child = NULL;
    
    glLoadIdentity();
    glTranslatef(-(TORSO_RADIUS+UPPER_ARM_RADIUS), 0.9*TORSO_HEIGHT, 0.0);
    glRotatef(theta[3], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,lua_node.m);
    lua_node.f = left_upper_arm;
    lua_node.sibling =  &rua_node;
    lua_node.child = &lla_node;
    
    glLoadIdentity();
    glTranslatef(TORSO_RADIUS+UPPER_ARM_RADIUS, 0.9*TORSO_HEIGHT, 0.0);
    glRotatef(theta[5], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rua_node.m);
    rua_node.f = right_upper_arm;
    rua_node.sibling =  &lul_node;
    rua_node.child = &rla_node;
    
    glLoadIdentity();
    glTranslatef(-(TORSO_RADIUS+UPPER_LEG_RADIUS), 0.1*UPPER_LEG_HEIGHT, 0.0);
    glRotatef(theta[7], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,lul_node.m);
    lul_node.f = left_upper_leg;
    lul_node.sibling =  &rul_node;
    lul_node.child = &lll_node;
    
    glLoadIdentity();
    glTranslatef(TORSO_RADIUS+UPPER_LEG_RADIUS, 0.1*UPPER_LEG_HEIGHT, 0.0);
    glRotatef(theta[9], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rul_node.m);
    rul_node.f = right_upper_leg;
    rul_node.sibling =  NULL;
    rul_node.child = &rll_node;
    
    glLoadIdentity();
    glTranslatef(0.0, UPPER_ARM_HEIGHT, 0.0);
    glRotatef(theta[4], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,lla_node.m);
    lla_node.f = left_lower_leg;
    lla_node.sibling =  NULL;
    lla_node.child = NULL;
    
    glLoadIdentity();
    glTranslatef(0.0, UPPER_ARM_HEIGHT, 0.0);
    glRotatef(theta[6], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rla_node.m);
    rla_node.f = right_lower_arm;
    rla_node.sibling =  NULL;
    rla_node.child = NULL;
    
    glLoadIdentity();
    glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
    glRotatef(theta[8], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,lll_node.m);
    lll_node.f = left_lower_leg;
    lll_node.sibling =  NULL;
    lll_node.child = NULL;
    
    glLoadIdentity();
    glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
    glRotatef(theta[10], 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rll_node.m);
    
    
    glColor3d(1,0,0);
    rll_node.f = right_lower_leg;
    rll_node.sibling =  NULL;
    rll_node.child = NULL;
    
    glLoadIdentity();
    
}



//Main Function
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("FIFA");
    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
}