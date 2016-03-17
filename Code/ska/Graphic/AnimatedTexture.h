#pragma once
#include "Animation.h"
#include "Texture.h"
#include "Rectangle.h"
#include "Point.h"
#include <memory>

namespace ska {
	class AnimatedTexture
	{
	public:
		AnimatedTexture();
		
		void loadFromText(unsigned int fontSize, std::string text, SDL_Color c, const unsigned int horizontalFrames, const unsigned int verticalFrames, const unsigned int animatedFrames, bool isVertical = false);
		void load(std::string id, const unsigned int horizontalFrames, const unsigned int verticalFrames, const unsigned int animatedFrames, bool isVertical = false, int r = DEFAULT_T_RED, int g = DEFAULT_T_GREEN, int b = DEFAULT_T_BLUE, int a = -1);
		
		void stop(const bool x);
		void nextFrame();
		
		int render(int x, int y);

		void setDelay(const unsigned int delay);
		void setAlpha(int alpha);
		void setColor(int red, int green, int blue);
		void setOffset(const Point<int>& offset);

		const unsigned int getWidth() const;
		const unsigned int getHeight() const;

		void operator=(const AnimatedTexture& text);

	private:
		void recalculateFrames(const unsigned int horizontalFrames, const unsigned int verticalFrames, const unsigned int animatedFrames, const bool isVertical);
		Animation m_anim;
		Texture m_sprite;
	};
	typedef std::unique_ptr<AnimatedTexture> AnimatedTexturePtr;
}
