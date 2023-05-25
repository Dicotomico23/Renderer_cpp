///#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#define WIDTH 1000
#define HEIGHT 600
#define MAX_OUTBUF 128
#define MAX_ARR 10000
#define ARCHIVO1 "C:\\Modelos.obj\\airplane.obj"
#define ARCHIVO2 "C:\\Modelos.obj\\cubo.obj"

class Vertices{
public:
    float x;
    float y;
    float z;
    float w;//cuarta componente para coordenadas homogeneas
};

class Texturas{
public:
    float t1;
    float t2;
};

class Caras{
public:
    int c1;
    int c2;
    int c3;
};

class Objeto3D{
public:
    vector<Vertices> vert;
    vector<Caras> c;
    int indicesVert[MAX_ARR][MAX_ARR];
    int numVertices=0, numCaras=0, contVert=0;
};

///PROTOTIPO DE FUNCIONES///
void IniciarPuntosControl();
Vertices Bezier(float t, Vertices p0, Vertices p1, Vertices p2, Vertices p3);
void display(void);
void init(void);
bool LoadObjFile(const char* filepath, int index);
Vertices MultMat4x4Vector4x1(float mat[4][4],Vertices v);
void MultMat4x4Matriz4x4(float mat1[4][4], float mat2[4][4], float res[4][4]);
void ImprimirMatriz(float Matriz[4][4]);
void graficaCurvaBezier(void);
//////////////////////////

///VARIABLES GLOBALES///
Objeto3D obj[2];
int NumModelos=2;
int cont_tiempo = 0;
Vertices p0, p1, p2, p3;
///////////////////////

void IniciarPuntosControl()
{
    p0.x = -5;
    p0.y = -5;
    p0.z = 0;

    p1.x = 5;
    p1.y = 1;
    p1.z = 3;

    p2.x = -5;
    p2.y = 5;
    p2.z = 0;

    p3.x = 0;
    p3.y = 0;
    p3.z = 0;
}

Vertices Bezier(float t, Vertices p0, Vertices p1, Vertices p2, Vertices p3)
{
    Vertices v;
    v.x = pow((1-t),3)*p0.x + 3*t*pow((1-t),2)*p1.x + 3*pow(t,2)*(1-t)*p2.x + pow(t,3)*p3.x;
    v.y = pow((1-t),3)*p0.y + 3*t*pow((1-t),2)*p1.y + 3*pow(t,2)*(1-t)*p2.y + pow(t,3)*p3.y;
    v.z = pow((1-t),3)*p0.z + 3*t*pow((1-t),2)*p1.z + 3*pow(t,2)*(1-t)*p2.z + pow(t,3)*p3.z;
    return v;
}

