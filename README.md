# Histogram-Equalisation_OpenMP
Implemented Histogram Equalization on Images using OpenMP Parallel Programming

Steps to Run the given source code:
1. In the given OS, install openCV and create parallelizing environment. I installed Ubuntu 20.04 in Oracle Virtual box and installed openCV using cmake tool in Ubuntu installin certain dependencies (The instruction text file also attached). 
2. Open the attached source code and edit it for the path of the image to be analyzed (directly using text editor or using gedit 19EC10052_hp3_openmp_assignment1.cpp)
3. Compile the source code using the following command:  g++ -fopenmp 19EC10052_hp3_openmp_assignment1.cpp -o output `pkg-config --cflags --libs opencv4` 
4. Run the code using the command: output.
5. The console asks for the number of threads to be executed in parallel region using openMP eg..1,2,4
6. The program runs and displays the time taken to run, the given image name to indicate that image is displayed (eg..ResImage Displayed sample3) and the input and resized versions of the histogram equalized images as new window pop ups such that both the images are scaled same in both directions (i.e., pixel ratio remains same to fit the screen)
7. Also, note that the png versions of the histogram equalized images will be stored at the path of the code with same dimensions as that of the input image.
8. For better realization of histogram equalization than the images from console popping up, open the stored source location where the new output image will be stored. Open the input and output image simultaneously in some photo viewing app/software to realize the effect of histogram equalization more effectively.


Link to the sample test case is here: <https://drive.google.com/drive/folders/1GX9yTku7FKfUywVSz0eLa0SxCtXcEbr3?usp=sharing>



