/**
 * @file log.h
 * @author JumHorn (JumHorn@gamil.com)
 * @brief a logger wrapper for boost.log
 * @version 0.1
 * @date 2020-11-23
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <memory>
#include <string>
using std::shared_ptr;
using std::string;

namespace boost
{
	namespace log
	{
		namespace sources
		{
			class logger;
		}
	} // namespace log
} // namespace boost

class Logger
{
public:
	static void log(const string &logstr);
	static void log(char *logfmt, ...);

	/**
	 * @brief init must before calling to log and init is not thread safe
	 *
	 * @param logFileModule log module name
	 * @param logFilePath	log path name
	 * final path is logFilePath+"/"+logFileModule
	 */
	static void init(const string &logFileModule = "", const string &logFilePath = "logs");

private:
	Logger() {}
	~Logger() {}

	static shared_ptr<boost::log::sources::logger> m_logger;
};

#endif