float t=0;
float angulo = -5;
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    IniciarPuntosControl();
    Vertices v = Bezier(t,p0,p1,p2,p3);
    float MatrizTraslacion[4][4]=  {{1,0,0,v.x},
                                    {0,1,0,v.y},
                                    {0,0,1,v.z},
                                    {0,0,0,1}};

    float MatrizOrigen[4][4]=  {{1,0,0,0},
                                {0,1,0,0},
                                {0,0,1,0},
                                {0,0,0,1}};

    float MatrizRotacion[4][4]={{1,0,0,0},
                                {0,cos(angulo*t),-sin(angulo*t),0},
                                {0,sin(angulo*t), cos(angulo*t),0},
                                {0,0,0,1}};
    float MatrizRes1[4][4]={0};
    float MatrizModelo[4][4]={0};

    glBegin(GL_QUADS);
    glColor3f(0.2, 0.2, 0);

    for(int contModelo1=0;contModelo1<obj[0].numCaras;contModelo1++)
    {
        Vertices v1_m1 = obj[0].vert[obj[0].indicesVert[contModelo1][0]-1];
        Vertices v2_m1 = obj[0].vert[obj[0].indicesVert[contModelo1][1]-1];
        Vertices v3_m1 = obj[0].vert[obj[0].indicesVert[contModelo1][2]-1];
        Vertices v4_m1 = obj[0].vert[obj[0].indicesVert[contModelo1][3]-1];

        MultMat4x4Matriz4x4(MatrizTraslacion,MatrizRotacion,MatrizRes1);
        MultMat4x4Matriz4x4(MatrizRes1,MatrizOrigen,MatrizModelo);

        v1_m1 = MultMat4x4Vector4x1(MatrizModelo,v1_m1);
        v2_m1 = MultMat4x4Vector4x1(MatrizModelo,v2_m1);
        v3_m1 = MultMat4x4Vector4x1(MatrizModelo,v3_m1);
        v4_m1 = MultMat4x4Vector4x1(MatrizModelo,v4_m1);

        glVertex3f(v1_m1.x, v1_m1.y, v1_m1.z);
        glVertex3f(v2_m1.x, v2_m1.y, v2_m1.z);
        glVertex3f(v3_m1.x, v3_m1.y, v3_m1.z);
        glVertex3f(v4_m1.x, v4_m1.y, v4_m1.z);
    }

    for(int contModelo1=0;contModelo1<obj[1].numCaras;contModelo1++)
    {
        Vertices v1_m1 = obj[1].vert[obj[1].indicesVert[contModelo1][0]-1];
        Vertices v2_m1 = obj[1].vert[obj[1].indicesVert[contModelo1][1]-1];
        Vertices v3_m1 = obj[1].vert[obj[1].indicesVert[contModelo1][2]-1];
        Vertices v4_m1 = obj[1].vert[obj[1].indicesVert[contModelo1][3]-1];

        glVertex3f(v1_m1.x, v1_m1.y, v1_m1.z);
        glVertex3f(v2_m1.x, v2_m1.y, v2_m1.z);
        glVertex3f(v3_m1.x, v3_m1.y, v3_m1.z);
        glVertex3f(v4_m1.x, v4_m1.y, v4_m1.z);
    }
    if(cont_tiempo==20)
    {
        if(t<1)
            t += 0.01;
        cont_tiempo=0;
    }
    cont_tiempo++;
    glEnd();
    glutSwapBuffers();
    glFlush();
}

void init(void)
{
	/*  select clearing (background) color       */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//IMPORTANTE
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_EQUAL);
	//
	/*  initialize viewing values  */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	//gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	gluPerspective(45.0, WIDTH/HEIGHT, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(9.0, 0.0, 0.0, 0.0,0.0,0.0, 0.0, 1.0, 0.0  );
}

bool LoadObjFile(const char* filepath, int index)
{
    Caras cara = Caras();
    FILE * file = fopen(filepath, "r");
    if (file == NULL){
        cout<<"No se pudo abrir el archivo :("<<endl;
        return false;
    }else{cout<<"Cargando el modelo 3D :) ..."<<endl;}
    while (1){
        char outbuf[MAX_OUTBUF];
        int output = fscanf(file, "%s", outbuf);
        if (output == EOF)
            break;
        if(strcmp(outbuf,"v")==0)
        {
            Vertices vert = Vertices();
            fscanf(file,"%f %f %f",&vert.x,&vert.y,&vert.z);
            obj[index].vert.push_back(vert);
            obj[index].numVertices++;
        }
        if(strcmp(outbuf,"f")==0)
        {
            char aux;
            int cont = 0;
            do{
                fscanf(file,"%d/%d/%d", &cara.c1, &cara.c2, &cara.c3);
                obj[index].c.push_back(cara);
                obj[index].indicesVert[obj[index].numCaras][cont] = cara.c1;
                cont++;
                aux = getc(file);
                obj[index].contVert++;
            }while(aux==' ');
            obj[index].numCaras++;
        }
    }
    fclose(file);
}

int main(int argc, char** argv)
{
    printf("INICIA\n");
    LoadObjFile(ARCHIVO1, 0);
    LoadObjFile(ARCHIVO2, 1);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Render");
	init();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();

	return 0;   /* ISO C requires main to return int. */
}

Vertices MultMat4x4Vector4x1(float mat[4][4],Vertices v)
{
    Vertices res;
    v.w = 1;
    res.x = mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3] * v.w;
    res.y = mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3] * v.w;
    res.z = mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3] * v.w;
    res.w = 1;
    return res;
}

void MultMat4x4Matriz4x4(float mat1[4][4], float mat2[4][4], float res[4][4])
{
    for(int k=0;k<4;k++)
    {
        for(int i=0;i<4;i++)
        {
            res[k][i] = 0;
            for(int j=0;j<4;j++)
            {
                res[k][i] += mat1[k][j] * mat2[j][i];
            }
        }
    }
}

void ImprimirMatriz(float Matriz[4][4])
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            printf("[%0.2f\t]", Matriz[i][j]);
        }
        printf("\n");
    }
}
