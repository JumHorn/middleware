/**
 * @file log.cpp
 * @author JumHorn (JumHorn@gamil.com)
 * @brief
 * @version 0.1
 * @date 2020-11-23
 *
 * @copyright Copyright (c) 2020
 *
 */

#define BOOST_LOG_DYN_LINK 1 // for MacOS clang compile through

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <cstdarg>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/logger.hpp>

//with forward namespace define
//the include must behind #include <boost/log/sources/logger.hpp>
#include "log.h"

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

shared_ptr<src::logger> Logger::m_logger(new src::logger());

void Logger::init(const string &logFileModule /* = ""*/, const string &logFilePath /* = "logs"*/)
{
	try
	{
		// Create a text file sink
		typedef sinks::synchronous_sink<sinks::text_file_backend> file_sink;
		boost::shared_ptr<file_sink> sink(new file_sink(
			keywords::file_name = "%Y%m%d_%H%M%S_%5N.log", // file name pattern
			keywords::rotation_size = 16384				   // rotation size, in characters
			));

		// Set up where the rotated files will be stored
		sink->locked_backend()->set_file_collector(sinks::file::make_collector(
			keywords::target = logFilePath + "/" + logFileModule, // where to store rotated files
			keywords::max_size = 16 * 1024 * 1024,				  // maximum total size of the stored files, in bytes
			keywords::min_free_space = 100 * 1024 * 1024,		  // minimum free space on the drive, in bytes
			keywords::max_files = 512							  // maximum number of stored files
			));

		// Upon restart, scan the target directory for files matching the file_name pattern
		sink->locked_backend()->scan_for_files();

		sink->set_formatter(
			expr::format("%1%: [%2%] - %3%") % expr::attr<unsigned int>("RecordID") % expr::attr<boost::posix_time::ptime>("TimeStamp") % expr::smessage);

		// Add it to the core
		logging::core::get()->add_sink(sink);

		// Add some attributes too
		logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
		logging::core::get()->add_global_attribute("RecordID", attrs::counter<unsigned int>());
	}
	catch (std::exception &e)
	{
		std::cout << "FAILURE: " << e.what() << std::endl;
	}
}

void Logger::log(const string &logstr)
{
	BOOST_LOG(*m_logger) << logstr;
}

void Logger::log(char *logfmt, ...)
{
	char msg[8192];
	va_list ap;
	va_start(ap, logfmt);
	vsprintf(msg, logfmt, ap);
	va_end(ap);
	BOOST_LOG(*m_logger) << msg;
}