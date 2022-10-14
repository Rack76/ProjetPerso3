#include "MeshLoading.h"

GLfloat* loadOBJMesh(std::string filename, int& size, GLfloat** textureCoordArray, int& textCoordSize)
{
	std::vector<GLfloat> unsortedVertexArray;
	std::vector<GLfloat> unsortedTextureCoordArray;
	std::vector<GLfloat> sortedVertexArray;
	std::vector<GLfloat> sortedTextureCoordArray;
	std::vector<int> order;
	std::vector<int> textureOrder;

	std::ifstream file(filename);
	std::string line;

	while (std::getline(file, line), line[0] != 'o')
	{

	}

	while (std::getline(file, line), line[1] != 't')
	{
		std::istringstream ss(line);
		char t;
		float item;
		ss >> t;
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

	std::vector<EdgeInfo> edgeInfoList;
	EdgeInfo edge;
	std::vector<int> edgeIndex;
	auto it = edgeIndex.begin();

	for (i; i < arraySize; i++)
	{
		edgeIndex.push_back(order[i]);
		edge.v0.x = unsortedVertexArray[(order[i] - 1) * 3];
		edge.v0.y = unsortedVertexArray[(order[i] - 1) * 3 + 1];
		edge.v0.z = unsortedVertexArray[(order[i] - 1) * 3 + 2];

		edge.v1.x = unsortedVertexArray[(order[((i + 1) % 3) + 3 * (i / 3)] - 1) * 3];
		edge.v1.y = unsortedVertexArray[(order[((i + 1) % 3) + 3 * (i / 3)] - 1) * 3 + 1];
		edge.v1.z = unsortedVertexArray[(order[((i + 1) % 3) + 3 * (i / 3)] - 1) * 3 + 2];

		if ((it = std::lower_bound(edgeIndex.begin(), edgeIndex.end(), order[(i + 1) % arraySize] )) == edgeIndex.end())
		{

		}
		else
		{
			
		}
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

	*textureCoordArray = createArray(sortedTextureCoordArray);

	size = 4 * sortedVertexArray.size();
	textCoordSize =  4 * sortedTextureCoordArray.size();

	return createArray(sortedVertexArray);
}



GLfloat* createArray(const std::vector<GLfloat>& v)
{
	GLfloat* result = new GLfloat[v.size()];

	memcpy(result, &v.front(), v.size() * sizeof(GLfloat));

	return result;
}