///////////////////////////////////////////////////////
//	Andoni Beristain Iraola. PhD. Student. 2009
//  Computer Intelligence Group (GIC). University of The Basque Country UPV/EHU.
//	beristainandoni@yahoo.es
///////////////////////////////////////////////////////

#include "stdafx.h"
#include "Skeleton.h"


//Additional variables for the skeletonization procedure
//Static definition of variables focused in performance
int INFTY=(int)(1+sqrt(static_cast<float>(I_WIDTH*I_WIDTH+I_HEIGHT*I_HEIGHT)));
int LIM=max(I_HEIGHT,I_WIDTH);
long dtline[I_WIDTH][I_HEIGHT];
long ftline[I_WIDTH];
long ftx[I_HEIGHT][I_WIDTH];
int dt[I_WIDTH][I_HEIGHT];
int ft[I_WIDTH][I_WIDTH] ;
int ss[I_WIDTH] ; // scratch variable for oneDT
int tt[I_WIDTH+1] ; // scratch variable for oneDT

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Main procedure for skeletonization
/////////////////////////////////////////////////////////////////////////////////////////////////////////


//Main procedure for skeletonization
//Input: 
// in : array containing the source image for skeletonization in grayscale (0.255).
// Both source and destination array are ordered by rows
//Output:
// out: array containing the resultant skeleton 
void EuclideanSkeleton(uchar inI[I_SIZE],uchar* outI)
{
	int x,y,right,left,xy;
	int yLim=I_HEIGHT;
	int xLim=I_WIDTH;
	int q=0;
	int w=0; 

	//Phase 1

	for (int y = 0 ; y < yLim ; y ++) 
	{
		dtline [xLim - 1][y] = right = 
			(isBackground(inI,xLim - 1,y) ? 0 : INFTY) ;
		/* INFTY simulates a isBackground outside the image */

		//(*scan 1*)
		for (x = xLim - 2 ; x >= 0 ; x --) 
		{
			dtline[x][y] = right = (isBackground(inI,x,y) ? 0 : right + 1) ;
		}

		//(*scan 2*)
		ftx[y][0] = left = dtline[0][y] ;
		for (x = 1 ; x < xLim ; x ++) 
		{
			right = dtline[x][y] ;
			ftx[y][x] = left = (x - left <= right ? left : x + right) ;

		}
	}

	//Phase 2

	int maxDt=0;	
	for (int x = 0 ; x < xLim ; x ++) 
	{
		for (y = 0 ; y < yLim ; y ++) 
		{
			ftline[y] = xy = ftx[y][x] ;
			dtline[x][y] = (long)pow(static_cast<float>(xy - x),2) ;
		}


		//(*scan 1*)
		ss[0] = tt[0] = 0 ;	
		q=0;
		for (int j = 1 ; j < /*LIM*/yLim ; j ++) 
		{
			while (q >= 0 && 
				(j-ss[q])*(j+ss[q]-2*tt[q]) < dtline[x][ss[q]]-dtline[x][j])
			{
				q-- ;
			}
			if (q < 0) 
			{
				q = 0 ;
				ss[0] = j ;
			} else {
				w = 1 + ((j+ss[q])*(j-ss[q])+dtline[x][j]-dtline[x][ss[q]])
					/ (2*(j-ss[q])) ;
				if (w < yLim )
				{
					q ++ ;
					ss[q] = j ;
					tt[q] = w ; 
				}
			}
		}
		//(*scan 2*)
		for (int j = yLim - 1 ; j >= 0 ; j --) 
		{
			dt[x][j] = dtline[x][ss[q]] + (long)pow(static_cast<float>(ss[q]-j),2);

			//if (ft[x] != 0) 
			//{
			ft[x][j] = /*(ftline == NULL ? ss[q] : */ftline[ss[q]] * (yLim) + ss[q]/*)*/ ;
			//}
			if (j == tt[q]) q -- ;

		}
	}

	//Compute IMA
	float tg=5;

	for (int x = 0 ; x < xLim ; x ++) 
	{
		for (y = 1 ; y < yLim ; y++)
		{
			if(x>0)
			{
				if((!isBackground(inI,x,y))||(!isBackground(inI,x-1,y)))
				{
					compare(x, y, x-1, y, ft[x][y], ft[x-1][y],xLim, tg,outI) ;
				}
			}
			if(y>0)
			{
				if((!isBackground(inI,x,y))||(!isBackground(inI,x,y-1)))
				{
					compare(x, y, x, y-1, ft[x][y], ft[x][y-1],yLim, tg,outI) ;
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Helper functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Indicates if a pixel belongs to the background of the image
//Input:
//im: array containing the input image. Ordered by rows
//x: column
//y: row
//Output:
//Returns true if the pixel is black, belongs to the background
bool isBackground(uchar* im,int x, int y)
{
	return (im[y*I_WIDTH+x]==BLACK);
}

//Assigns a value to a pixel in an image.
//Input:
//im: array containing the input image. Ordered by rows
//x: column
//y: row
//Input/Output:
//im: The input array is modified.
void setPixel(uchar* im,int x, int y,int value)
{
	im[y*I_WIDTH+x]=value;
}


//Computes de Euclidean norm.
//Input:
//x: first value
//y: second value
//Return:
//Euclidean norm.
double euclidnorm(int x, int y) 
{
	return sqrt(static_cast<double>(x * x + y * y)) ;
}


//Classifies a pixel as part of the skeleton or not.
//Input:
//x: first pixel row
//y: first pixel column
//p: second pixel row
//q: second pixel column
//xf: ft value for first pixel
//pf: ft value for second pixel
//yLim: Maximun dimension of the image. (width or height)
//tuneGrain: Type of pruning. Constant, linear or square root
//Output:
//imSal: If the pixel belongs to the skeleton that pixel is assigned SKELETON value.
void compare (int x, int y, int p, int q, int xf, int pf, int yLim, float tuneGrain,uchar* imSal) 
{
	// compute feature transform vectors(xf, yf), (pf, qf)
	int yf, qf, dif;
	yf = xf % yLim ; xf = xf / yLim ;
	qf = pf % yLim ; pf = pf / yLim ;
	dif = (int)(pow(static_cast<float>(xf - pf),2) + pow(static_cast<float>(yf - qf),2)) ;
	if (dif > 1 && dif > 
		(tuneGrain > 0 ? tuneGrain * tuneGrain // Constant pruning
		: tuneGrain < 0 ?     // Linear pruning
		1 + (pow(static_cast<float>(x-xf+p-pf),2)+pow(static_cast<float>(y-yf+q-qf),2))*tuneGrain*tuneGrain/4
		: // tuneGrain == 0   // Square root pruning
	euclidnorm(x-xf+p-pf, y-yf+q-qf) 
		+ 2 * ((x-p)*(xf-pf) + (y-q)*(yf-qf)) + 1 ) )
	{
		// one of the points is in the skeleton
		int innerproduct = (xf-pf)*(xf+pf-x-p) + (yf-qf)*(yf+qf-y-q) ;

		if (innerproduct >=  0) 
		{
			setPixel(imSal,x,y,SKELETON);
		}
		if (innerproduct <= 0) 
		{
			setPixel(imSal,p,q,SKELETON);
		}
	}
}