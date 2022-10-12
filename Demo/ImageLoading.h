#include <string>
#include <fstream>

void loadPNGWidthAndHeight(std::string filename, int& width, int& height);
unsigned char* loadImageData(std::string filename);