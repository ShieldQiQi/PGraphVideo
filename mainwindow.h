#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTextStream *textStream;
    QString *dataStr;
    QFile *file;

    QTimer *timer;
    void Video_to_Image(std::string filename);

    void cannyEdgeFliter(std::vector<Mat> &Images, Mat &DstImage);
    void sobelEdgeFliter(std::vector<Mat> &Images, Mat &DstImage);
    void laplacianEdgeFliter(std::vector<Mat> &Images, Mat &DstImage);
    void getContours(std::vector<Mat> &Images, Mat &DstImage);

public slots:
    void PlayImages();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
