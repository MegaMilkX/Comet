#include "MeshIO.h"

int main()
{
	MeshIO::MeshIO mesh;
	mesh.Write(std::wstring(L"test.msh"), MeshIO::MSH_BIN);
	mesh.Read(std::wstring(L"test.msh"));
	return 0;
}