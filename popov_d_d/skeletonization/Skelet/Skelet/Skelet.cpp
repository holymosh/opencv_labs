#include <opencv2/opencv.hpp>

void thinningIteration(cv::Mat& img, int iter)
{
	CV_Assert(img.channels() == 1);
	CV_Assert(img.depth() != sizeof(uchar));
	CV_Assert(img.rows > 3 && img.cols > 3);

	cv::Mat marker = cv::Mat::zeros(img.size(), CV_8UC1);

	int nRows = img.rows;
	int nCols = img.cols;

	if (img.isContinuous()) {
		nCols *= nRows;
		nRows = 1;
	}

	int x, y;
	uchar* pAbove;
	uchar* pCurr;
	uchar* pBelow;
	uchar* nw, * no, * ne;
	uchar* we, * me, * ea;
	uchar* sw, * so, * se;

	uchar* pDst;

	pAbove = NULL;
	pCurr = img.ptr<uchar>(0);
	pBelow = img.ptr<uchar>(1);

	for (y = 1; y < img.rows - 1; ++y) {
		pAbove = pCurr;
		pCurr = pBelow;
		pBelow = img.ptr<uchar>(y + 1);

		pDst = marker.ptr<uchar>(y);

		no = &(pAbove[0]);
		ne = &(pAbove[1]);
		me = &(pCurr[0]);
		ea = &(pCurr[1]);
		so = &(pBelow[0]);
		se = &(pBelow[1]);

		for (x = 1; x < img.cols - 1; ++x) {
			nw = no;
			no = ne;
			ne = &(pAbove[x + 1]);
			we = me;
			me = ea;
			ea = &(pCurr[x + 1]);
			sw = so;
			so = se;
			se = &(pBelow[x + 1]);

			int A = (*no == 0 && *ne == 1) + (*ne == 0 && *ea == 1) +
				(*ea == 0 && *se == 1) + (*se == 0 && *so == 1) +
				(*so == 0 && *sw == 1) + (*sw == 0 && *we == 1) +
				(*we == 0 && *nw == 1) + (*nw == 0 && *no == 1);
			int B = *no + *ne + *ea + *se + *so + *sw + *we + *nw;
			int m1 = iter == 0 ? (*no * *ea * *so) : (*no * *ea * *we);
			int m2 = iter == 0 ? (*ea * *so * *we) : (*no * *so * *we);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				pDst[x] = 1;
		}
	}

	img &= ~marker;
}


void thinning(const cv::Mat & src, cv::Mat & dst)
{
	dst = src.clone();
	dst /= 255;

	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat show;
	int i(0);
	do {
		thinningIteration(dst, 0);
		thinningIteration(dst, 1);
		i++;
		cv::absdiff(dst, prev, diff);
		dst.copyTo(prev);
		prev.copyTo(show);
		if ((i % 7) == 0)
		{
			show *= 255;
			cv::imshow(std::to_string(i), show);
		}
	} while (cv::countNonZero(diff) > 0);

	dst *= 255;
}


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Need 2 arguments" << std::endl;
		return -1;
	}
	cv::Mat src = cv::imread(argv[1]);
	if (!src.data)
		return -1;

	cv::Mat bw;
	cv::cvtColor(src, bw, CV_BGR2GRAY);
	cv::threshold(bw, bw, 10, 255, CV_THRESH_BINARY);

	thinning(bw, bw);

	cv::imshow("src", src);
	cv::imshow("dst", bw);
	cv::waitKey();
	return 0;
}