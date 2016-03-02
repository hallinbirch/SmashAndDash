#ifndef DEF_TEXTURE
#define DEF_TEXTURE

#include <map>
#include <memory>
#include "../Utils\ResourceTemplate.h"
#include "SDLTexture.h"
#include "TextureData.h"
#include "../Utils/SkaConstants.h"
#include "Rectangle.h"
#include "./GUI/Window.h"

//opérateur < pour SDL_Color nécessaire pour le stockage dans la map
inline bool operator<(const SDL_Color &a, const SDL_Color &b)
{
	return a.a < b.a;
}

namespace ska {
	class Texture : public ResourceTemplate<ska::SDLTexture, ska::TextureData>
	{

	public:
		Texture(std::string id, int r = DEFAULT_T_RED, int g = DEFAULT_T_GREEN, int b = DEFAULT_T_BLUE, int a = -1);
		Texture(std::string id, SDL_Color finalColor);
		Texture();
		void loadFromText(unsigned int fontSize, std::string text, SDL_Color c);
		void load(std::string id, int r = DEFAULT_T_RED, int g = DEFAULT_T_GREEN, int b = DEFAULT_T_BLUE, int a = -1);
		static void setDefaultWindow(ska::Window* w);
		static void freeAll();
		virtual ~Texture();

		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blending);
		Texture clone();
		void setAlpha(Uint8 alpha);
		int render(int x, int y, ska::Rectangle* clip = NULL);

		int getWidth() const;
		int getHeight() const;

	private:
		static void checkWindow();
		static ska::Window* m_window;
	};
}




#endif