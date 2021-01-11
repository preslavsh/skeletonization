# Skeletonization

## An augmented Fast Marching Method for computing skeletons and centerlines

## Medial axis skeletonization

## Multiscale 2D medial axes and 3D surface skeletons by the image foresting transform

## Euclidean distance-based skeletons: a few notes on average outward flux and ridgeness

## Euclidean Skeletons of Digital Image and Volume Data in Linear Time by the Integer Medial Axis Transform
http://www.ehu.eus/ccwintco/index.php/Euclidean_skeletons_of_digital_image_and_volume_data_in_linear_time_by_integer_medial_axis_transform
Note that our algorithm works directly on grid data (pixels in 2D, voxels in 3D).

The skeletonization algorithm has two phases. First, a
feature transform is computed which uses essentially the
same algorithm as for the distance transform, the difference
being that not only are distances computed, but also
background points that realize the closest distance. The
actual skeletonization is performed in a second pass
through the data, where the integer medial axis is
computed by assigning points to the skeleton depending
on their feature transform

code is parsed but needs work

## Skeleton Extraction of 2D Objects Using Shock Wavefront Detection

## Fast and robust curve skeletonization for real-world elongated objects

## A Novel Algorithm for Skeleton Extraction From Images Using Topological Graph Analysis

## Maybe useful:
https://github.com/javierfdr/thesis-repo/tree/master/daily-work
https://scikit-image.org/docs/dev/auto_examples/edges/plot_skeleton.html
https://imagej.net/AnalyzeSkeleton

Terms: 
euclidean distance transform (EDT)
enters of maximal (integer) disks CMD
integer medial axis IMA
real medial axis RMA


# Voronoi diagram
https://stackoverflow.com/questions/16741337/from-voronoi-diagram-to-skeleton
A simple algorithm that I have seen to work with some polygonal shapes works like this:
Create the Voronoi diagram
Remove all half-edges so that only edges connecting two "finite" points remain
Optionally prune edges that connect two points on the boundary of your shape
The remaining graph is, for some cases, a pretty good approximation to the skeleton of your shape

https://www.learnopencv.com/delaunay-triangulation-and-voronoi-diagram-using-opencv-c-python/
https://github.com/arnaud-ramey/voronoi/blob/master/src/voronoi.h
https://www.mathworks.com/matlabcentral/fileexchange/27543-skeletonization-using-voronoi
https://pythonpedia.com/en/knowledge-base/33095476/is-there-any-build-in-function-can-do-skeletonization-in-opencv-

The skeleton/MAT can be produced in two main ways.

The first is to use some kind of morphological thinning that successively erodes 
away pixels from the boundary (while preserving the end points of line segments)
 until no more thinning is possible, at which point what is left approximates the skeleton.

The alternative method is to first calculate the distance transform of the image. 
The skeleton then lies along the singularities (i.e. creases or curvature discontinuities) 
in the distance transform. This latter approach is more suited to calculating the MAT 
since the MAT is the same as the distance transform but with all points off the skeleton suppressed to zero.
https://stackoverrun.com/ru/q/9119111


https://rosettacode.org/wiki/Voronoi_diagram#Python
https://medium.com/datadriveninvestor/computational-geometry-for-computer-vision-f140fab91c76
# Media Axis Transform
https://scikit-image.org/docs/0.12.x/auto_examples/edges/plot_medial_transform.html

# Morphological algorithms
https://github.com/linbojin/Skeletonization-by-Zhang-Suen-Thinning-Algorithm
http://agcggs680.pbworks.com/f/Zhan-Suen_algorithm.pdf

http://www.jatit.org/volumes/Vol94No2/2Vol94No2.pdf
https://github.com/LitLeo/Thinning_Soft_by_CUDA/blob/master/GuoHall/OpenCV/thinningGuoHall.cpp


