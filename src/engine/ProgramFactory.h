/**
 * (c) Joshua Farr <j.wgasa@gmail.com>
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <string>

#include <gl/Program.h>

class AssetLoader;

class ProgramFactory
{
	public:
		ProgramFactory(boost::shared_ptr<AssetLoader> loader);

		boost::shared_ptr<v3D::Program> create(unsigned int shaderTypes, const std::string & name);

	private:
		boost::shared_ptr<AssetLoader> loader_;
};
