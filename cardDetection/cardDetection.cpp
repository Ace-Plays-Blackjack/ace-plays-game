#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error opening camera" << endl;
        return -1;
    }

    Mat template_images[13];
    string ranks[] = { "/home/jijonv/Card_Imgs/Ranks/ace.jpg", "/home/jijonv/Card_Imgs/Ranks/2.jpg","/home/jijonvCard_Imgs/Ranks/3.jpg" ,"/home/jijonv/Card_Imgs/Ranks/4.jpg" , "/home/jijonv/Card_Imgs/Ranks/5.jpg" , "/home/jijonv/Card_Imgs/Ranks/6.jpg" , "/home/jijonv/Card_Imgs/Ranks/7.jpg" , "/home/jijonv/Card_Imgs/Ranks/8.jpg" , "/home/jijonv/Card_Imgs/Ranks/9.jpg" , "/home/jijonv/Card_Imgs/Ranks/10.jpg" , "/home/jijonv/Card_Imgs/Ranks/jack.jpg" , "/home/jijonv/Card_Imgs/Ranks/queen.jpg" , "/home/jijonv/Card_Imgs/Ranks/king.jpg" };
    for (int i = 0; i < 13; i++) {
        template_images[i] = imread(ranks[i], IMREAD_GRAYSCALE);
        if (template_images[i].empty()) {
            cout << "Error loading template image for " << ranks[i] << endl;
            return -1;
        }
    }

    while (true) {
        Mat frame, frame_gray;
        cap >> frame;

        if (frame.empty()) {
            cout << "Error capturing frame" << endl;
            break;
        }

        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

        Mat binary_image;
        threshold(frame_gray, binary_image, 128, 255, THRESH_BINARY);

        vector<vector<Point>> contours;
        findContours(binary_image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < contours.size(); i++) {
            Rect bbox = boundingRect(contours[i]);

            if (bbox.width > 50 && bbox.height > 50) {
                Mat roi = binary_image(bbox);

                Mat roi_resized;
                resize(roi, roi_resized, Size(120, 180));

                string detected_rank = "Unknown";
                double min_similarity = 1.0;

                for (int j = 0; j < 13; j++) {
                    Mat diff_image;
                    absdiff(template_images[j], roi_resized, diff_image);

                    double similarity = sum(diff_image)[0] / (template_images[j].rows * template_images[j].cols * 255);
                    if (similarity < min_similarity) {
                        min_similarity = similarity;
                        detected_rank = ranks[j];
                    }
                }

                for (int j = 0; j < 13; j++) {
                    if (detected_rank == ranks[j]) {
                        Mat matched_template_image = template_images[j];
                        rectangle(frame, bbox, Scalar(0, 0, 255), 2);
                        putText(frame, detected_rank, Point(bbox.x, bbox.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
                        break;
                    }
                }
            }
        }

        imshow("Frame", frame);

        char c = waitKey(1);
        if (c == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}

