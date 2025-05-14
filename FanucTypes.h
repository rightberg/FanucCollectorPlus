#pragma once
#ifndef FANUC_TYPES_H
#define FANUC_TYPES_
#include <string>
#include <map>

struct VoidFunc
{
	short error;

	bool IsError() const
	{
		return error != 0;
	}
};

struct StrData
{
	std::string data;
    short error;

	bool IsError() const 
	{
		return error != 0;
	}
};

struct LongData
{
	long data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
};

struct DoubleData
{
	double data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}

};

struct ShortData
{
	short  data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
};

struct UShortData
{
	unsigned short  data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
};

struct LongMapData
{
	std::map<std::string, long> data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
};

struct DoubleMapData
{
	std::map<std::string, double> data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}
};
#endif