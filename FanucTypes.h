#pragma once
#ifndef FANUC_TYPES_H
#define FANUC_TYPES_H
#include <string>
#include <iostream>
#include <map>

typedef struct fanuc_data
{
	unsigned short  ushort_data;
	int int_data;
	float float_data;
	std::string str_data;
	std::string error_msg;
	std::map<std::string, int> data;

	bool IsError() const
	{
		return !error_msg.empty();
	}
} fanuc_data;


typedef struct void_func
{
	std::string error_msg;

	bool IsError() const
	{
		return !error_msg.empty();
	}
} void_func;

typedef struct str_data
{
	std::string data;
	std::string error_msg;

	bool IsError() const
	{
		return !error_msg.empty();
	}
} str_data;

typedef struct int_data
{
	int data;
	std::string error_msg;

	bool IsError() const
	{
		return !error_msg.empty();
	}
} int_data;

typedef struct float_data
{
	float data;
	std::string error_msg;

	bool IsError() const
	{
		return !error_msg.empty();
	}
} float_data;

typedef struct ushort_data
{
	unsigned short  data;
	std::string error_msg;

	bool IsError() const 
	{
		return !error_msg.empty();
	}
} ushort_data;

typedef struct map_data
{
	std::map<std::string,int> data;
	std::string error_msg;

	bool IsError() const 
	{
		return !error_msg.empty();
	}
} map_data;

#endif