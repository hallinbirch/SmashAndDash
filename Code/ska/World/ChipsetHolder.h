#pragma once
#include <unordered_map>
#include <memory>
#include "../Graphic/Point.h"
#include "../Graphic/Color.h"
#include "Chipset.h"
namespace ska {
	class Block;
	typedef std::unique_ptr<Block> BlockPtr;
	typedef std::unique_ptr<Chipset> ChipsetPtr;
	class SDLSurface;
	class ChipsetHolder {
	public:
		ChipsetHolder(const std::string& correspondanceFile);
		bool attach(const unsigned int blockSize, const std::string& chipsetName);
		Texture& getChipset();
		const std::string& getName() const;
		BlockPtr generateBlock(ska::Color& key);
		~ChipsetHolder() = default;

	private:
		void buildCorrMap(const ska::SDLSurface& fichierMCorr);
		std::unordered_map<ska::Color, ska::Point<int>> m_corr;
		ChipsetPtr m_chipset;
		unsigned int m_corrFileWidth;
		std::string m_currentChipsetName;
	};
}