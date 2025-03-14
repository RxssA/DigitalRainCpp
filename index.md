---
layout: default
---

**Digital Rain** is a C++ console application that simulates the iconic "Matrix" falling code effect. Using multithreading, double buffering, and randomization, the program generates cascading streams of Unicode characters that dynamically change and flow down the screen, mimicking the visual style from the Matrix movies.

![Branching](docs/assets/images/2025-03-1418-05-19-ezgif.com-video-to-gif-converter.gif)
![Branching](docs/assets/images/Screenshot20250308132546.png)

# Features
**Realistic Falling Code Effect** – Uses a list-based system to manage individual character streams.
**Dynamic Character Changes** – Each raindrop element has a chance to mutate into a different symbol.
**Adjustable Rain Stream Size** – The length of each stream varies dynamically for a more organic look.
**Custom Unicode Characters** – Uses a selection of unique symbols to enhance the digital rain aesthetic.

# RainDrop.h
This block of code defines different sets of Unicode characters stored in wchar_t arrays (wide character arrays). These characters are categorized into different themes and can be used for various visual effects.
```c++
const wchar_t unicodeCharacters[]{
		 L'Ƣ',L'Ʃ',L'Ʊ',L'Ƹ',L'ǂ',L'ƽ',L'ǌ',L'Ȣ',L'Ⱦ',L'Ƚ',L'ɑ',L'ɤ',L'ʑ',
	L'ʫ',L'ʥ',L'ʮ',L'λ',L'ε',L'Ϡ',L'Ϟ',L'Ϡ',L'ϰ',L'Є',
	L'ʃ',L'ʘ',L'ʭ',L'ʗ',L'ʡ',L'ʖ',L'ɕ',L'ɺ',L'ɧ',L'ʄ',
	L'β',L'θ',L'ψ',L'ξ',L'ζ',L'ϵ',L'ϑ',
	L'Ж',L'Щ',L'Ѧ',L'Җ',L'Ҩ',L'Ӝ',L'Ԇ',
	L'∑',L'∂',L'∏',L'∩',L'∬',L'⊕',
	L'★',L'♠',L'♣',L'♪',L'♯',L'░',L'▓' 
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
```
This code defines two functions: getCharacterSet and getCharacterSetSize. getCharacterSet returns a pointer to a specific character set based on the provided CharacterSet enum. getCharacterSetSize calculates and returns the size of the specified character set by dividing the total size of the array by the size of its elements
```c++
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
```


This code initializes a random number generator using a random device (rd) and Mersenne Twister (mt), defines a uniform distribution (disI) for selecting random Unicode characters, and sets a maximum depth (maxDepth = 50) for controlling the depth of the rain effect.
```c++
static std::random_device rd; 
static std::mt19937 mt{ rd() };  
static std::uniform_int_distribution<size_t> disI(0, sizeof(unicodeCharacters) / sizeof(unicodeCharacters[0]));
const static size_t maxDepth = 50;
```
This struct represents a single character in a falling rain effect. It stores (x, y) coordinates and a Unicode character (c). The changeRandomly() function updates c based on a random chance. The constructor initializes c with a random character.
```c++
struct RainDropElement
{
	const int x, y;  // x,y coords
	wchar_t c;   // character

	inline void changeRandomly(const float chance)
	{
		static std::uniform_real_distribution<float> localDisF(0.0f, 1.0f);
		if (chance >= localDisF(mt))
			c = unicodeCharacters[disI(mt)];
	}

	inline RainDropElement(const int x, const int y)
		: x(x), y(y), c(L' ')
	{
		changeRandomly(1);
	}
};
```
This code defines a RainDrop class that simulates raindrops in a column. It uses the std::list container for storing individual RainDropElement objects. Key attributes include maxSize (max size of the column), x (x-coordinate), maxDepthY (screen size), changeChance (probability of change), and charSet (character set for display). The constructor initializes these parameters, and the fall method simulates the raindrop falling.
```c++
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
```
get() returns a constant reference to the rainDropElements list, providing access to the falling raindrop elements.
operator++() increments the size of the raindrop column, ensuring it doesn't exceed maxSize by using std::min.
operator--() decrements the size of the raindrop column, ensuring it doesn't go below 1 by using std::max. These operators adjust the size of the column during simulation.
```c++
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
```

#### Header 2
The fall() method in the RainDrop class simulates the falling of raindrops:
It iterates through each droplet in the rainDropElements list and calls changeRandomly() on each, giving them a chance to change based on changeChance.
A new raindrop is added to the top of the screen with push_front.
If the number of raindrop elements exceeds the current size, the oldest element is removed with pop_back.
The y position of the raindrop is incremented. If it reaches the screen's maximum depth (maxDepthY), it resets to the top with  y = 0, simulating the continuous fall of raindrops.
```c++
void matrix::RainDrop::fall() {
	for (auto& droplet : rainDropElements)
		droplet.changeRandomly(changeChance); //iterates through all existing raindrop elements, each has a changce to change (changeChance)
	rainDropElements.push_front({ x, y, charSet, charSetSize });  // add rain drop at the top of screen

	if (rainDropElements.size() > static_cast<size_t>(size)) {
		rainDropElements.pop_back();  // removes characters that exceed size of screen.
	}
	 
	if (++y >= maxDepthY) // makes rain fall
		y = 0;  // reset
}
```
##### Header 5

1.  This is an ordered list following a header.
2.  This is an ordered list following a header.
3.  This is an ordered list following a header.

###### Header 6

| head1        | head two          | three |
|:-------------|:------------------|:------|
| ok           | good swedish fish | nice  |
| out of stock | good and plenty   | nice  |
| ok           | good `oreos`      | hmm   |
| ok           | good `zoute` drop | yumm  |

### There's a horizontal rule below this.

* * *

### Here is an unordered list:

*   Item foo
*   Item bar
*   Item baz
*   Item zip

### And an ordered list:

1.  Item one
1.  Item two
1.  Item three
1.  Item four

### And a nested list:

- level 1 item
  - level 2 item
  - level 2 item
    - level 3 item
    - level 3 item
- level 1 item
  - level 2 item
  - level 2 item
  - level 2 item
- level 1 item
  - level 2 item
  - level 2 item
- level 1 item

### Small image

![Octocat](https://github.githubassets.com/images/icons/emoji/octocat.png)

### Large image

![Branching](https://guides.github.com/activities/hello-world/branching.png)


### Definition lists can be used with HTML syntax.

<dl>
<dt>Name</dt>
<dd>Godzilla</dd>
<dt>Born</dt>
<dd>1952</dd>
<dt>Birthplace</dt>
<dd>Japan</dd>
<dt>Color</dt>
<dd>Green</dd>
</dl>

```
Long, single-line code blocks should not wrap. They should horizontally scroll if they are too long. This line should be long enough to demonstrate this.
```

```
The final element.
```
