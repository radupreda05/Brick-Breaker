#include "Objects.h"

Objects::Objects() {}

Objects::Objects(std::string &name, glm::vec2 &pos)
	: name(name), pos(pos), id(0) {}

void Objects::ChangePos(glm::vec2 &newPos) {
	pos = newPos;
}

std::string& Objects::GetName() { return name; }
glm::vec2& Objects::GetPos() { return pos; }
bool Objects::GetExistence() { return existence; }
void Objects::SetExistence(bool existence) { this->existence = existence;}