#pragma once

class Piano
{
	public:
		Cuboid key, lever, damperarm, damper, hammerarm, hammer;
		Tetrahedron pivot;
		Cylinder wire;

		Piano();
		Piano(std::string);
		Shape getObjectByIndex(int);
		int getIndexByObject(std::string);
};