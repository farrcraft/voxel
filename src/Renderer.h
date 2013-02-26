/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include "Voxel.h"
#include "AssetLoader.h"

#include <hookah/Window.h>

#include <boost/shared_ptr.hpp>

class Program;
class Scene;

/**
 * Main engine renderer
 */
class Renderer
{
	public:
		/**
		 * Default Constructor
		 */
		Renderer(boost::shared_ptr<Scene> scene, AssetLoader loader);

		/**
		 * Draw the frame
		 */
		void draw(Hookah::Window * window);
		/**
		 * Resize the frame
		 */
		void resize(int width, int height);

	protected:

		unsigned int createIndexBuffer(const std::vector<unsigned int> & data);
		unsigned int createVertexBuffer(const std::vector<Vertex> & data);

		void drawVoxel(const Voxel & voxel);
		void drawMesh(unsigned int ebo, unsigned int vbo, unsigned int indices);

	private:
		boost::shared_ptr<Program> program_;

		unsigned int vbo_;
		unsigned int vao_;

		boost::shared_ptr<Scene> scene_;
};
