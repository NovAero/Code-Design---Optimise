#pragma once
#include "d_array.h"

class Graph {

public:
	struct Vertex;
	struct Edge {
	public:
		Edge(Vertex* v = nullptr) : target{ v } {}
		virtual ~Edge() {}

		Vertex* target;

		void* userData = nullptr;

	};
	struct Vertex {
	public:
		Vertex() {}
		virtual ~Vertex() {}

		d_array<Edge*> edges;

	};

public:
	void AddVertex(Vertex* v) {
		vertices.PushBack(v);
	}
	void RemoveVertex() {
		 
	}
	void AddEdge(Vertex* v, Vertex* v1) {

		Edge* e = new Edge(v1);
		v->edges.PushBack(e);
	}
	void RemoveEdge(Edge* e) {
		for (auto it = vertices.Begin(); it != vertices.End(); ++it) {
			if (e->target = *it) {

			}
		}
	}

private:

	d_array<Vertex*> vertices;
};