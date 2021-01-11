///////////////////////////////////////////////////////
//	Andoni Beristain Iraola. PhD. Student. 2009
//  Computer Intelligence Group (GIC). University of The Basque Country UPV/EHU.
//	beristainandoni@yahoo.es
///////////////////////////////////////////////////////

//Limited to 320*240 sized images. Only for testing purposes
#define I_WIDTH 320
#define I_HEIGHT 240
#define I_SIZE I_WIDTH*I_HEIGHT

#define BLACK 0
#define WHITE 255
#define SKELETON 128

//Helper functions
bool isBackground(uchar* im,int x, int y);
void setPixel(int x, int y, uchar* im,int value);
double euclidnorm(int x, int y);
int fAux(int a,int b, int* g);
int Sep(int i,int u, int* g);
void compare (int x, int y, int p, int q, int xf, int pf,int yLim, float tuneGrain,uchar* imSal) ;


//Main procedure for skeletonization
void EuclideanSkeleton(uchar inI[I_SIZE],uchar* outI);

