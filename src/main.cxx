/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/helpers/transcoder.h>

#include <cstdlib>
#include <string>

#include "Controller.h"


int main(int argc, char *argv[]) 
{
	// extract exe path from argv (needed for loading file assets with relative paths)
	boost::filesystem::path path(boost::filesystem::initial_path<boost::filesystem::path>());
    path = boost::filesystem::system_complete(boost::filesystem::path(argv[0])).remove_filename();

	boost::filesystem::path slash("/");
	std::string preferredSlash = slash.make_preferred().string();

	std::string appPath = path.string() + preferredSlash;

	// setup the logger
	std::string logfile = appPath + std::string("voxel.log");
	log4cxx::FileAppender * fileAppender = new log4cxx::FileAppender(log4cxx::LayoutPtr(new log4cxx::SimpleLayout()), 
		log4cxx::helpers::Transcoder::decode(logfile.c_str()), false);
	log4cxx::BasicConfigurator::configure(log4cxx::AppenderPtr(fileAppender));

	log4cxx::helpers::Pool p;
	fileAppender->activateOptions(p);

	// logging level set to debug
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("voxel.log"));
	logger->setLevel(log4cxx::Level::getDebug());

	Controller controller(appPath);
	if (!controller.run())
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
