/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "DebugOverlay.h"
#include "AssetLoader.h"
#include "Version.h"
#include "Scene.h"
#include "engine/Program.h"
#include "font/TextureFont.h"
#include "font/FontCache.h"
#include "font/TextureAtlas.h"

#include <string>
#include <sstream>

DebugOverlay::DebugOverlay(boost::shared_ptr<Scene> scene, boost::shared_ptr<Program> shaderProgram, boost::shared_ptr<AssetLoader> loader) :
	scene_(scene),
	enabled_(false)
{
	// setup text buffer
	text_.reset(new TextBuffer(shaderProgram, TextBuffer::LCD_FILTERING_ON));

	markup_.bold_ = false;
	markup_.italic_ = false;
	markup_.rise_ = 0.0f;
	markup_.spacing_ = 0.0f;
	markup_.gamma_ = 0.5f;
	markup_.underline_ = false;
	markup_.overline_ = false;
	markup_.strikethrough_ = false;
	markup_.foregroundColor_ = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	markup_.backgroundColor_ = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	markup_.size_ = 24.0f;

	// characters to cache
	const wchar_t *charcodes =  L" !\"#$%&'()*+,-./0123456789:;<=>?"
								L"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
								L"`abcdefghijklmnopqrstuvwxyz{|}~";
	text_->cache()->charcodes(charcodes);

	//std::string filename = loader->path() + std::string("fonts/DroidSerif-Regular.ttf");
	std::string filename = loader->path() + std::string("fonts/Vera.ttf");
	markup_.font_ = text_->cache()->load(filename, markup_.size_);
}


void DebugOverlay::enable(bool status)
{
	enabled_ = status;
	if (enabled_)
	{
		// reset fps counters
		frames_ = 0;
		elapsed_ = 0;

		update(0);
	}
}

bool DebugOverlay::enabled() const
{
	return enabled_;
}

void DebugOverlay::render()
{
	if (!enabled_)
	{
		return;
	}
	text_->render();
}

const unsigned int samples = 100;
unsigned int tickindex = 0;
unsigned int ticksum = 0;
unsigned int ticklist[samples];

/* need to zero out the ticklist array before starting */
/* average will ramp up until the buffer is full */
/* returns average ticks per frame over the MAXSAMPPLES last frames */

unsigned int averageTick(unsigned int newtick)
{
    ticksum -= ticklist[tickindex];  /* subtract value falling off */
    ticksum += newtick;              /* add new value */
    ticklist[tickindex] = newtick;   /* save new value so it can be subtracted later */
    if(++tickindex == samples)    /* inc buffer index */
        tickindex = 0;

    /* return average */
    return ticksum / samples;
}

void DebugOverlay::update(unsigned int delta)
{
	glm::vec2 pen(20.0f, 50.0f);

	frames_++;
	elapsed_ += delta;

	text_->clear();
	std::stringstream info;
	unsigned int fps = averageTick(delta);//frames_ / elapsed_ * 1000;
	info << "Voxel " << VOXEL_VERSION << "(" << fps << /* ", " << elapsed_ << ", " << frames_ << */ ")" << std::endl;
	info << std::endl;
	glm::vec3 playerPosition = scene_->player()->position();
	info << "x: " << playerPosition.x << std::endl;
	info << "y: " << playerPosition.y << std::endl;
	info << "z: " << playerPosition.z << std::endl;

	std::string buffer = info.str();
	std::wstring widestr = std::wstring(buffer.begin(), buffer.end());

	text_->addText(pen, markup_, widestr.c_str());
	text_->upload();
}
