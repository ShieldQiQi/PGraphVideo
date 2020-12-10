#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string filename = "E:/PGraphVideo/video/jilejingtu.mp4";
//    Video_to_Image(filename);
//    waitKey(0);

    textStream = new QTextStream;
    dataStr = new QString;
    file = new QFile("E:/PGraphVideo/pointCsv/pointXY.csv");
    if(!file->open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
//        QMessageBox::information(this,"","E:/PGraphVideo/pointCsv/pointXY.csv",NULL);
    }else{
        textStream->setDevice(file);
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(PlayImages()));
    timer->start(100);

}
void MainWindow::PlayImages()
{
    static int i = 0;
    std::string filePath = "E:/PGraphVideo/image/";
    std::vector<Mat>Images(2);
    Mat DstImage;

    if(i == 0){
        namedWindow( "極楽浄土", WINDOW_NORMAL );
        resizeWindow("極楽浄土",1500,800);
        moveWindow("極楽浄土",210,20);

        Images[0]=imread(filePath+"frame_1.png",1);
//        cannyEdgeFliter(Images,DstImage);
//        sobelEdgeFliter(Images,DstImage);
//        laplacianEdgeFliter(Images,DstImage);
        getContours(Images,DstImage);

        imshow( "極楽浄土", DstImage );
        i++;
    }else if( i < 675) {
        Images[0] = imread(filePath+"frame_"+std::to_string(i)+".png",1);
        if (!Images[0].empty())
        {
//            cannyEdgeFliter(Images,DstImage);
//            sobelEdgeFliter(Images,DstImage);
//            laplacianEdgeFliter(Images,DstImage);
            getContours(Images,DstImage);

            imshow( "極楽浄土", DstImage );
        }
        i++;
    }else{
        destroyAllWindows();
    }
}

