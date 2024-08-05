/*
Author: Sam Wilcock (sam.wilcock@polimi.it)
*/

#include "rclcpp/rclcpp.hpp"
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <ros2_file_server/srv/get_binary_file.hpp>
#include <fstream>

static const rclcpp::Logger LOGGER = rclcpp::get_logger("file_server");


void get_file(
	const std::shared_ptr<ros2_file_server::srv::GetBinaryFile::Request> req,
	std::shared_ptr<ros2_file_server::srv::GetBinaryFile::Response> res)
{

	// analyze request:
	if (req->name.compare(0,10,"package://")!=0)
	{
		RCLCPP_INFO(LOGGER, "only \"package://\" adresses allowed.");
	}	
	std::string adress = req->name.substr(10);
	std::string package = adress.substr(0,adress.find("/"));
	std::string filepath = adress.substr(package.length());
	std::string directory = ament_index_cpp::get_package_share_directory(package);
	directory+=filepath;
	
	// open file:
	std::ifstream inputfile(directory.c_str(),std::ios::binary);

	// stop if file does not exist:
	if(!inputfile.is_open())
	{
		RCLCPP_INFO(LOGGER, "file \"%s\" not found.\n", req->name.c_str());
	}

	// read file contents:	
	res->value.assign(
		(std::istreambuf_iterator<char>(inputfile)),
		std::istreambuf_iterator<char>());

	RCLCPP_INFO(LOGGER, "get_file request: %s\n", req->name.c_str());
	RCLCPP_INFO(LOGGER, "package: %s\n",package.c_str());
	RCLCPP_INFO(LOGGER, "filepath: %s\n",filepath.c_str());
	RCLCPP_INFO(LOGGER, "directory: %s\n",directory.c_str());
}


int main(int argc, char **argv)
{
	rclcpp::init(argc, argv);
	auto node = rclcpp::Node::make_shared("file_server");
	rclcpp::Service<ros2_file_server::srv::GetBinaryFile>::SharedPtr service = 
		node->create_service<ros2_file_server::srv::GetBinaryFile>("/file_server/get_file", &get_file);
	RCLCPP_INFO(LOGGER, "ROSbridge File Server initialized.");
	rclcpp::spin(node);

	return 0;
}


	