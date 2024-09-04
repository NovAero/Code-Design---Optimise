#pragma once
#include "AABB.h"

class Critter;

class Quadtree {
public:
	static const int m_capacity = 2;
	enum SUBTREE {
		TOP_LEFT = 0,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};
public:

	Quadtree(const int width, const int height);
	Quadtree(AABB boundary);

	~Quadtree();

	bool Insert(Critter*);
	void Subdivide();

	void Update(float deltaTime);
	void Draw();

private:

	Quadtree** m_children;
	AABB m_boundary;
	Critter** m_objects;
};


