#include "MeshLoading.h"

GLfloat* loadOBJMesh(std::string filename, int& size, GLfloat** textureCoordArray, int& textCoordSize, std::vector<GLfloat>& vertexPositionsVector, std::vector<GLfloat> &unsortedVertexArray, std::vector<int> &order)
{
	std::vector<GLfloat> unsortedTextureCoordArray;
	std::vector<GLfloat> sortedVertexArray;
	std::vector<GLfloat> sortedTextureCoordArray;
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

	arraySize = textureOrder.size();
	i = 0;

	for (i; i < arraySize; i++)
	{
		sortedTextureCoordArray.push_back(unsortedTextureCoordArray[(textureOrder[i] - 1) * 2]);
		sortedTextureCoordArray.push_back(unsortedTextureCoordArray[(textureOrder[i] - 1) * 2 + 1]);
	}

	*textureCoordArray = createArray(sortedTextureCoordArray);
	vertexPositionsVector = sortedVertexArray;
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

void meshInfo(const int faceCount, std::vector<FaceInfo> &faceInfoList, std::vector<EdgeInfo> &edgeInfoList, 
	          std::vector<glm::vec3> &normals, const std::vector<GLfloat> &sortedVertexArray, 
	          const std::vector<GLfloat>& unsortedVertexArray, const std::vector<int>& order)
{
	int i = 0;
	for (i; i < faceCount; i ++)
	{
		faceInfoList.push_back(FaceInfo());
	}
	for (i = 0; i < faceCount * 3 * 3; i+=3)
	{
		faceInfoList[i/9].v[(i/3) % 3] = glm::vec3(sortedVertexArray[i], sortedVertexArray[i + 1], sortedVertexArray[i + 2]);
	}

	for (int i = 0; i < faceCount; i++)
	{
		normals.push_back(glm::normalize(glm::cross(faceInfoList[i].v[1] - faceInfoList[i].v[0], faceInfoList[i].v[2] - faceInfoList[i].v[0])));
	}

	auto it = edgeInfoList.begin();
	EdgeInfo edgeInfo;

	for (i = 0; i < faceCount * 3; i++)
	{
		edgeInfo.v0.x = unsortedVertexArray[(order[i] - 1) * 3];
		edgeInfo.v0.y = unsortedVertexArray[(order[i] - 1) * 3 + 1];
		edgeInfo.v0.z = unsortedVertexArray[(order[i] - 1) * 3 + 2];

		edgeInfo.v1.x = unsortedVertexArray[(order[((i + 1) % 3) + 3 * (i / 3)] - 1) * 3];
		edgeInfo.v1.y = unsortedVertexArray[(order[((i + 1) % 3) + 3 * (i / 3)] - 1) * 3 + 1];
		edgeInfo.v1.z = unsortedVertexArray[(order[((i + 1) % 3) + 3 * (i / 3)] - 1) * 3 + 2];

		it = std::find(edgeInfoList.begin(), edgeInfoList.end(), edgeInfo);
		if (it == edgeInfoList.end())
		{
			edgeInfoList.push_back(EdgeInfo(edgeInfo.v0, edgeInfo.v1));
			edgeInfoList[edgeInfoList.size() - 1].faceInfoLeft = &faceInfoList[i / 3];
			faceInfoList[i / 3].edgeInfo[i % 3] = &edgeInfoList[edgeInfoList.size() - 1];
		}
		else
		{
			it->faceInfoRight = &faceInfoList[i / 3];
			faceInfoList[i / 3].edgeInfo[i % 3] = it._Ptr;
		}
	}
}