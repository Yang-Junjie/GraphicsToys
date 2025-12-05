#pragma once
#include "tiny_obj_loader.h"
#include "Mesh.hpp"
#include <iostream>
#include <vector>


inline bool LoadObjToMesh(const std::string &filepath, gty::Mesh &outMesh)
{
    tinyobj::ObjReaderConfig cfg;
    cfg.mtl_search_path = "";
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filepath, cfg))
    {
        std::cerr << "OBJ load error: " << reader.Error() << "\n";
        return false;
    }

    const auto &attrib = reader.GetAttrib();
    const auto &shapes = reader.GetShapes();

  
    size_t vertexCount = attrib.vertices.size() / 3;
    std::vector<glm::vec3> vertexNormals(vertexCount, glm::vec3(0.0f));


    for (const auto &shape : shapes)
    {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
        {
            int fv = shape.mesh.num_face_vertices[f];
            if (fv != 3)
            {
                index_offset += fv;
                continue; 
            }

            tinyobj::index_t idx0 = shape.mesh.indices[index_offset + 0];
            tinyobj::index_t idx1 = shape.mesh.indices[index_offset + 1];
            tinyobj::index_t idx2 = shape.mesh.indices[index_offset + 2];

            glm::vec3 v0(
                attrib.vertices[3 * idx0.vertex_index + 0],
                attrib.vertices[3 * idx0.vertex_index + 1],
                attrib.vertices[3 * idx0.vertex_index + 2]);
            glm::vec3 v1(
                attrib.vertices[3 * idx1.vertex_index + 0],
                attrib.vertices[3 * idx1.vertex_index + 1],
                attrib.vertices[3 * idx1.vertex_index + 2]);
            glm::vec3 v2(
                attrib.vertices[3 * idx2.vertex_index + 0],
                attrib.vertices[3 * idx2.vertex_index + 1],
                attrib.vertices[3 * idx2.vertex_index + 2]);

            glm::vec3 faceNormal = glm::cross(v1 - v0, v2 - v0);
            if (glm::length(faceNormal) > 1e-6f)
                faceNormal = glm::normalize(faceNormal);
            else
                faceNormal = glm::vec3(0.f, 0.f, 1.f);

            vertexNormals[idx0.vertex_index] += faceNormal;
            vertexNormals[idx1.vertex_index] += faceNormal;
            vertexNormals[idx2.vertex_index] += faceNormal;

            index_offset += fv;
        }
    }


    for (auto &n : vertexNormals)
    {
        if (glm::length(n) > 1e-6f)
            n = glm::normalize(n);
        else
            n = glm::vec3(0.f, 0.f, 1.f);
    }

   
    for (const auto &shape : shapes)
    {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
        {
            int fv = shape.mesh.num_face_vertices[f];
            if (fv != 3)
            {
                index_offset += fv;
                continue;
            }

            tinyobj::index_t idx0 = shape.mesh.indices[index_offset + 0];
            tinyobj::index_t idx1 = shape.mesh.indices[index_offset + 1];
            tinyobj::index_t idx2 = shape.mesh.indices[index_offset + 2];

            glm::vec3 v0(
                attrib.vertices[3 * idx0.vertex_index + 0],
                attrib.vertices[3 * idx0.vertex_index + 1],
                attrib.vertices[3 * idx0.vertex_index + 2]);
            glm::vec3 v1(
                attrib.vertices[3 * idx1.vertex_index + 0],
                attrib.vertices[3 * idx1.vertex_index + 1],
                attrib.vertices[3 * idx1.vertex_index + 2]);
            glm::vec3 v2(
                attrib.vertices[3 * idx2.vertex_index + 0],
                attrib.vertices[3 * idx2.vertex_index + 1],
                attrib.vertices[3 * idx2.vertex_index + 2]);

            glm::vec3 n0 = vertexNormals[idx0.vertex_index];
            glm::vec3 n1 = vertexNormals[idx1.vertex_index];
            glm::vec3 n2 = vertexNormals[idx2.vertex_index];

            gty::Vertex3 vert0(v0, n0, glm::vec4(1, 1, 1, 1));
            gty::Vertex3 vert1(v1, n1, glm::vec4(1, 1, 1, 1));
            gty::Vertex3 vert2(v2, n2, glm::vec4(1, 1, 1, 1));

            outMesh.triangles.emplace_back(vert0, vert1, vert2);
            index_offset += fv;
        }
    }

    return true;
}
