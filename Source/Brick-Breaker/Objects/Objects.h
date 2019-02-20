#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>

class Objects {
public:
	// Used for knowing what type of object we are dealing with
	int id;
public:
	Objects();
	Objects(std::string &name, glm::vec2 &pos);
	
	// Getters and setters only
	std::string& GetName();
	glm::vec2& GetPos();
	void ChangePos(glm::vec2 &newPos);
	bool GetExistence();
	void SetExistence(bool existence);
protected:
	bool existence;
	std::string name;
	glm::vec2 pos;
};