#include "common.h"

#define NUM_VERTICES       8     /* a cube has eight corners */
#define NUM_FACES          6     /* a cube has six faces */


typedef struct VTX		 /* vertex data */
{
    fixed x, y, z;
} VTX;


typedef struct QUAD              /* four vertices makes a quad */
{
    VTX *vtxlist;
    int v1, v2, v3, v4;          /* these vars define the points in vtxlist
	   			     used for this QUAD */
} QUAD;


typedef struct SHAPE             /* store position of a shape */
{
    fixed x, y, z;               /* x, y, z position */
    fixed rx, ry, rz;            /* rotations */
    fixed drx, dry, drz;         /* speed of rotation */
} SHAPE;


#define TEST 32
VTX points[] =                   /* a cube, centered on the origin */
{
   /* vertices of the cube, of type fixed */
   { -TEST << 16, -TEST << 16, -TEST << 16 },
   { -TEST << 16,  TEST << 16, -TEST << 16 },
   {  TEST << 16,  TEST << 16, -TEST << 16 },
   {  TEST << 16, -TEST << 16, -TEST << 16 },
   { -TEST << 16, -TEST << 16,  TEST << 16 },
   { -TEST << 16,  TEST << 16,  TEST << 16 },
   {  TEST << 16,  TEST << 16,  TEST << 16 },
   {  TEST << 16, -TEST << 16,  TEST << 16 },
};


QUAD faces[] =                   /* group the vertices into polygons */
{
   { points, 0, 3, 2, 1 },
   { points, 4, 5, 6, 7 },
   { points, 0, 1, 5, 4 },
   { points, 2, 3, 7, 6 },
   { points, 0, 4, 7, 3 },
   { points, 1, 2, 6, 5 }
};


SHAPE shape;


/* somewhere to put translated vertices */
VTX output_points[NUM_VERTICES];
QUAD output_faces[NUM_FACES];

/* initialise shape position */
void init_shape()
{
    shape.x = 0;
    shape.y = 0;
    shape.z = itofix(50);
    shape.rx = 0;
    shape.ry = 0;
    shape.rz = 0;

	srand(NULL);
    shape.drx = (rand() & 0x1FFFF) - 0x10000;
    shape.dry = (rand() & 0x1FFFF) - 0x10000;
    shape.drz = (rand() & 0x1FFFF) - 0x10000;
}


/* update shape positions */
#define MULT 1
void animate_shape()
{
    shape.rx += MULT * shape.drx;
    shape.ry += MULT * shape.dry;
    //shape.rz += MULT * shape.drz;
}


/* translate shape from 3d world space to 2d screen space */
void translate_shape()
{
    int d;
    MATRIX matrix;
    VTX *outpoint = output_points;
    QUAD *outface = output_faces;

    /* build a transformation matrix */
    get_transformation_matrix(&matrix, itofix(1),
			      shape.rx, shape.ry, shape.rz,
			      shape.x, shape.y, shape.z);

    /* output the vertices */
    for (d=0; d<NUM_VERTICES; d++)
	{
		apply_matrix(&matrix, points[d].x, points[d].y, points[d].z, &outpoint[d].x, &outpoint[d].y, &outpoint[d].z);
		persp_project(outpoint[d].x, outpoint[d].y, outpoint[d].z, &outpoint[d].x, &outpoint[d].y);
    }

    /* output the faces */
    for (d=0; d<NUM_FACES; d++)
	{
		outface[d] = faces[d];
		outface[d].vtxlist = outpoint;
    }
}


/* draw a quad */
void quad(BITMAP *b, VTX *v1, VTX *v2, VTX *v3, VTX *v4)
{
    int col;
    fixed x, y, z;
    
    /* four vertices */
    V3D vtx1 = { v1->x, v1->y, v1->z, 0, 0, 0 };
    V3D vtx2 = { v2->x, v2->y, v2->z, 0, 0, 0 };
    V3D vtx3 = { v3->x, v3->y, v3->z, 0, 0, 0 };
    V3D vtx4 = { v4->x, v4->y, v4->z, 0, 0, 0 };
    
    /* use the cross-product to cull backfaces */
    cross_product(v2->x-v1->x, v2->y-v1->y, 0, v3->x-v2->x, v3->y-v2->y, 0, &x, &y, &z);
    if (z < 0)
      return;
    
    /* set up the vertex color */
    col = MID(128, 255 - fixtoi(v1->z+v2->z) / 16, 255);
    vtx1.c = col;
    
    /* draw the quad */
    quad3d(b, POLYTYPE_FLAT, NULL, &vtx1, &vtx2, &vtx3, &vtx4);
}


/* draw the shape calculated by translate_shape() */
void draw_shape(BITMAP *b)
{
    int c, clr  = makecol(0,45,89),
		   clr2 = makecol(0,28,55);
    QUAD *face = output_faces;
    VTX *v1, *v2, *v3, *v4;
    
    for (c=0; c < NUM_FACES; c++)
	{
		/* find the vertices used by the face */
		v1 = face->vtxlist + face->v1;
		v2 = face->vtxlist + face->v2;
		v3 = face->vtxlist + face->v3;
		v4 = face->vtxlist + face->v4;

		/* draw the face */
		//quad(b, v1, v2, v3, v4);

		//printff_direct("x: %d y: %d", (v1->x >> 16), (v1->y >> 16));
		
//if (c == 0)
{
		line(tmpscreen, v1->x >> 16, v1->y >> 16, v2->x >> 16, v2->y >> 16, clr);
		line(tmpscreen, v2->x >> 16, v2->y >> 16, v3->x >> 16, v3->y >> 16, clr);
		line(tmpscreen, v3->x >> 16, v3->y >> 16, v4->x >> 16, v4->y >> 16, clr);
		line(tmpscreen, v4->x >> 16, v4->y >> 16, v1->x >> 16, v1->y >> 16, clr);

		line(tmpscreen, v1->x >> 16, v1->y >> 16, v3->x >> 16, v3->y >> 16, clr2);
		line(tmpscreen, v2->x >> 16, v2->y >> 16, v4->x >> 16, v4->y >> 16, clr2);

		//circlefill(tmpscreen, v1->x >> 16, v1->y >> 16, 3, clr);
		//circlefill(tmpscreen, v2->x >> 16, v2->y >> 16, 3, clr);
		//circlefill(tmpscreen, v3->x >> 16, v3->y >> 16, 3, clr);
		//circlefill(tmpscreen, v4->x >> 16, v4->y >> 16, 3, clr);
}
		face++;
    }
}
