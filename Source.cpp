using namespace cv;

int main()
{
	int numBoards = 0;
	int numCornersHor;
	int numCornersVer;

	printf("Enter number of corners along width: "); // Taking in the values from the user
	scanf("%d", &numCornersHor);

	printf("Enter number of corners along height: ");
	scanf("%d", &numCornersVer);

	printf("Enter number of boards: ");
	scanf("%d", &numBoards);

	int numSquares = numCornersHor * numCornersVer;
	Size board_sz = Size(numCornersHor, numCornersVer);

	VideoCapture capture = VideoCapture(0); // We can use this to get live feed or we can remove it if we are working with static images only

	// A list of object points and image points
	vector<vector<Point3f>> object_points; // A physical position of the corners in 3D
	vector<vector<Point2f>> image_points; // Position of corners in 2D

	vector<Point2f> corners; // a list of corners
	int successes = 0;
	// Createing two images and taking a snapshot from camera
	Mat image; 
	Mat gray_image;
	capture >> image;

	vector<Point3f> obj;
	for (int j = 0; j<numSquares; j++) // Creates a list of coordinates corresponding to a particular vertex
		obj.push_back(Point3f(j / numCornersHor, j%numCornersHor, 0.0f));

	while (successes<numBoards)
	{
		cvtColor(image, gray_image, CV_BGR2GRAY);
		//Looks for corners on the chessboard
		bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS); 

		if (found) //refines the corners for more accuracy
		{
			cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray_image, board_sz, corners, found);
		}
		imshow("win1", image);
		imshow("win2", gray_image);

		capture >> image;
		int key = waitKey(1);

		if (key == 27) // quit the function if ecscape key is pressed

			return 0;

		if (key == ' ' && found != 0) // store the data if the spacebar is pressed
		{
			image_points.push_back(corners);
			object_points.push_back(obj);

			printf("Snap stored!");

			successes++;

			if (successes >= numBoards) // break the loop if we find the required number of snaps
				break;
		}
	}
	 
	