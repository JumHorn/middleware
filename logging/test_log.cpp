/**
 * @file test_log.cpp
 * @author JumHorn (JumHorn@gamil.com)
 * @brief
 * @version 0.1
 * @date 2020-11-24
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "log.h"

int main()
{
	Logger::init();
	Logger::log("hello");
	Logger::log("%d %d", 123, 876);
	return 0;
}