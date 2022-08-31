/*	Rekha Lokesh
	19EC10052
*/
	
	
#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<omp.h>
#include<string>

using namespace std;
using namespace cv;

int num_threads = 4;	//number of threads

string getFileName(const string&);	//function to getfilename

int main()
{
	cout<<"Enter the number of threads: ";
	cin>>num_threads;
	omp_set_num_threads(num_threads);	//setting the number of threads for parallelized part

	double st = omp_get_wtime();//start timer

	string img_path = "sample1.png";	//Path to the image (absolute or relative)

	Mat img = imread(img_path, CV_8UC1);	//Reading the image as each pixel having intensity value from 0 to (2^8-1) (unsigned 8 bit Single Channel)
	
	if(img.empty())
	{
		cout<<"Can't Load Image"<<endl;
	}

	Mat resImg(img);	//Creating a new image same as input image

	int L = 256; //maximum intensity value of a pixel

	int img_height = img.rows;	//Number of rows of the image
	int img_width = img.cols;	//Number of columns of image

	long int tot = (img_width * img_height);	//Total number of pixels of the image

	cout << "Image dimesions: " << img_width << " x " << img_height << endl;

	//Dynamically allocating the arrays to store the histogram, s and p arrays
	int* hist = new int[L];
	double* s = new double[L];
	double* p = new double[L];

	int i, j;
	#pragma omp parallel
		{
		#pragma omp for private (i)
			for (i = 0; i < L; i++)	hist[i] = 0, s[i] = 0.0, p[i] = 0.0;

		#pragma omp barrier	//to synchronise the threads so that above operation gets finished by all

		#pragma omp for collapse(2) private(i, j) reduction(+:hist[0:(L-1)])
			for (i = 0; i < img_height; i++)
			{
				for (j = 0; j < img_width; j++)
				{
					int pixel_val = int(img.at<unsigned char>(i, j)); //the unsigned pixel gray value of the image
						hist[pixel_val]++;
				}
			}

		#pragma omp barrier	//to synchronise the threads so that above operation gets finished by all

		#pragma omp for private(i)
			for (i = 0; i < L; i++)
			{
				p[i] = double(hist[i]) / tot;
			}

			//Histogram Equalization or Histogram Linear Transformation 
			//This part can be done only by one thread

		#pragma omp single
			{
				s[0] = (L - 1) * p[0];
				for (int i = 1; i < L; i++)
				{
					s[i] = s[i - 1] + (L - 1) * p[i];
				}
			}

			/*
			cout << "Intensity distribution and histogram values (rk, nk and pk): " << endl;
			cout << "rk   nk   pk" << endl;
			for (int i = 0; i < 256; i++)
			{
				cout << i << " " << cnt[i] << " " << prob[i] << endl;
			}
			cout << "\n";

			cout << "Transformation Function (sk): " << endl;
			for (int i = 0; i < 256; i++)
			{
				cout << s[i] << " ";
			}
			cout << "\n";
			*/

			//Replacing each intensity value r_k with its corresponding s_k in the new image
			
		#pragma omp barrier	//to synchronise the threads so that above operation gets finished by all

		#pragma omp for collapse(2) private(i, j)
			for (i = 0; i < img_height; i++)
			{
				for (j = 0; j < img_width; j++)
				{
					int pixel_val = int(img.at<unsigned char>(i, j));
					int k = round(s[pixel_val]);	//k is the mapped intensity value of val
					resImg.at<unsigned char>(i, j) = k;
				}
			}
		}

	double end = omp_get_wtime(); //sequential execution time

	//freeing the memory
	delete[] hist;
	delete[] s;
	delete[] p;

	cout << "Time for Parallel execution with threads = " << num_threads << ": " << (end - st) << endl;

	string filename = getFileName(img_path);
	if (img_width < 1800 && img_height < 900)
	{
		imshow("Input Image", img);	//Shows the Input Image since large it gets compressed
		imshow("Output Image", resImg);
	}
	else
	{
		//Since the input and output images are large, their resized versions are being shown
		float rath = img_width / 1900, ratv = img_height / 1000;
		float maxim = (rath > ratv) ? rath : ratv;
		Mat img_rsz;
		resize(img, img_rsz, Size(), 1 / maxim, 1 / maxim);
		imshow("Resized Input Image", img_rsz);
		Mat resImg_rsz;
		resize(resImg, resImg_rsz, Size(), 1 / maxim, 1 / maxim);
		imshow("Resized output image", resImg_rsz);
	}
	cout << "ResImage Displayed " + filename << endl;
	imwrite(filename + "_" + to_string(num_threads) + " thread_Parallelized Histogram Equalized Image.png", resImg);	//Saving the Resized Image at the current location
	waitKey(0);

	return 0;
}

string getFileName(const string& s) {

	char sep = '/';
	size_t i = s.rfind(sep, s.length());
	if (i != string::npos) {
		if (s.substr(s.length() - 4, 4) == ".png")
			return(s.substr(i + 1, s.length() - (i + 4)));
		else
			return(s.substr(i + 1, s.length() - (i)));
	}
	if (s.substr(s.length() - 4, 4) == ".png")
		return(s.substr(0, s.length() - 4));
	else
		return(s.substr(0, s.length()));
}


