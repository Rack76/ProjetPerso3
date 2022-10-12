#include "MeshLoading.h"

GLfloat* loadOBJMesh(std::string filename, int& size, GLfloat* textureCoordArray, int& textCoordSize)
{
	std::vector<GLfloat> unsortedVertexArray;
	std::vector<GLfloat> unsortedTextureCoordArray;
	std::vector<GLfloat> sortedVertexArray;
	std::vector<GLfloat> sortedTextureCoordArray;
	std::vector<GLfloat> order;
	std::vector<GLfloat> textureOrder;

	std::ifstream file(filename);
	std::string line;

	while (std::getline(file, line), line[0] != 'o')
	{

	}

	while (std::getline(file, line), line[1] != 't')
	{
		std::istringstream ss(line);
		char v;
		float item;
		ss >> v;
		ss >> item;
		unsortedVertexArray.push_back(item);
		ss >> item;
		unsortedVertexArray.push_back(item);
		ss >> item;
		unsortedVertexArray.push_back(item);
	}

	do 
	{
		std::istringstream ss(line);
		char vt;
		float item;
		ss >> vt;
		ss >> vt;
		ss >> item;
		unsortedTextureCoordArray.push_back(item);
		ss >> item;
		unsortedTextureCoordArray.push_back(item);
	} while (std::getline(file, line), line[0] != 's');

	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		char f;
		char slash;
		int item;
		ss >> f;
		ss >> item;
		order.push_back(item);
		ss >> slash;
		ss >> item;
		textureOrder.push_back(item);
		ss >> item;
		order.push_back(item);
		ss >> slash;
		ss >> item;
		textureOrder.push_back(item);
		ss >> item;
		order.push_back(item);
		ss >> slash;
		ss >> item;
		textureOrder.push_back(item);
	} 

	int arraySize = order.size();
	int i = 0;
	
	for (i; i < arraySize; i++)
	{
		sortedVertexArray.push_back(unsortedVertexArray[(order[i] - 1) * 3]);
		sortedVertexArray.push_back(unsortedVertexArray[(order[i] - 1) * 3 + 1]);
		sortedVertexArray.push_back(unsortedVertexArray[(order[i] - 1) * 3 + 2]);
	}

	arraySize = textureOrder.size();
	i = 0;

	for (i; i < arraySize; i++)
	{
		sortedTextureCoordArray.push_back(unsortedTextureCoordArray[(textureOrder[i] - 1) * 2]);
		sortedTextureCoordArray.push_back(unsortedTextureCoordArray[(textureOrder[i] - 1) * 2 + 1]);
	}

	for (auto var : sortedTextureCoordArray)
	{
		std::cout << var << '\n';
	}

	textureCoordArray = createArray(sortedTextureCoordArray);

	size = sortedVertexArray.size();
	textCoordSize =  4 * sortedTextureCoordArray.size();

	return createArray(sortedVertexArray);
}



GLfloat* createArray(const std::vector<GLfloat>& v)
{
	GLfloat* result = new GLfloat[v.size()];

	memcpy(result, &v.front(), v.size() * sizeof(GLfloat));

	return result;
}