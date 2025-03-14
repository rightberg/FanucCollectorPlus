#pragma once
#ifndef FANUC_TYPES_H
#define FANUC_TYPES_H
#include <string>
#include <iostream>
#include <map>

typedef struct void_func
{
	bool error;
	std::string error_msg;
} void_func;

typedef struct str_data
{
	bool error;
	std::string data;
	std::string error_msg;
} str_data;

typedef struct int_data
{
	bool error;
	int data;
	std::string error_msg;
} int_data;

typedef struct float_data
{
	bool error;
	float data;
	std::string error_msg;
} float_data;

typedef struct ushort_data
{
	bool error;
	unsigned short  data;
	std::string error_msg;
} ushort_data;

typedef struct map_data
{
	bool error;
	std::map<std::string,int> data;
	std::string error_msg;
} map_data;

#endif