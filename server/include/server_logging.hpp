//
// Created by egor on 02.06.2020.
//

#ifndef SERVER_SERVER_LOGGING_HPP
#define SERVER_SERVER_LOGGING_HPP

#include <boost/log/core.hpp>

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;


inline void InitServerLogging()
{
	logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");

	logging::add_file_log
			(
					keywords::file_name = "server.log",
					keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
			);

	logging::core::get()->set_filter
			(
					logging::trivial::severity >= logging::trivial::trace
			);

	logging::add_common_attributes();
}

#endif //SERVER_SERVER_LOGGING_HPP
