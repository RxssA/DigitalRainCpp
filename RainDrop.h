/*
	Digital Rain
	Ross Amritage
	G00401655
	16/02/2025
*/

#pragma once
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

	static std::random_device rd; // random device to seed MT
	static std::mt19937 mt{ rd() };   //Mersenne Twister 19937

	/*
	 rand() % n is biased apparently so using uniform_int_distribution instead.
	 https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	*/
	static std::uniform_int_distribution<size_t> disI(0, sizeof(unicodeCharacters) / sizeof(unicodeCharacters[0]));
	const static size_t maxDepth = 50;

	struct RainDropElement {

		inline void changeRandomly(const float chance) {
			const int x, y;
			wchar_t c;

			static std::uniform_real_distribution<float> localDisF(0.0f, 1.0f);
			if ((chance >= localDisF(mt)) {
				c = unicodeCharacters[disI(mt)];
			}
		}
		inline RainDropElement(const int x, const int y) : x(x) y(y), c(L' ') {
			changeRandomly(1);
		}

	};

				class RainDrop {
				public:
					using raindrops_t = std::list<RainDropElement>;
				private:
					const int maxSize;
					int size;
					const int x;
					const int maxDepthY;
					float changeChance{ 0.5 };
					int y{ 0 };

				public:
					RainDrop(int maxSize, int x, int maxDepthY, float changeChance) : maxSize(maxSize), x(x), y(0),
						size(maxSize / 2),
						maxDepthY(maxDepthY),
						changeChance(changeChance) {}

					void fall();


	};

}