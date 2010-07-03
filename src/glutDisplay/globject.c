/*
 * =====================================================================================
 *       Filename:  globject.c
 *
 *    Description:  :
 *
 *        Version:  1.0
 *        Created:  07/03/2010 11:57:28 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai , djx.zhenghua@gmail.com
 *        Company:  
 *
 * =====================================================================================
 *     Copyright (c) 2010, Zhenghua Dai. All rights reserved.
 * =====================================================================================
 */
#include <GL/gl.h>
#include <GL/glut.h>

/*
 * Move object into position.  Use 3rd through 12th 
 * parameters to specify the material property.  Draw a teapot.
 */
void renderTeapot(GLfloat x, GLfloat y, GLfloat z,
        GLfloat ambr, GLfloat ambg, GLfloat ambb,
        GLfloat difr, GLfloat difg, GLfloat difb,
        GLfloat specr, GLfloat specg, GLfloat specb, GLfloat shine)
{
    GLfloat mat[4];

    glPushMatrix();
    glTranslatef(x, y, z);
    //   glRotatef(spin, 0.0, 0.0, 1.0);
#if 0 
    mat[0] = ambr; mat[1] = ambg; mat[2] = ambb; mat[3] = 1.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
    mat[0] = difr; mat[1] = difg; mat[2] = difb;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
    mat[0] = specr; mat[1] = specg; mat[2] = specb;
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
    glMaterialf(GL_FRONT, GL_SHININESS, shine * 128.0);
#endif
    //   glCallList(teapotList);
    glDisable(GL_TEXTURE_2D);
    glutSolidTeapot(.2);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

