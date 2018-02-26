#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

#define DIVISOR 4

image_transport::Publisher pub;


  void readImage(const sensor_msgs::ImageConstPtr& old_image, cv::Mat &image)
  {
    cv_bridge::CvImageConstPtr pCvImage;
    pCvImage = cv_bridge::toCvShare(old_image, old_image->encoding);
    pCvImage->image.copyTo(image);
	}




void imageCallback(const sensor_msgs::ImageConstPtr& old_image)
{
		cv_bridge::CvImagePtr cv_ptr_in;

		cv_bridge::CvImagePtr cv_ptr_out;


		cv::Mat color;
		cv::Mat color_resized;

		//readImage(old_image,color);

		cv_ptr_in = cv_bridge::toCvCopy(old_image,sensor_msgs::image_encodings::BGR8);

		cv_ptr_out = cv_bridge::toCvCopy(old_image,sensor_msgs::image_encodings::BGR8);


//		cv::resize(color, cv_ptr->image, color.size(),0.
//0,0.0, cv::INTER_LINEAR);
		//width,heigth
		cv::resize(cv_ptr_out->image, cv_ptr_out->image, cv::Size(1920,1080),0.0,0.0, cv::INTER_CUBIC);


		cv_ptr_out->toImageMsg();

		pub.publish(cv_ptr_out->toImageMsg());
        // created shared pointer Image
        sensor_msgs::Image::Ptr small_image =
                        boost::make_shared<sensor_msgs::Image>();
        // copy image properties
        small_image->header = old_image->header;
        small_image->height = old_image->height / DIVISOR;
        small_image->width = old_image->width / DIVISOR;
        small_image->encoding = old_image->encoding;
        small_image->is_bigendian = old_image->is_bigendian;
        small_image->step = old_image->step / DIVISOR;


//void cv::resize(cv::InputArray, cv::OutputArray, cv::Size, double, double, int)





        //small_image->data.resize(small_image->width * small_image->height);

        // copy every DIVISORth byte
        // subpixels will be merged if multiple bytes per pixel
        //uint new_index = 0;
        //for(uint row = 0; row < small_image->height; row++) {
         //       int row_offset = row*old_image->step*DIVISOR;
           //     for(uint col = 0; col < small_image->width; col++) {
             //           int old_index = row_offset + col*DIVISOR;
               //         small_image->data[new_index++] =
                 //                       old_image->data[old_index];
                //}
        //}

        //pub.publish(small_image);

}





int main(int argc, char **argv)
{
        ros::init(argc, argv, "image_shrinker");
        ros::NodeHandle nh;
        image_transport::ImageTransport it(nh);
        image_transport::Subscriber sub =
                        it.subscribe("web_cam/image_raw", 1, imageCallback);
        pub = it.advertise("web_cam/image_small", 1);

        ros::spin();
        printf("Im out! No error.");
}
