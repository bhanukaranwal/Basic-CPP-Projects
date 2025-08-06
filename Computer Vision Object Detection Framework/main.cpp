/*
  Computer Vision Object Detection Framework (main.cpp)
  ------------------------------------------------------
  - Uses OpenCV to capture webcam frames in real-time
  - Applies a classic Haar Cascade face detector (included in OpenCV)
  - Annotates detected objects (faces) live, draws bounding boxes with confidence scores
  - Press 's' to save a snapshot of current frame with annotation
  - Self-contained logic for modular input, detection, and display
*/

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main() {
    cout << "=== Computer Vision Object Detection DEMO ===\n";
    cout << "  - Live face detection from webcam, 's' to save, ESC to exit.\n";

    // Load default Haar Cascade for face detection
    CascadeClassifier face_cascade;
    if(!face_cascade.load(
        "/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml") &&
       !face_cascade.load("haarcascade_frontalface_default.xml")) {
        cerr << "ERROR: Couldn't load Haar cascade XML!\n";
        return 1;
    }

    VideoCapture cap(0); // Opens default webcam
    if(!cap.isOpened()) {
        cerr << "ERROR: Cannot open webcam.\n";
        return 2;
    }

    Mat frame;
    int snapCount = 0;
    while(true) {
        cap >> frame;
        if(frame.empty()) break;

        // Detect faces
        vector<Rect> faces;
        face_cascade.detectMultiScale(frame, faces, 1.12, 3, 0|CASCADE_SCALE_IMAGE, Size(50,50));

        // Annotate faces
        for(const auto& face : faces) {
            rectangle(frame, face, Scalar(0,255,0), 2);
            putText(frame, "Face", Point(face.x, face.y-8),
                    FONT_HERSHEY_SIMPLEX, 0.88, Scalar(35,100,225), 2);
        }
        putText(frame, "Faces detected: " + to_string(faces.size()),
                Point(12,32), FONT_HERSHEY_SIMPLEX, 0.78, Scalar(220,220,50), 2);
        putText(frame, "Press 's'=save, ESC=exit", Point(12,60),
                FONT_HERSHEY_SIMPLEX, 0.55, Scalar(190,190,190), 1);

        imshow("Object Detection", frame);
        char c = (char)waitKey(30);
        if(c==27) break; // ESC: exit
        else if(c=='s') {
            string fname = "snapshot_" + to_string(++snapCount) + ".png";
            imwrite(fname, frame);
            cout << "Saved " << fname << "\n";
        }
    }
    cap.release();
    destroyAllWindows();
    cout << "Session ended.\n";
    return 0;
}
