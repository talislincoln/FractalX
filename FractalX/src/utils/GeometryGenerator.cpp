#include <FractalPCH.h>
#include <utils\GeometryGenerator.h>

#include <fstream>

namespace fractal
{
	fcore::MeshData GeometryGenerator::LoadObjFromFile (const std::string& filePath)
	{
		fcore::MeshData obj;

		int vertexCount, textureCount, normalCount, faceCount;

		if (!ReadFileCounts (filePath, vertexCount, textureCount, normalCount, faceCount))
		{
			return obj;
		}

		VertexPosColorTexture* vertices = new VertexPosColorTexture[vertexCount];
		int vertexIndex = 0, texcoordIndex = 0, normalIndex = 0, faceIndex = 0, vIndex = 0, tIndex = 0, nIndex = 0;

		WORD* faces = new WORD[faceCount];

		std::ifstream fin;
		// Open the file.
		fin.open (filePath);

		// Check if it was successful in opening the file.
		if (fin.fail () == true)
		{
			return obj;
		}

		char input, input2; int trash;
		// Read in the vertices, texture coordinates, and normals into the data structures.
		// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
		fin.get (input);
		while (!fin.eof ())
		{
			if (input == 'v')
			{
				fin.get (input);

				// Read in the vertices.
				if (input == ' ')
				{
					fin >> vertices[vertexIndex].Position.x >> vertices[vertexIndex].Position.y >> vertices[vertexIndex].Position.z;

					// Invert the Z vertex to change to left hand system.
					vertices[vertexIndex].Position.z = vertices[vertexIndex].Position.z * -1.0f;
					vertexIndex++;
				}

				// Read in the texture uv coordinates.
				if (input == 't')
				{
					fin >> vertices[texcoordIndex].UV.y >> vertices[texcoordIndex].UV.x;

					// Invert the V texture coordinates to left hand system.
					vertices[texcoordIndex].UV.x = 1.0f - vertices[texcoordIndex].UV.x;
					texcoordIndex++;
				}

				// Read in the normals.
				if (input == 'n')
				{
					fin >> vertices[normalIndex].Normal.x >> vertices[normalIndex].Normal.y >> vertices[normalIndex].Normal.z;

					// Invert the Z normal to change to left hand system.
					vertices[normalIndex].Normal.z = vertices[normalIndex].Normal.z * -1.0f;
					normalIndex++;
				}
			}

			// Read in the faces.
			if (input == 'f')
			{
				fin.get (input);
				if (input == ' ')
				{
					// Read the face data in backwards to convert it to a left hand system from right hand system.
					fin >> faces[faceIndex] >> input2 >> /*trash >>*/ input2 >> trash >> faces[faceIndex + 1] >> input2 >> /*trash >>*/ input2 >> trash >> faces[faceIndex + 2];

					faces[faceIndex + 2] = faces[faceIndex + 2] - 1;
					faces[faceIndex + 1] = faces[faceIndex + 1] - 1;
					faces[faceIndex] = faces[faceIndex] - 1;

					faceIndex +=3;
				}
			}

			// Read in the remainder of the line.
			while (input != '\n')
			{
				fin.get (input);
			}

			// Start reading the beginning of the next line.
			fin.get (input);
		}

		//UINT aa[reinterpret_cast<const UINT>(faceCount)];

		obj.vertices.assign (&vertices[0], &vertices[vertexCount]);
		obj.indices.assign (&faces[0], &faces[faceCount]);

		delete[] vertices;
		delete[] faces;
		// Close the file.
		fin.close ();


		return obj;
	}

