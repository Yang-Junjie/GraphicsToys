#pragma once
#include "tiny_obj_loader.h"
#include "Mesh.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <stb_image.h>

inline bool LoadObjToMesh(const std::string &filepath, gty::Mesh &outMesh)
{

    std::string baseDir;
    {
        size_t slash = filepath.find_last_of("/\\");
        if (slash != std::string::npos)
            baseDir = filepath.substr(0, slash + 1);
    }

    tinyobj::ObjReaderConfig cfg;
    cfg.mtl_search_path = baseDir;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filepath, cfg))
    {
        std::cerr << "OBJ load error: " << reader.Error() << "\n";
        return false;
    }

    const auto &attrib = reader.GetAttrib();
    const auto &shapes = reader.GetShapes();
    const auto &materials = reader.GetMaterials();

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
                faceNormal = glm::vec3(0, 0, 1);

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
            n = glm::vec3(0, 0, 1);
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

            auto readPos = [&](tinyobj::index_t idx)
            {
                return glm::vec3(
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]);
            };

            glm::vec3 v0 = readPos(idx0);
            glm::vec3 v1 = readPos(idx1);
            glm::vec3 v2 = readPos(idx2);

            glm::vec3 n0 = vertexNormals[idx0.vertex_index];
            glm::vec3 n1 = vertexNormals[idx1.vertex_index];
            glm::vec3 n2 = vertexNormals[idx2.vertex_index];

            auto readUV = [&](tinyobj::index_t idx)
            {
                if (idx.texcoord_index >= 0)
                {
                    float u = attrib.texcoords[2 * idx.texcoord_index + 0];
                    float v = attrib.texcoords[2 * idx.texcoord_index + 1];
                    v = 1.0f - v;
                    return glm::vec2(u, v);
                }
                return glm::vec2(0.f, 0.f);
            };

            glm::vec2 uv0 = readUV(idx0);
            glm::vec2 uv1 = readUV(idx1);
            glm::vec2 uv2 = readUV(idx2);

           
            glm::vec4 faceColor(1.f, 1.f, 1.f, 1.f);
            glm::vec3 matAmbient(1.f, 1.f, 1.f);
            glm::vec3 matSpecular(1.f, 1.f, 1.f);
            float matShininess = 32.f;
            bool hasMtl = false;

            int matId = -1;
            if (f < shape.mesh.material_ids.size())
                matId = shape.mesh.material_ids[f];
            if (matId >= 0 && matId < static_cast<int>(materials.size()))
            {
                const auto &m = materials[static_cast<size_t>(matId)];
               
                faceColor = glm::vec4(m.diffuse[0], m.diffuse[1], m.diffuse[2], 1.0f);
             
                matAmbient = glm::vec3(m.ambient[0], m.ambient[1], m.ambient[2]);
                matSpecular = glm::vec3(m.specular[0], m.specular[1], m.specular[2]);
                matShininess = m.shininess;
                hasMtl = true;
            }

            gty::Vertex3 vert0(v0, n0, uv0, faceColor);
            gty::Vertex3 vert1(v1, n1, uv1, faceColor);
            gty::Vertex3 vert2(v2, n2, uv2, faceColor);

            gty::Triangle tri(vert0, vert1, vert2);
            if (hasMtl)
            {
                tri.matAmbient = matAmbient;
                tri.matSpecular = matSpecular;
                tri.matShininess = matShininess;
                tri.hasMaterialProps = true;
            }

            outMesh.triangles.emplace_back(tri);

            index_offset += fv;
        }
    }

    outMesh.hasDiffuseTexture = false;

    std::string diffuseTexRelPath;
    for (const auto &mat : materials)
    {
        if (!mat.diffuse_texname.empty())
        {
            diffuseTexRelPath = mat.diffuse_texname;
            break;
        }
    }

    if (!diffuseTexRelPath.empty())
    {
        std::string texPath = baseDir + diffuseTexRelPath;
        int texW = 0, texH = 0, texChannels = 0;

        stbi_set_flip_vertically_on_load(false);
        unsigned char *pixels = stbi_load(texPath.c_str(), &texW, &texH, &texChannels, 4);
        if (!pixels)
        {
            std::cerr << "Failed to load texture: " << texPath
                      << " : " << stbi_failure_reason() << "\n";
        }
        else
        {
            outMesh.diffuseTexture.width = texW;
            outMesh.diffuseTexture.height = texH;
            outMesh.diffuseTexture.data.assign(pixels, pixels + texW * texH * 4);
            stbi_image_free(pixels);
            outMesh.hasDiffuseTexture = outMesh.diffuseTexture.IsValid();
        }
    }

    return true;
}