void MainWindow::getContours(std::vector<Mat> &Images, Mat &DstImage)
{
//    Mat grad_x, grad_y;
//    Mat abs_grad_x, abs_grad_y;

//    // get the grad-x
//    Sobel(Images[0], grad_x, CV_16S, 1, 0, 3, 0.6, 0, BORDER_DEFAULT);
//    convertScaleAbs(grad_x, abs_grad_x);

//    // get the grad-y
//    Sobel(Images[0], grad_y, CV_16S, 0, 1, 3, 0.6, 0, BORDER_DEFAULT);
//    convertScaleAbs(grad_y, abs_grad_y);

//    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, Images[1]);

    Images[1] = Images[0];
    cvtColor(Images[1], Images[1], COLOR_RGB2GRAY);
    threshold(Images[1],Images[1], 198, 255, THRESH_BINARY_INV);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<Vec4i> hierarchy;
    findContours(Images[1], contours, hierarchy, RETR_CCOMP,CHAIN_APPROX_SIMPLE);


    Mat contoursImage(Images[1].rows,Images[1].cols,CV_8U,Scalar(255));
//    std::cout<<contours.size()<<std::endl;
    for(int i = 0;i < contours.size();i++){
//           if(hierarchy[i][3] == -1 && hierarchy[i][2] != -1)
               drawContours(contoursImage,contours,i,Scalar(0),3);
    }
    Images[1] = contoursImage;

    int count = 0;
    for (short i = 0;i<Images[1].rows;i++) {
        for (short j = 0;j<Images[1].cols;j++) {
            if(Images[1].ptr<uchar>(i)[j] == 0 && j > 0 && j < Images[1].cols-1 &&
                    i > 0 && i < Images[1].rows-1 &&
                    ((Images[1].ptr<uchar>(i)[j-1] != 0 && Images[1].ptr<uchar>(i)[j+1] != 0) ||
                    (Images[1].ptr<uchar>(i-1)[j] != 0 && Images[1].ptr<uchar>(i+1)[j] != 0))){
                *dataStr += (QString::number(i)+","+ QString::number(j)+",");
                count ++;
            }else{
                Images[1].at<uchar>(i, j) = 255;
            }
        }
    }
    std::cout<<count<<std::endl;
    *dataStr += "\n";
    *textStream << *dataStr;
    dataStr->clear();

    cvtColor(Images[1], Images[1], COLOR_GRAY2RGB);
    hconcat(Images,DstImage);
}
void MainWindow::cannyEdgeFliter(std::vector<Mat> &Images, Mat &DstImage)
{
    Mat edge, grayImage;

    cvtColor(Images[0], grayImage, COLOR_BGR2GRAY);
    blur(grayImage, edge, Size(3, 3));
    Canny(edge, edge, 3, 9, 3);
    cvtColor(edge, Images[1], COLOR_GRAY2RGB);

    hconcat(Images,DstImage);
}
void MainWindow::sobelEdgeFliter(std::vector<Mat> &Images, Mat &DstImage)
{
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    // get the grad-x
    Sobel(Images[0], grad_x, CV_16S, 1, 0, 3, 0.6, 0, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);

    // get the grad-y
    Sobel(Images[0], grad_y, CV_16S, 0, 1, 3, 0.6, 0, BORDER_DEFAULT);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, Images[1]);

    cvtColor(Images[1], Images[1], COLOR_RGB2GRAY);
    threshold(Images[1],Images[1], 10, 255, THRESH_BINARY_INV);

    for (short i = 0;i<Images[1].rows;i++) {
        for (short j = 0;j<Images[1].cols;j++) {
            if(Images[1].ptr<uchar>(i)[j] == 0){
                *dataStr += (QString::number(i)+","+ QString::number(j)+",");
            }else{
                Images[1].at<uchar>(i, j) = 255;
            }
        }
    }
    *dataStr += "\n";
    *textStream << *dataStr;
    dataStr->clear();

    cvtColor(Images[1], Images[1], COLOR_GRAY2RGB);
    hconcat(Images,DstImage);
}
void MainWindow::laplacianEdgeFliter(std::vector<Mat> &Images, Mat &DstImage)
{
    Mat grayImage;

    GaussianBlur(Images[0], Images[0], Size(3, 3), 0, 0, BORDER_DEFAULT);
    cvtColor(Images[0], grayImage, COLOR_RGB2GRAY);

    Laplacian(grayImage, Images[1], CV_16S, 3, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(Images[1], Images[1]);

    threshold(Images[1],Images[1], 10, 255, THRESH_BINARY_INV);
    cvtColor(Images[1], Images[1], COLOR_GRAY2RGB);

    hconcat(Images,DstImage);
}

void MainWindow::Video_to_Image(std::string filename)
{
    VideoCapture capture(filename);

    if (capture.isOpened())
    {
        std::cout<<"load video successfully"<<std::endl;
        //    int frame_width = (int)capture.get(CAP_PROP_FRAME_WIDTH);
        //    int frame_height = (int)capture.get(CAP_PROP_FRAME_HEIGHT);
        long frame_number = capture.get(CAP_PROP_FRAME_COUNT);
        std::cout<<"the total frame number is: "<<frame_number<<std::endl;
        // set the first frame number
        capture.set(CAP_PROP_POS_FRAMES, 1);

        Mat frame;
        std::string image_name[frame_number];
        for(int i = 0;i<frame_number;i++)
        {
            if(i%10 == 0)
            {
                if (!capture.read(frame))
                {
                    std::cout<<"Failed to read frame"<<std::endl;
                }
                image_name[i] = "E:/PGraphVideo/image/frame_"+std::to_string(i/10+1)+".png";
                imwrite(image_name[i], frame);
            }
        }
    }else{
        std::cout<<"Failed to load video"<<std::endl;
    }
    capture.release();
}

MainWindow::~MainWindow()
{
    delete file;
    delete timer;
    delete ui;
    delete textStream;
    delete dataStr;
}

