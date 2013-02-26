/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "AssetLoader.h"

#include <log4cxx/logger.h>

#include <fstream>

AssetLoader::AssetLoader(const std::string & path) :
	path_(path)
{
}

std::string AssetLoader::load(const std::string & filename)
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("voxel.log"));

	std::string fullPath = path_ + filename;

	// read shader file content
	std::ifstream file(fullPath.c_str(), std::ios::in | std::ios::binary);
	if (!file)
	{
		std::string err = std::string("error loading asset file: ") + fullPath + std::string(" - ") + strerror(errno);
		LOG4CXX_ERROR(logger, err);
		throw std::runtime_error(err);
	}
	std::string content;
	file.seekg(0, std::ios::end);
	content.resize(static_cast<unsigned int>(file.tellg()));
	file.seekg(0, std::ios::beg);
	file.read(&content[0], content.size());
	file.close();

	return content;
}