Algorithm proposed by Guo and Hall [191] (GH) is two sub-iteration parallel thinning
algorithm. The GH gives thinner skeletons than ZS and LW algorithms.
In that algorithm new function N(P) is introduces. It allows to preserve the end points as
well as to remove redundant pixels. The function NP is defined:
N(P) = Min(N1(P1), N2(P1)); where
N1(P1) = (P9 V P2) Λ (P3 V P4) Λ (P5 V P6) Λ (P7 V P8)
N2(P1) = (P2 V P3) Λ (P4 V P5) Λ (P6 V P7) Λ (P8 V P9)
An edge point will be deleted if it satisfies these conditions:
1. C(P1) = l;
2. 2 ≤ N(P1) ≤ 3;
3. Then apply one of the following depending of the iteration:
a) (P2 V P3 V P5) Λ P4 = 0 in odd iterations; or
b) (P6 V P7 V P9) Λ P8 = 0 in even iterations
Condition 1 preserves local connectivity that means the deletion of pixel P1 does not
break the connectivity and guarantees that pixel P1 is not a break point [1]. Condition 3(a)
deletes north-east pixels and 3(b) deletes south-west pixels [180]. The GH algorithm detects the
end points better than the ZS algorithm [1].


Zhang Suen thinning algorithm [186] (ZS) is fast parallel thinning algorithm with two
sub-iterations. The conditions used in the first sub-iteration in order to remove the south-east
pixel are:
1.2 ≤ B(P1) ≤ 6
2.A(P1) = 1
3.P2 Λ P4 Λ P6 = 0
4.P4 Λ P6 Λ P8 = 0
In the second sub-iteration conditions in steps 3 and 4 change, in order to remove the
north-west pixels:
3.P2 Λ P4 Λ P8 = 0
4.P2 Λ P6 Λ P8 = 0
End points [181] and pixel connectivity should be preserved [1]. Issue with that algorithm
is that with the presence of a noise near north-east and south-west corners these are extended
instead of deleted [180]


All tested thinning algorithms in this paper are iterative parallel algorithms. Whether the
pixels will be deleted in nth iteration depends in parallel thinning on the result from the previous
(n-1)th iteration. Values of the pixels and its neighbors at the (n-1)th iteration determine the values 
of the pixels at the next n
th iteration. Parallel thinning algorithms usually use a 3*3 matrix that
represents neighborhood around the examined pixel as shown in table 1.
Table 1 Matrix 3x3 represents 8-neighboorhood of pixel P1
P9 P2 P3
P8 P1 P4
P7 P6 P5
For the purposes of iterative parallel thinning algorithms let´s propose matrix 3x3 and
define the three functions B(P), A(P) and C(P).
B(P1) represents the number of non-zero neighbors of P1. It is computed as:
B(P1) = P2+P3+.............................+P9.
A(P1) represents the number of 0,1 patterns in the sequence P2, P3, P4, P5, P6, P7, P8,
P9, P2. Examples of these functions can be seen on fig 1.
Fig 1 Example of functions B(P1) and A(P1): a) B(p1) = 2 , A(p1) = 1; b) B(p1) = 2, A(p1) = 2
The function C(P) is connectivity number. C(P1) function is little harder to understand so
fig. 2 is proposed. C(P1) is the number of distinct 8-connected components count in the
neighborhood of the pixel P1. One of the ways how to compute function C(P1) can be:
C(P1) = !P2 Λ (P3 V P4) + !P4 Λ (P5 V P6) + !P6 Λ (P7 V P8) + !P8 Λ (P9 V P2) [191].
Fig.2 Example of functions C(P1): a) C(P1) = 1;b) C(P1) = 1; c) C(P1) = 2; d) C(P1) = 3.
The Guo hall and Stentiford thinning algorithms use the function C(P)

https://github.com/arnaud-ramey/voronoi/blob/master/src/voronoi.h
https://web.archive.org/web/20160314104646/http://opencv-code.com/quick-tips/implementation-of-guo-hall-thinning-algorithm/

https://github.com/ulikoehler/cv_algorithms/blob/master/src/thinning.cpp

https://github.com/wuciawe/guo-hall-thinning/blob/master/thinning.h


https://github.com/LitLeo/Thinning_Soft_by_CUDA/blob/master/Zhang-Suen/OpenCV/main.cpp
