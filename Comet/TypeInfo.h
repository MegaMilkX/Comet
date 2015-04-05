#pragma once

#include <string>

template <class T> struct TypeInfo
{
	static std::string GetName()
	{static_assert(0, "USE DEFTYPE");}
	static const int GetId()
	{return id;}
	static const int id;
};