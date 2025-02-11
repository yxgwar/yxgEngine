#pragma once
#include <iostream>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

#define ERROR(...)		std::cout << RED	<< (__VA_ARGS__) << RESET << std::endl
#define WARNING(...)	std::cout << YELLOW << (__VA_ARGS__) << RESET << std::endl
#define INFO(...)		std::cout << GREEN	<< (__VA_ARGS__) << RESET << std::endl