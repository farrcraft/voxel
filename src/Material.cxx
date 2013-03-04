#include "Material.h"
#include "Program.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

Material::Material(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, float shininess) :
	ambient_(ambient),
	specular_(specular),
	diffuse_(diffuse),
	shininess_(shininess)
{
}


void Material::program(boost::shared_ptr<Program> program)
{
	program->enable();
	unsigned int ambient = program->uniform("Material.Ka");
	unsigned int diffuse = program->uniform("Material.Kd");
	unsigned int specular = program->uniform("Material.Ks");
	unsigned int shininess = program->uniform("Material.Shininess");

	glUniform3fv(ambient, 1, glm::value_ptr(ambient_));
	glUniform3fv(diffuse, 1, glm::value_ptr(diffuse_));
	glUniform3fv(specular, 1, glm::value_ptr(specular_));
	glUniform1f(shininess, shininess_);
}
