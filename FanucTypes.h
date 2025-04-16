#pragma once
#ifndef FANUC_TYPES_H
#define FANUC_TYPES_H
#include <string>
#include <iostream>
#include <map>

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
    short error;

	bool IsError() const 
	{
		return error != 0;
	}
} str_data;

typedef struct int_data
{
	int data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
} int_data;

typedef struct long_data
{
	long data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
} long_data;

typedef struct float_data
{
	float data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
} float_data;

typedef struct short_data
{
	short  data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
} short_data;

typedef struct ushort_data
{
	unsigned short  data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
} ushort_data;

typedef struct map_data
{
	std::map<std::string,int> data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
} map_data;

#endif