#pragma once
#include "Utils.h"

namespace Collision
{
    static double Dot(const Vertex& v1, const Vertex& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    static void Project(const Vertices& vertices, const Vertex& axis, double& max, double& min)
    {
        for (const Vertex& v : vertices)
        {
            double projection = Dot(v, axis);

            if (projection < min) min = projection;
            if (projection > max) max = projection;
        }
    }
    static bool Intersection(const Vertices& verticesA, const Vertices& verticesB)//returns true if figures intersect
    {
        if (verticesA.empty() || verticesB.empty()) return false;
        for (int i = 0; i < verticesA.size(); ++i)
        {
            Vertex v1 = verticesA[i];
            Vertex v2 = verticesA[(i + 1) % verticesA.size()];

            Vertex edge = v2 - v1;
            Vertex axis(-edge.y, edge.x); //creating a normal

            double minA = DBL_MAX;
            double maxA = DBL_MIN;
            double minB = DBL_MAX;
            double maxB = DBL_MIN;

            Project(verticesA, axis, maxA, minA);//projecting 1st rectangle
            Project(verticesB, axis, maxB, minB);//projecting 2nd rectangle

            if (minA >= maxB || minB >= maxA) return false;

        }

        for (int i = 0; i < verticesB.size(); ++i) // doing the same for other rect
        {
            Vertex v1 = verticesB[i];
            Vertex v2 = verticesB[(i + 1) % verticesB.size()];

            Vertex edge = v2 - v1;
            Vertex axis(-edge.y, edge.x); //creating a normal

            double minA = DBL_MAX;
            double maxA = DBL_MIN;
            double minB = DBL_MAX;
            double maxB = DBL_MIN;

            Project(verticesA, axis, maxA, minA);
            Project(verticesB, axis, maxB, minB);

            if (minA >= maxB || minB >= maxA) return false;

        }
        return true;
    }
};

