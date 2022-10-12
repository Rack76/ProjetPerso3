#include "glew.h"
#include <iostream>
#include <string>
#include <fstream>

unsigned char* loadImageData(std::string filename)
{
	std::string string;
	std::ifstream file(filename, std::ios::binary | std::ios::in);

	file.ignore(8);
	 char item;
	 unsigned int IHDRlength;
	 char IHDRtypeField;
	 char IHDRCRC;
	 char PLTElength;
	 char PLTEtypeField;
	 char PLTECRC;
	 char IDATlength;
	 char IDATtypeField;

	file.get(item);
	IHDRlength = item;
	file.get(item);
	IHDRlength += item << 8;
	file.get(item);
	IHDRlength += item << 16;
	file.get(item);
	IHDRlength += item << 24;

	std::cout << IHDRlength;
	file.get(IHDRtypeField);
	file.get(IHDRtypeField);
	file.get(IHDRtypeField);
	file.get(IHDRtypeField);

	file.ignore(IHDRlength);
	file.get(IHDRCRC);
	file.get(IHDRCRC);
	file.get(IHDRCRC);
	file.get(IHDRCRC);

	file.get(item);
	PLTElength = item;
	file.get(item);
	PLTElength += item << 8;
	file.get(item);
	PLTElength += item << 16;
	file.get(item);
	PLTElength += item << 24;

	file.get(PLTEtypeField);
	file.get(PLTEtypeField);
	file.get(PLTEtypeField);
	file.get(PLTEtypeField);

	file.ignore(PLTElength);

	file.get(PLTECRC);
	file.get(PLTECRC);
	file.get(PLTECRC);
	file.get(PLTECRC);

	file.get(item);
	IDATlength = item;
	file.get(item);
	IDATlength += item << 8;
	file.get(item);
	IDATlength += item << 16;
	file.get(item);
	IDATlength += item << 24;

	file.get(IDATtypeField);
	file.get(IDATtypeField);
	file.get(IDATtypeField);
	file.get(IDATtypeField);



	unsigned char* imageData = new unsigned char[IDATlength];
	for (int i = 0; i < IDATlength; i++)
	{
		file >> imageData[i];
	}

	return imageData;
}

void loadPNGWidthAndHeight(std::string filename, int &width, int& height)
{
	std::ifstream file(filename, std::ios::in);
	char item;
	file.ignore(16);
	file.get(item);

	width = item;
	file.get(item);
	width += item << 8;
	file.get(item);
	width += item << 16;
	file.get(item);
	width += item << 24;

	height = item;
	file.get(item);
	height += item << 8;
	file.get(item);
	height += item << 16;
	file.get(item);
	height += item << 24;
}