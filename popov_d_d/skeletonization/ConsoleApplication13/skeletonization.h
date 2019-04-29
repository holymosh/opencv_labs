#ifndef SKELETONIZATION_H
#define SKELETONIZATION_H
using namespace std;

struct Arch
{
	vector<CvPoint> points;
};

struct Skelet
{
	vector<Arch> arch;
};

struct TwoPoints
{
	CvPoint vertex;
	CvPoint neighbor;
};

void rotateMask(IplImage* mask1);
void fileOutput(char* s, IplImage* inputImage);
void improveImageQuality(IplImage* src, IplImage* dst);

vector<Skelet> getSkelets(IplImage* image);
bool findBlackPixelWithOneNeighbor(IplImage* tempImage, int& x_first_black_pixel, int& y_first_black_pixel);

void addVertexAndPixel(vector<TwoPoints>& current_vector, int x_vertex, int y_vertex, int x_neighbor, int y_neighbor);

IplImage* buildSkeleton(IplImage* inputImage)
{
	IplImage* outputImage = cvCreateImage(cvSize(inputImage->width, inputImage->height), IPL_DEPTH_8U, 1);
	outputImage = cvCloneImage(inputImage);
	IplImage* mask1 = cvCreateImage(cvSize(3, 3), IPL_DEPTH_8U, 1);
	uchar* mask1Ptr = (uchar*)(mask1->imageData + 0 * mask1->widthStep);
	mask1Ptr[0] = 255;
	mask1Ptr[1] = 255;
	mask1Ptr[2] = 255;
	mask1Ptr = (uchar*)(mask1->imageData + 1 * mask1->widthStep);
	mask1Ptr[0] = 128;
	mask1Ptr[1] = 0;
	mask1Ptr[2] = 128;
	mask1Ptr = (uchar*)(mask1->imageData + 2 * mask1->widthStep);
	mask1Ptr[0] = 0;
	mask1Ptr[1] = 0;
	mask1Ptr[2] = 0;
	IplImage* mask2 = cvCreateImage(cvSize(3, 3), IPL_DEPTH_8U, 1);
	uchar* mask2Ptr = (uchar*)(mask2->imageData + 0 * mask2->widthStep);
	mask2Ptr[0] = 128;
	mask2Ptr[1] = 255;
	mask2Ptr[2] = 255;
	mask2Ptr = (uchar*)(mask2->imageData + 1 * mask2->widthStep);
	mask2Ptr[0] = 0;
	mask2Ptr[1] = 0;
	mask2Ptr[2] = 255;
	mask2Ptr = (uchar*)(mask2->imageData + 2 * mask2->widthStep);
	mask2Ptr[0] = 128;
	mask2Ptr[1] = 0;
	mask2Ptr[2] = 128;

	for (int y = 0; y < (outputImage->height - 2); y++)
	{
		uchar* imagePtr0 = (uchar*)(outputImage->imageData + y * outputImage->widthStep);
		uchar* imagePtr1 = (uchar*)(outputImage->imageData + (y + 1) * outputImage->widthStep);
		uchar* imagePtr2 = (uchar*)(outputImage->imageData + (y + 2) * outputImage->widthStep);
		for (int x = 0; x < (outputImage->width - 2); x++)
		{
			for (int iter = 1; iter <= 4; iter++)
			{
				uchar* maskPtr0 = (uchar*)(mask1->imageData);
				uchar* maskPtr1 = (uchar*)(mask1->imageData + mask1->widthStep);
				uchar* maskPtr2 = (uchar*)(mask1->imageData + 2 * mask1->widthStep);
				if ((maskPtr0[0] == 128 || maskPtr0[0] == imagePtr0[x]) &&
					(maskPtr0[1] == 128 || maskPtr0[1] == imagePtr0[x + 1]) &&
					(maskPtr0[2] == 128 || maskPtr0[2] == imagePtr0[x + 2]) &&

					(maskPtr1[0] == 128 || maskPtr1[0] == imagePtr1[x]) &&
					(maskPtr1[1] == 128 || maskPtr1[1] == imagePtr1[x + 1]) &&
					(maskPtr1[2] == 128 || maskPtr1[2] == imagePtr1[x + 2]) &&

					(maskPtr2[0] == 128 || maskPtr2[0] == imagePtr2[x]) &&
					(maskPtr2[1] == 128 || maskPtr2[1] == imagePtr2[x + 1]) &&
					(maskPtr2[2] == 128 || maskPtr2[2] == imagePtr2[x + 2]))
				{
					imagePtr1[x + 1] = 255;
				}
				if (iter < 4)
				{
					rotateMask(mask1);
				}
			}
		}
		for (int x = 0; x < (outputImage->width - 2); x++)
		{
			for (int iter = 1; iter <= 4; iter++)
			{
				uchar* maskPtr0 = (uchar*)(mask2->imageData);
				uchar* maskPtr1 = (uchar*)(mask2->imageData + mask2->widthStep);
				uchar* maskPtr2 = (uchar*)(mask2->imageData + 2 * mask2->widthStep);
				if ((maskPtr0[0] == 128 || maskPtr0[0] == imagePtr0[x]) &&
					(maskPtr0[1] == 128 || maskPtr0[1] == imagePtr0[x + 1]) &&
					(maskPtr0[2] == 128 || maskPtr0[2] == imagePtr0[x + 2]) &&

					(maskPtr1[0] == 128 || maskPtr1[0] == imagePtr1[x]) &&
					(maskPtr1[1] == 128 || maskPtr1[1] == imagePtr1[x + 1]) &&
					(maskPtr1[2] == 128 || maskPtr1[2] == imagePtr1[x + 2]) &&

					(maskPtr2[0] == 128 || maskPtr2[0] == imagePtr2[x]) &&
					(maskPtr2[1] == 128 || maskPtr2[1] == imagePtr2[x + 1]) &&
					(maskPtr2[2] == 128 || maskPtr2[2] == imagePtr2[x + 2])
				)
				{
					imagePtr1[x + 1] = 255;
				}
				if (iter < 4)
				{
					rotateMask(mask2);
				}
			}
		}
	}

	cvReleaseImage(&mask1);
	cvReleaseImage(&mask2);

	return outputImage;
}

