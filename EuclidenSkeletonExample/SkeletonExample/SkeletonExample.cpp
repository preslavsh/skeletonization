///////////////////////////////////////////////////////
//	Andoni Beristain Iraola. PhD. Student. 2009
//  Computer Intelligence Group (GIC). University of The Basque Country UPV/EHU.
//	beristainandoni@yahoo.es
///////////////////////////////////////////////////////

#include "stdafx.h"
#include "Skeleton.h"

#define I_WIDTH 320
#define I_HEIGHT 240

#include <string>

int main(int argc, CHAR* argv[])
{
	if(argc>1)
	{
		//First parameter is the path of the input image
		std::string imPath(argv[1]);

		//Load input image
		IplImage* imI=cvLoadImage(imPath.c_str());

		CvSize imSize=cvGetSize(imI);
		if((imSize.width!=I_WIDTH)||(imSize.height!=I_HEIGHT))
		{
			cvReleaseImage(&imI);
			printf("The image must have a resolution of 320*240\n");
			exit(0);
		}

		//Show original image in grayscale
		IplImage* imIgray=cvCreateImage(imSize,IPL_DEPTH_8U,1);
		cvCvtColor( imI,imIgray,CV_BGR2GRAY);
		cvvNamedWindow("Input_Image");
		cvvShowImage("Input_Image",imIgray);
		
		//Convert the image into a pixel array
		uchar* rawImage=NULL;
		int stepImage;
		cvGetRawData(imIgray,&rawImage,&stepImage);
		stepImage/=sizeof(rawImage[0]);
		
		//Compute skeleton
		uchar rawSkeleton[I_WIDTH*I_HEIGHT];
		for(int i=0;i<I_WIDTH*I_HEIGHT;i++)
		{
			rawSkeleton[i]=0;
		}
		EuclideanSkeleton(rawImage,&rawSkeleton[0]);

		//Show result Skeleton
		IplImage* imO;
		imO=cvCreateImage(imSize,IPL_DEPTH_8U,1 );

		cvSetData(imO,&rawSkeleton[0],stepImage);
		cvvNamedWindow("Output_Image");
		cvvShowImage("Output_Image",imO);

		cvvWaitKey(0);

		cvReleaseImage(&imI);
		cvReleaseImage(&imO);
	}
	else
	{
		printf("///////////////////////////////////////////////\nA binary image path is required\nCall SkeletonExample.exe image\n///////////////////////////////////////////////\n");
	}

	 
	return 0;
}