	fcore::MeshData GeometryGenerator::CreateBox (float width, float height, float depth)
	{
		fcore::MeshData box;

		VertexPosColorTexture vertices[24];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;
		float d2 = 0.5f * depth;

		// Fill in the front face vertex data.
		vertices[0] = VertexPosColorTexture (-w2, -h2, -d2, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f);
		vertices[1] = VertexPosColorTexture (-w2, +h2, -d2, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		vertices[2] = VertexPosColorTexture (+w2, +h2, -d2, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		vertices[3] = VertexPosColorTexture (+w2, -h2, -d2, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f);

		// Fill in the back face vertex data.
		vertices[4] = VertexPosColorTexture (-w2, -h2, +d2, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		vertices[5] = VertexPosColorTexture (+w2, -h2, +d2, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		vertices[6] = VertexPosColorTexture (+w2, +h2, +d2, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		vertices[7] = VertexPosColorTexture (-w2, +h2, +d2, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

		// Fill in the top face vertex data.
		vertices[8]  = VertexPosColorTexture (-w2, +h2, -d2, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		vertices[9] = VertexPosColorTexture (-w2, +h2, +d2, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		vertices[10] = VertexPosColorTexture (+w2, +h2, +d2, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		vertices[11] = VertexPosColorTexture (+w2, +h2, -d2, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);

		// Fill in the bottom face vertex data.
		vertices[12] = VertexPosColorTexture (-w2, -h2, -d2, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
		vertices[13] = VertexPosColorTexture (+w2, -h2, -d2, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);
		vertices[14] = VertexPosColorTexture (+w2, -h2, +d2, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
		vertices[15] = VertexPosColorTexture (-w2, -h2, +d2, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f);

		// Fill in the left face vertex data.
		vertices[16] = VertexPosColorTexture (-w2, -h2, +d2, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		vertices[17] = VertexPosColorTexture (-w2, +h2, +d2, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		vertices[18] = VertexPosColorTexture (-w2, +h2, -d2, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		vertices[19] = VertexPosColorTexture (-w2, -h2, -d2, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f);

		// Fill in the right face vertex data.
		vertices[20] = VertexPosColorTexture (+w2, -h2, -d2, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
		vertices[21] = VertexPosColorTexture (+w2, +h2, -d2, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		vertices[22] = VertexPosColorTexture (+w2, +h2, +d2, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		vertices[23] = VertexPosColorTexture (+w2, -h2, +d2, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);

		box.vertices.assign (&vertices[0], &vertices[24]);

		UINT i[36];

		// Fill in the front face index data
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		// Fill in the back face index data
		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		// Fill in the top face index data
		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		// Fill in the bottom face index data
		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		// Fill in the left face index data
		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		// Fill in the right face index data
		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		box.indices.assign (&i[0], &i[36]);

		return box;
	}

	fcore::MeshData GeometryGenerator::CreateSphere (float radius, UINT sliceCount, UINT stackCount)
	{
		using namespace DirectX;

		fcore::MeshData sphere;

		VertexPosColorTexture topVertex		(0.0f, +radius, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		VertexPosColorTexture bottomVertex	(0.0f, -radius, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);

		sphere.vertices.push_back (topVertex);

		float phiStep = MATH_PI / stackCount;
		float thetaStep = 2.0f*MATH_PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for(UINT i = 1; i <= stackCount-1; ++i)
		{
			float phi = i*phiStep;

			// Vertices of ring.
			for(UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j*thetaStep;

				VertexPosColorTexture v;

				// spherical to cartesian
				v.Position.x = radius*sinf(phi)*cosf(theta);
				v.Position.y = radius*cosf(phi);
				v.Position.z = radius*sinf(phi)*sinf(theta);

				v.Color.x = 1.0f;
				v.Color.y = 1.0f;
				v.Color.z = 1.0f;

				// Remove the comments when adding tagent and normals
				// Partial derivative of P with respect to theta
				//v.TangentU.x = -radius*sinf(phi)*sinf(theta);
				//v.TangentU.y = 0.0f;
				//v.TangentU.z = +radius*sinf(phi)*cosf(theta);

				//XMVECTOR T = XMLoadFloat3(&v.TangentU);
				//XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

				XMVECTOR p = XMLoadFloat3(&v.Position);
				XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

				v.UV.x = theta / XM_2PI;
				v.UV.y = phi / XM_PI;

				sphere.vertices.push_back( v );
			}
		}

		sphere.vertices.push_back (bottomVertex);

		for (UINT i = 1; i <= sliceCount; ++i)
		{
			sphere.indices.push_back (0);
			sphere.indices.push_back (i + 1);
			sphere.indices.push_back (i);
		}

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		UINT baseIndex = 1;
		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				sphere.indices.push_back (baseIndex + i*ringVertexCount + j);
				sphere.indices.push_back (baseIndex + i*ringVertexCount + j + 1);
				sphere.indices.push_back (baseIndex + (i + 1)*ringVertexCount + j);
					   
				sphere.indices.push_back (baseIndex + (i + 1)*ringVertexCount + j);
				sphere.indices.push_back (baseIndex + i*ringVertexCount + j + 1);
				sphere.indices.push_back (baseIndex + (i + 1)*ringVertexCount + j + 1);
			}
		}

		UINT southPoleIndex = (UINT)sphere.vertices.size () - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			sphere.indices.push_back (southPoleIndex);
			sphere.indices.push_back (baseIndex + i);
			sphere.indices.push_back (baseIndex + i + 1);
		}

		return sphere;
	}

	bool GeometryGenerator::ReadFileCounts (const std::string& filePath, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
	{
		std::ifstream fin;
		char input;


		// Initialize the counts.
		vertexCount = 0;
		textureCount = 0;
		normalCount = 0;
		faceCount = 0;

		// Open the file.
		fin.open (filePath);

		// Check if it was successful in opening the file.
		if (fin.fail () == true)
		{
			return false;
		}

		// Read from the file and continue to read until the end of the file is reached.
		fin.get (input);
		while (!fin.eof ())
		{
			// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
			if (input == 'v')
			{
				fin.get (input);
				if (input == ' ') { vertexCount++; }
				if (input == 't') { textureCount++; }
				if (input == 'n') { normalCount++; }
			}

			// If the line starts with 'f' then increment the face count.
			if (input == 'f')
			{
				fin.get (input);
				if (input == ' ') { faceCount += 3; }
			}

			// Otherwise read in the remainder of the line.
			while (input != '\n')
			{
				fin.get (input);
			}

			// Start reading the beginning of the next line.
			fin.get (input);
		}

		// Close the file.
		fin.close ();

		return true;
	}
}