void rotateMask(IplImage* mask)
{
	uchar* mask1Ptr0 = (uchar*)(mask->imageData);
	uchar* mask1Ptr1 = (uchar*)(mask->imageData + mask->widthStep);
	uchar* mask1Ptr2 = (uchar*)(mask->imageData + 2 * mask->widthStep);

	uchar buf;
	buf = mask1Ptr0[2];
	mask1Ptr0[2] = mask1Ptr0[0];
	mask1Ptr0[0] = mask1Ptr2[0];
	mask1Ptr2[0] = mask1Ptr2[2];
	mask1Ptr2[2] = buf;
	buf = mask1Ptr0[1];
	mask1Ptr0[1] = mask1Ptr1[0];
	mask1Ptr1[0] = mask1Ptr2[1];
	mask1Ptr2[1] = mask1Ptr1[2];
	mask1Ptr1[2] = buf;
}

void fileOutput(char* s, IplImage* inputImage)
{
	FILE* f;
	fopen_s(&f, s, "w");
	char* temp = new char[5];
	for (int y = 0; y < inputImage->height; y++)
	{
		uchar* ptr = (uchar*)(inputImage->imageData + y * inputImage->widthStep);
		for (int x = 0; x < inputImage->width; x++)
		{
			_itoa_s(ptr[x], temp, 20, 10);
			fwrite(temp, sizeof(char), strlen(temp), f);
			fwrite("  ", sizeof(char), strlen("  "), f);
		}
		fwrite("\n", sizeof(char), strlen("\n"), f);
	}
	delete temp;
	fclose(f);
}

void improveImageQuality(IplImage* src, IplImage* dst)
{
	IplImage* hsv = cvCreateImage(CvSize(src->width, src->height), 8, 3);
	cvCvtColor(src, hsv, CV_BGR2HSV);

	int fmin = 255;
	int fmax = 0;
	for (int y = 0; y < hsv->height; y++)
	{
		uchar* ptr = (uchar*)(hsv->imageData + y * hsv->widthStep);
		for (int x = 0; x < hsv->width; x++)
		{
			if (ptr[3 * x + 2] < fmin)
			{
				fmin = ptr[3 * x + 2];
			}
			if (ptr[3 * x + 2] > fmax)
			{
				fmax = ptr[3 * x + 2];
			}
		}
	}
	int gmin = 0, gmax = 255;
	for (int y = 0; y < hsv->height; y++)
	{
		uchar* ptr = (uchar*)(hsv->imageData + y * hsv->widthStep);
		for (int x = 0; x < hsv->width; x++)
		{
			ptr[3 * x + 2] = (int)(
				(double)(
					(double)((ptr[3 * x + 2] - fmin)) /
					((double)(fmax - fmin))
				) * (gmax - gmin) +
				gmin);
		}
	}

	cvCvtColor(hsv, dst, CV_HSV2BGR);

	cvReleaseImage(&hsv);
}

