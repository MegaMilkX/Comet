#include "BinTree.h"


unsigned int count_bin_data_storage(unsigned int depth)
{
	unsigned int res = 0;
	
	while (depth > 0)
	{
		depth--;
		res += 1 << depth;
	}
	//res += 1;

	return res;
}