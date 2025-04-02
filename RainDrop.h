#ifndef RAIN_DROP_H
#define RAIN_DROP_H

/*
	Digital Rain
	Ross Amritage
	G00401655
	16/02/2025
*/

#include <random> 
#include <list>


namespace matrix
{

	const wchar_t unicodeCharacters[]{
			 L'Ƣ',L'Ʃ',L'Ʊ',L'Ƹ',L'ǂ',L'ƽ',L'ǌ',L'Ȣ',L'Ⱦ',L'Ƚ',L'ɑ',L'ɤ',L'ʑ',
		L'ʫ',L'ʥ',L'ʮ',L'λ',L'ε',L'Ϡ',L'Ϟ',L'Ϡ',L'ϰ',L'Є',
		L'ʃ',L'ʘ',L'ʭ',L'ʗ',L'ʡ',L'ʖ',L'ɕ',L'ɺ',L'ɧ',L'ʄ',
		L'β',L'θ',L'ψ',L'ξ',L'ζ',L'ϵ',L'ϑ',
		L'Ж',L'Щ',L'Ѧ',L'Җ',L'Ҩ',L'Ӝ',L'Ԇ',
		L'∑',L'∂',L'∏',L'∩',L'∬',L'⊕',
		L'★',L'♠',L'♣',L'♪',L'♯',L'░',L'▓'     // got chatGPT to generate unicode characters similiar to matrix movie
	};
	const wchar_t unicodeCharactersSnow[]
	{
		L'✼',L'❋',L'❊'
	};
	const wchar_t unicodeCharactersRain[]
	{
		L'░',L'▓'
	};
	const wchar_t unicodeCharactersDiamonds[]
	{
		L'⬖',L'⬗',L'⬘',L'⬙',L'◈'
	};


	static std::random_device rd; // random device to seed MT
	static std::mt19937 mt{ rd() };   //Mersenne Twister 19937 https://en.wikipedia.org/wiki/Mersenne_Twister

	/*
	 rand() % n is biased apparently so using uniform_int_distribution instead.
	 https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	*/
	static std::uniform_int_distribution<size_t> disI(0, sizeof(unicodeCharacters) / sizeof(unicodeCharacters[0]));
	const static size_t maxDepth = 50;

	struct RainDropElement
	{
		const int x, y;  // x,y coords
		wchar_t c;   // character
		const wchar_t* charSet;
		size_t charSetSize;

		inline void changeRandomly(const float chance) {
			static std::uniform_real_distribution<float> localDisF(0.0f, 1.0f); // rand num 
			if (chance >= localDisF(mt)) {  // If the generated value is less than or equal to chance, change c
				std::uniform_int_distribution<size_t> disI(0, charSetSize - 1); // random index within the range [0, charSetSize - 1]
				c = charSet[disI(mt)]; //picks rand char from charSet
			}
		}

		inline RainDropElement(const int x, const int y, const wchar_t* charSet, size_t charSetSize)
			: x(x), y(y), c(L' '), charSet(charSet), charSetSize(charSetSize) // Member initializer list https://en.cppreference.com/w/cpp/language/constructor
		{
			changeRandomly(1);
		}
	};

	class RainDrop {
	public:
		using raindrops_t = std::list<RainDropElement>;
	private:
		raindrops_t rainDropElements;  // falling charcters
		const int maxSize; // size of column of rain
		int size; // current size of colunm
		const int x;
		const int maxDepthY; // screen size
		float changeChance{ 0.5 }; // 50% chance of change
		int y{ 0 };
		const wchar_t* charSet;
		size_t charSetSize;

	public:
		RainDrop(int maxSize, int x, int maxDepthY, float changeChance, const wchar_t* charSet, size_t charSetSize)
			: maxSize(maxSize), x(x), y(0), size(maxSize / 2), maxDepthY(maxDepthY), changeChance(changeChance),
			charSet(charSet), charSetSize(charSetSize) {
		}

		void fall();


		inline const raindrops_t& get() const
		{
			return rainDropElements;
		}
		inline void operator++()  //increases raindrop size
		{
			size = std::min(maxSize, size + 1);
		}

		inline void operator--()  // decreases raindrop saize
		{
			size = std::max(1, size - 1);
		}

	};

	enum class CharacterSet {
		Standard,
		Snow,
		Diamonds,
		Rain
	};

	inline const wchar_t* getCharacterSet(CharacterSet set) {
		switch (set)
		{
		case matrix::CharacterSet::Standard:
			return unicodeCharacters;
			break;
		case matrix::CharacterSet::Snow:
			return unicodeCharactersSnow;
			break;
		case matrix::CharacterSet::Diamonds:
			return unicodeCharactersDiamonds;
			break;
		case matrix::CharacterSet::Rain:
			return unicodeCharactersRain;
			break;
		default:
			return unicodeCharacters;
			break;
		}
	}

	static size_t getCharacterSetSize(CharacterSet set) {
		switch (set)
		{
		case matrix::CharacterSet::Standard:
			return sizeof(unicodeCharacters) / sizeof(unicodeCharacters[0]);
			
		case matrix::CharacterSet::Snow:
			return sizeof(unicodeCharactersSnow) / sizeof(unicodeCharactersSnow[0]);
			
		case matrix::CharacterSet::Diamonds:
			return sizeof(unicodeCharactersDiamonds) / sizeof(unicodeCharactersDiamonds[0]);
			
		case matrix::CharacterSet::Rain:
			return sizeof(unicodeCharactersRain) / sizeof(unicodeCharactersRain[0]);
			
		default:
			return 0;
		}
	}

	

}

#endif // RAIN_DROP_H