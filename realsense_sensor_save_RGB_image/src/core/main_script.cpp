// This program is handling intel realsense  R200 with infrared image.
// Note. This program surely is used on R200.

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <librealsense/rs.hpp>
#include <boost/filesystem.hpp>


int main() try
{
    // Create a context object. This object owns the handles to all connected realsense devices.
    rs::context ctx;
    std::cout << "There are " << ctx.get_device_count() << " connected RealSense device.\n" << std::endl;
    if(ctx.get_device_count() == 0)
    {
      return EXIT_FAILURE;
    }


    // This tutorial will access only a single device, but it is trivial to extend to multiple devices
    rs::device * dev = ctx.get_device(0);
    float depth_scale = dev->get_depth_scale();
    std::cout << "\nUsing device 0, an " << dev->get_name() << std::endl;
    std::cout << "Serial number: " << dev->get_serial() << std::endl;
    std::cout << "Firmware version: " << dev->get_firmware_version() << std::endl;
    std::cout << "Depth scale : " << depth_scale << std::endl;


    // Configure depth to run at VGA resolution at 60 frames per second
    dev->enable_stream(rs::stream::color, 640, 480, rs::format::bgr8, 60);
    dev->start();


    // Make folders for saving current image
    int image_counter = 0;
    std::string image_file_name;
    std::string folder_name = "images/";
    std::string folder_create_command = "mkdir -p " + folder_name;
    system(folder_create_command.c_str());


    while(true)
    {
        dev->wait_for_frames();
        // Get color frame and Change the format to Mat files
        const void * color_frame = dev->get_frame_data(rs::stream::color);
        cv::Mat color(480, 640, CV_8UC3, (void*)color_frame);


        // change current image file name
        char image_index[255];
        sprintf(image_index, "%010d.png", image_counter);
        image_file_name = folder_name + image_index;
        std::cout << image_file_name << std::endl;
        image_counter++;


        // update and save the current image
        cv::imshow("color", color);
        cv::waitKey(1);
        cv::imwrite(image_file_name, color);
   }


   return EXIT_SUCCESS;
}


catch(const rs::error & e)
{
    // Method calls against librealsense objects may throw exceptions of type rs::error
    std::cout << "rs::error was thrown when calling " << e.get_failed_function().c_str() << "(" << e.get_failed_args().c_str() << ")" << std::endl;
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
}


/* enum class stream : int32_t
    {
        depth                           ,  ///< Native stream of depth data produced by RealSense device
        color                           ,  ///< Native stream of color data captured by RealSense device
        infrared                        ,  ///< Native stream of infrared data captured by RealSense device
        infrared2                       ,  ///< Native stream of infrared data captured from a second viewpoint by RealSense device
        fisheye                         ,
        points                          ,  ///< Synthetic stream containing point cloud data generated by deprojecting the depth image
        rectified_color                 ,  ///< Synthetic stream containing undistorted color data with no extrinsic rotation from the depth stream
        color_aligned_to_depth          ,  ///< Synthetic stream containing color data but sharing intrinsic of depth stream
        infrared2_aligned_to_depth      ,  ///< Synthetic stream containing second viewpoint infrared data but sharing intrinsic of depth stream
        depth_aligned_to_color          ,  ///< Synthetic stream containing depth data but sharing intrinsic of color stream
        depth_aligned_to_rectified_color, ///< Synthetic stream containing depth data but sharing intrinsic of rectified color stream
        depth_aligned_to_infrared2        ///< Synthetic stream containing depth data but sharing intrinsic of second viewpoint infrared stream
}; */