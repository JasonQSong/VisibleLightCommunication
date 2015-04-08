#include "stdafx.h"
using namespace std;

#define NOT_VALID_IMAGE 1

int main(int argc, char** argv)
{
  char cstr[MAX_PATH];
  int i;

  GetCurrentDirectoryA(MAX_PATH, cstr);
  string CurrentDirectory = cstr;
  string ImageFilename = CurrentDirectory + "\\images\\09.jpg";
  cv::Mat image;
  image = cv::imread(ImageFilename, cv::IMREAD_COLOR); // Read the file

  if (!image.data) // Check for invalid input
  {
    return NOT_VALID_IMAGE;
  }


  vlc::CameraInfo* Lumia1020 = new vlc::CameraInfo();
  Lumia1020->Focal = 5620;
  Lumia1020->RollingShutterRate = 1 / 47.54e3;

  vlc::CameraInfo* Camera = Lumia1020;

  vlc::RoomInfo* SJTULab = new vlc::RoomInfo();
  SJTULab->Transmitters[2000] = cv::Point3d(-5, 5, 0);
  SJTULab->Transmitters[2500] = cv::Point3d(5, 5, 0);
  SJTULab->Transmitters[3000] = cv::Point3d(5, -5, 0);
  SJTULab->Transmitters[3500] = cv::Point3d(-5, 5, 0);
  SJTULab->Transmitters[4000] = cv::Point3d(0, 0, 0);

  vlc::RoomInfo* Room = SJTULab;

  vector<double> Frequencies;
  for (map<double, cv::Point3d>::const_iterator iter = Room->Transmitters.begin(); iter != Room->Transmitters.end(); ++iter) {
    Frequencies.push_back(iter->first);
  }


  vlc::Processor::OpenFFT(image, Camera);



  cv::Mat imageForDisplay;
  cv::resize(image, imageForDisplay, cv::Size(300, 300));


  i = 0;
  for (map<double, cv::Point3d>::const_iterator iter = Room->Transmitters.begin(); iter != Room->Transmitters.end(); ++iter) {
    snprintf(cstr, MAX_PATH, "%0.1lf (%0.1lf,%0.1lf,%0.1lf)", iter->first, iter->second.x, iter->second.y, iter->second.z);
    cv::putText(imageForDisplay, cstr, cv::Point(0, (i + 1) * 20), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0, 255));
    i++;
  }


  cv::namedWindow("Display", cv::WINDOW_AUTOSIZE); // Create a window for display.
  //cv::resizeWindow("Display window", 200, 200);
  cv::moveWindow("Display", 0, 0);
  cv::imshow("Display", imageForDisplay); // Show our image inside it.


  cv::waitKey(0); // Wait for a keystroke in the window
  return 0;
}
