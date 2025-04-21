#pragma once
#ifndef FANUC_TYPES_H
#define FANUC_TYPES_
#include <string>
#include <map>

typedef struct void_func
{
	short error;

	bool IsError() const
	{
		return error != 0;
	}
} void_func;

typedef struct bool_data
{
	bool data;
	short error;

	bool IsError() const
	{
		return error != 0;
	}

	void PullData(bool& _data, short& _error) const
	{
		if (error != 0)
			_error = error;
		else
			_data = data;
	}
} bool_data;

typedef struct str_data
{
	std::string data;
    short error;

	bool IsError() const 
	{
		return error != 0;
	}

	void PullData(std::string& _data, short& _error) const
	{
		if (error != 0)
			_error = error;
		else
			_data = data;
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

	void PullData(int& _data, short& _error) const
	{
		if (error != 0)
			_error = error;
		else
			_data = data;
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

	void PullData(long& _data, short& _error) const
	{
		if (error != 0)
			_error = error;
		else
			_data = data;
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

	void PullData(float& _data, short& _error) const
	{
		if (error != 0)
			_error = error;
		else
			_data = data;
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

	void PullData(short& _data, short& _error) const
	{
		if (error != 0)
			_error = error;
		else
			_data = data;
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

	void PullData(unsigned short& _data, short& _error) const
	{
		if (error != 0)
			_error = error;
		else
			_data = data;
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

	void PullData(std::map<std::string, int>& _data, short& _error) const
	{
		if (error != 0)
			_error = error;
		else
			_data = data;
	}
} map_data;
#endif