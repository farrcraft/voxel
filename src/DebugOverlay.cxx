#include "DebugOverlay.h"

#include "AssetLoader.h"

#include "engine/Program.h"

#include "font/TextBuffer.h"
#include "font/TextureFont.h"
#include "font/FontCache.h"
#include "font/TextureAtlas.h"


DebugOverlay::DebugOverlay(boost::shared_ptr<Program> shaderProgram, boost::shared_ptr<AssetLoader> loader) :
	enabled_(false)
{
	// setup text buffer
	text_.reset(new TextBuffer(shaderProgram, TextBuffer::LCD_FILTERING_ON));

	TextBuffer::Markup markup;
	markup.bold_ = false;
	markup.italic_ = false;
	markup.rise_ = 0.0f;
	markup.spacing_ = 0.0f;
	markup.gamma_ = 1.5f;
	markup.underline_ = false;
	markup.overline_ = false;
	markup.strikethrough_ = false;
	markup.foregroundColor_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	markup.backgroundColor_ = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	markup.size_ = 24.0f;

	// characters to cache
	const wchar_t *charcodes =  L" !\"#$%&'()*+,-./0123456789:;<=>?"
								L"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
								L"`abcdefghijklmnopqrstuvwxyz{|}~";
	text_->cache()->charcodes(charcodes);

	std::string filename = loader->path() + std::string("Vera.ttf");
	markup.font_ = text_->cache()->load(filename, markup.size_);

	glm::vec2 pen(20.0f, 200.0f);
	text_->addText(pen, markup, L"Now is the time for all good men");
	text_->upload();
}


void DebugOverlay::enable(bool status)
{
	enabled_ = status;
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

void DebugOverlay::update()
{
	/*
		Debug screen:
		Voxel 0.0.1 (60 fps)

		x:
		y:
		z:
	*/
}
