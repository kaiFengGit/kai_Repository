#pragma once


string int2str(const int &tmp)
{
	stringstream sst;
	sst << tmp;
	return sst.str();
}


string float2str(const float &tmp)
{
	stringstream sst;
	sst << tmp;
	return sst.str();
}