vector<Skelet> getSkelets(IplImage* image)
{
	vector<Skelet> skelets;
	IplImage* tempImage = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
	tempImage = cvCloneImage(image);
	int x_first_black_pixel, y_first_black_pixel, x_current_pixel_plus_x, y_current_pixel_plus_y;

	while (findBlackPixelWithOneNeighbor(tempImage, x_first_black_pixel, y_first_black_pixel))
	{
		Skelet currentSkelet;
		vector<CvPoint> vertexMass;
		vector<TwoPoints> vertexStack;
		bool is_second_black_neighbor_found = false;
		do
		{
			int x_current_pixel = x_first_black_pixel, y_current_pixel = y_first_black_pixel;
			bool is_single_black_neighbor_found = false;

			vector<CvPoint> currentArch;
			do
			{
				if (vertexStack.size() != 0 && (is_single_black_neighbor_found == false))
				{
					TwoPoints current2Points;
					current2Points = vertexStack.back();
					vertexStack.pop_back();

					CvPoint vertex = current2Points.vertex, neighbor = current2Points.neighbor;

					currentArch.push_back(vertex);

					x_current_pixel = neighbor.x;
					y_current_pixel = neighbor.y;
				}

				CvPoint newPoint;
				newPoint.x = x_current_pixel;
				newPoint.y = y_current_pixel;
				currentArch.push_back(newPoint);

				uchar* ptr = (uchar*)(tempImage->imageData) + y_current_pixel * tempImage->widthStep;
				ptr[x_current_pixel] = 2;

				is_single_black_neighbor_found = false;
				is_second_black_neighbor_found = false;
				int x_neighbor, y_neighbor;
				for (int y = -1; y < 2; y++)
				{
					y_current_pixel_plus_y = y_current_pixel + y;
					uchar* ptr = (uchar*)(tempImage->imageData) + y_current_pixel_plus_y * tempImage->widthStep;
					for (int x = -1; x < 2; x++)
					{
						x_current_pixel_plus_x = x_current_pixel + x;
						if ((x != 0 || y != 0)
							&& ((0 <= y_current_pixel_plus_y) && (y_current_pixel_plus_y < tempImage->height) &&
								(0 <= x_current_pixel_plus_x) && (x_current_pixel_plus_x < tempImage->width))
						)
						{
							if (ptr[x_current_pixel_plus_x] == 0)
							{
								if ((is_single_black_neighbor_found == false) && (is_second_black_neighbor_found ==
									false))
								{
									x_neighbor = x_current_pixel_plus_x;
									y_neighbor = y_current_pixel_plus_y;
									is_single_black_neighbor_found = true;
								}
								else
								{
									if (is_single_black_neighbor_found == true)
									{
										addVertexAndPixel(vertexStack, x_current_pixel, y_current_pixel, x_neighbor,
										                  y_neighbor);
										is_second_black_neighbor_found = true;

										uchar* cur_ptr = (uchar*)(tempImage->imageData) + y_neighbor * tempImage->
											widthStep;
										cur_ptr[x_neighbor] = 3;

										is_single_black_neighbor_found = false;
									}

									addVertexAndPixel(vertexStack, x_current_pixel, y_current_pixel,
									                  x_current_pixel_plus_x, y_current_pixel_plus_y);

									uchar* cur_ptr = (uchar*)(tempImage->imageData) + y_current_pixel_plus_y * tempImage
										->widthStep;
									cur_ptr[x_current_pixel_plus_x] = 3;
								}
							}
						}
					}
				}

				if (is_single_black_neighbor_found == true)
				{
					x_current_pixel = x_neighbor;
					y_current_pixel = y_neighbor;
				}
				else
				{
					CvPoint newVertex;
					newVertex.x = x_current_pixel;
					newVertex.y = y_current_pixel;
					vertexMass.push_back(newVertex);
				}
			}
			while (is_single_black_neighbor_found == true);

			Arch newArch;
			newArch.points = currentArch;
			currentSkelet.arch.push_back(newArch);
		}
		while (vertexStack.size() != 0);
		skelets.push_back(currentSkelet);
	}
	cvReleaseImage(&tempImage);
	return skelets;
}

bool findBlackPixelWithOneNeighbor(IplImage* tempImage, int& x_first_black_pixel, int& y_first_black_pixel)
{
	bool black_pixel_is_found = false, second_black_pixel_is_found = false, third_black_pixel_is_found = false;
	uchar* ptr = (uchar*)(tempImage->imageData);
	uchar* ptr2 = (uchar*)(tempImage->imageData) + tempImage->widthStep;
	for (int x = 0; (x < tempImage->width) && (black_pixel_is_found == false); x++)
	{
		if (ptr[x] == 0)
		{
			for (int i = -1; (i < 2) && (third_black_pixel_is_found == false); i++)
			{
				if (
					((x + i) != -1) &&
					((x + i) != tempImage->width))
				{
					if (ptr[x + i] == 0 && i != 0)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = 0;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}

					if (ptr2[x + i] == 0 && third_black_pixel_is_found == false)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = 0;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
				}
			}
			second_black_pixel_is_found = false;
			third_black_pixel_is_found = false;
		}
	}

	for (int y = 1; (y < (tempImage->height - 1)) && (black_pixel_is_found == false); y++)
	{
		uchar* ptr1 = (uchar*)(tempImage->imageData) + (y - 1) * tempImage->widthStep;
		uchar* ptr2 = (uchar*)(tempImage->imageData) + y * tempImage->widthStep;
		uchar* ptr3 = (uchar*)(tempImage->imageData) + (y + 1) * tempImage->widthStep;
		{
			if (ptr2[0] == 0)
			{
				for (int x = 0; (x < 2) && (third_black_pixel_is_found == false); x++)
				{
					if (ptr1[x] == 0)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = 0;
							y_first_black_pixel = y;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
					if (x != 0 && ptr2[x] == 0 && third_black_pixel_is_found == false)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = 0;
							y_first_black_pixel = y;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
					if (ptr3[x] == 0 && third_black_pixel_is_found == false)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = 0;
							y_first_black_pixel = y;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
				}
				second_black_pixel_is_found = false;
				third_black_pixel_is_found = false;
			}
		}
	}
	for (int y = 1; (y < (tempImage->height - 1)) && (black_pixel_is_found == false); y++)
	{
		uchar* ptr1 = (uchar*)(tempImage->imageData) + (y - 1) * tempImage->widthStep;
		uchar* ptr2 = (uchar*)(tempImage->imageData) + y * tempImage->widthStep;
		uchar* ptr3 = (uchar*)(tempImage->imageData) + (y + 1) * tempImage->widthStep;
		{
			if (ptr2[tempImage->width - 1] == 0)
			{
				for (int x = tempImage->width - 2; (x < tempImage->width) && (second_black_pixel_is_found == false); x++
				)
				{
					if (ptr1[x] == 0)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = tempImage->width - 1;
							y_first_black_pixel = y;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
					if (x != 0 && ptr2[x] == 0 && third_black_pixel_is_found == false)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = tempImage->width - 1;
							y_first_black_pixel = y;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
					if (ptr3[x] == 0 && third_black_pixel_is_found == false)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = tempImage->width - 1;
							y_first_black_pixel = y;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
				}
				second_black_pixel_is_found = false;
				third_black_pixel_is_found = false;
			}
		}
	}
	if (black_pixel_is_found == false)
	{
		uchar* ptr = (uchar*)(tempImage->imageData) + (tempImage->height - 2) * tempImage->widthStep;
		uchar* ptr2 = (uchar*)(tempImage->imageData) + (tempImage->height - 1) * tempImage->widthStep;
		for (int x = 0; (x < tempImage->width) && (black_pixel_is_found == false); x++)
		{
			if (ptr2[x] == 0)
			{
				for (int i = -1; (i < 2) && (third_black_pixel_is_found == false); i++)
				{
					if (
						((x + i) != -1) &&
						((x + i) != tempImage->width))
					{
						if (ptr[x + i] == 0)
						{
							if (second_black_pixel_is_found == false)
							{
								black_pixel_is_found = true;
								second_black_pixel_is_found = true;
								x_first_black_pixel = x;
								y_first_black_pixel = tempImage->width - 1;
							}
							else
							{
								black_pixel_is_found = false;
								second_black_pixel_is_found = false;
								third_black_pixel_is_found = true;
							}
						}

						if (i != 0 && ptr2[x + i] == 0 && third_black_pixel_is_found == false)
						{
							if (second_black_pixel_is_found == false)
							{
								black_pixel_is_found = true;
								second_black_pixel_is_found = true;
								x_first_black_pixel = x;
								y_first_black_pixel = tempImage->width - 1;
							}
							else
							{
								black_pixel_is_found = false;
								second_black_pixel_is_found = false;
								third_black_pixel_is_found = true;
							}
						}
					}
				}
				second_black_pixel_is_found = false;
				third_black_pixel_is_found = false;
			}
		}
	}
	for (int y = 1; (y < (tempImage->height - 2)) && (black_pixel_is_found == false); y++)
	{
		uchar* ptr1 = (uchar*)(tempImage->imageData) + (y - 1) * tempImage->widthStep;
		uchar* ptr2 = (uchar*)(tempImage->imageData) + y * tempImage->widthStep;
		uchar* ptr3 = (uchar*)(tempImage->imageData) + (y + 1) * tempImage->widthStep;
		for (int x = 1; x < (tempImage->width - 1) && (black_pixel_is_found == false); x++)
		{
			if (ptr2[x] == 0)
			{
				for (int w = -1; (w < 2) && (third_black_pixel_is_found == false); w++)
				{
					if (ptr1[x + w] == 0)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = y;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
					if (w != 0 && ptr2[x + w] == 0 && third_black_pixel_is_found == false)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = y;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
					if (ptr3[x + w] == 0 && third_black_pixel_is_found == false)
					{
						if (second_black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							second_black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = y;
						}
						else
						{
							black_pixel_is_found = false;
							second_black_pixel_is_found = false;
							third_black_pixel_is_found = true;
						}
					}
				}
				second_black_pixel_is_found = false;
				third_black_pixel_is_found = false;
			}
		}
	}

	ptr = (uchar*)(tempImage->imageData);
	ptr2 = (uchar*)(tempImage->imageData + tempImage->widthStep);
	for (int x = 0; (x < tempImage->width) && (black_pixel_is_found == false); x++)
	{
		if (ptr[x] == 255)
		{
			for (int i = -1; (i < 2) && (second_black_pixel_is_found == false); i++)
			{
				if (
					((x + i) != -1) &&
					((x + i) != tempImage->width)
				)
				{
					if (ptr[x + i] == 0)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x + i;
							y_first_black_pixel = 0;
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}

					if (ptr2[x + i] == 0 && second_black_pixel_is_found == false)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x + i;
							y_first_black_pixel = 1;
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
				}
			}
			second_black_pixel_is_found = false;
		}
	}

	for (int y = 1; (y < (tempImage->height - 1)) && (black_pixel_is_found == false); y++)
	{
		uchar* ptr1 = (uchar*)(tempImage->imageData) + (y - 1) * tempImage->widthStep;
		uchar* ptr2 = (uchar*)(tempImage->imageData) + y * tempImage->widthStep;
		uchar* ptr3 = (uchar*)(tempImage->imageData) + (y + 1) * tempImage->widthStep;
		{
			if (ptr2[0] == 255)
			{
				for (int x = 0; (x < 2) && (second_black_pixel_is_found == false); x++)
				{
					if (ptr1[x] == 0)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = y - 1;
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
					if (ptr2[x] == 0 && second_black_pixel_is_found == false)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = y;
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
					if (ptr3[x] == 0 && second_black_pixel_is_found == false)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = y + 1;
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
				}
				second_black_pixel_is_found = false;
			}
		}
	}

	for (int y = 1; (y < (tempImage->height - 1)) && (black_pixel_is_found == false); y++)
	{
		uchar* ptr1 = (uchar*)(tempImage->imageData) + (y - 1) * tempImage->widthStep;
		uchar* ptr2 = (uchar*)(tempImage->imageData) + y * tempImage->widthStep;
		uchar* ptr3 = (uchar*)(tempImage->imageData) + (y + 1) * tempImage->widthStep;
		{
			if (ptr2[tempImage->width - 1] == 255)
			{
				for (int x = tempImage->width - 2; (x < tempImage->width) && (second_black_pixel_is_found == false); x++
				)
				{
					if (ptr1[x] == 0)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = y - 1;
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
					if (ptr2[x] == 0 && second_black_pixel_is_found == false)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = y;
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
					if (ptr3[x] == 0 && second_black_pixel_is_found == false)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x;
							y_first_black_pixel = y + 1;
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
				}
				second_black_pixel_is_found = false;
			}
		}
	}
	if (black_pixel_is_found == false)
	{
		uchar* ptr = (uchar*)(tempImage->imageData) + (tempImage->width - 2) * tempImage->widthStep;
		uchar* ptr2 = (uchar*)(tempImage->imageData) + (tempImage->width - 1) * tempImage->widthStep;
		for (int x = 0; (x < tempImage->width) && (black_pixel_is_found == false); x++)
		{
			if (ptr2[x] == 255)
			{
				for (int i = -1; (i < 2) && (second_black_pixel_is_found == false); i++)
				{
					if (
						((x + i) != -1) &&
						((x + i) != tempImage->width)
					)
					{
						if (ptr[x + i] == 0)
						{
							if (black_pixel_is_found == false)
							{
								black_pixel_is_found = true;
								x_first_black_pixel = x + i;
								y_first_black_pixel = tempImage->width - 2;
							}
							else
							{
								second_black_pixel_is_found = true;
								black_pixel_is_found = false;
							}
						}

						if (ptr2[x + i] == 0 && second_black_pixel_is_found == false)
						{
							if (black_pixel_is_found == false)
							{
								black_pixel_is_found = true;
								x_first_black_pixel = x + i;
								y_first_black_pixel = tempImage->width - 1;
							}
							else
							{
								second_black_pixel_is_found = true;
								black_pixel_is_found = false;
							}
						}
					}
				}
				second_black_pixel_is_found = false;
			}
		}
	}

	for (int y = 1; (y < (tempImage->height - 1)) && (black_pixel_is_found == false); y++)
	{
		uchar* ptr1 = (uchar*)(tempImage->imageData) + (y - 1) * tempImage->widthStep;
		uchar* ptr2 = (uchar*)(tempImage->imageData) + y * tempImage->widthStep;
		uchar* ptr3 = (uchar*)(tempImage->imageData) + (y + 1) * tempImage->widthStep;
		for (int x = 1; x < (tempImage->width - 1) && (black_pixel_is_found == false); x++)
		{
			if (ptr2[x] == 255)
			{
				for (int w = -1; (w < 2) && (second_black_pixel_is_found == false); w++)
				{
					if (ptr1[x + w] == 0)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x + w;
							y_first_black_pixel = (y - 1);
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
					if (ptr2[x + w] == 0 && second_black_pixel_is_found == false)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x + w;
							y_first_black_pixel = y;
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
					if (ptr3[x + w] == 0 && second_black_pixel_is_found == false)
					{
						if (black_pixel_is_found == false)
						{
							black_pixel_is_found = true;
							x_first_black_pixel = x + w;
							y_first_black_pixel = (y + 1);
						}
						else
						{
							second_black_pixel_is_found = true;
							black_pixel_is_found = false;
						}
					}
				}
				second_black_pixel_is_found = false;
			}
		}
	}
	return black_pixel_is_found;
}

void addVertexAndPixel(vector<TwoPoints>& current_vector, int x_vertex, int y_vertex, int x_neighbor, int y_neighbor)
{
	TwoPoints newDoublePoint;

	CvPoint newPoint;
	newPoint.x = x_vertex;
	newPoint.y = y_vertex;
	newDoublePoint.vertex = newPoint;

	newPoint.x = x_neighbor;
	newPoint.y = y_neighbor;
	newDoublePoint.neighbor = newPoint;

	current_vector.push_back(newDoublePoint);
}

#endif //SKELETONIZATION_H
