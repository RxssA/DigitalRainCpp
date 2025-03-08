---
layout: default
---

**Digital Rain** is a C++ console application that simulates the iconic "Matrix" falling code effect. Using multithreading, double buffering, and randomization, the program generates cascading streams of Unicode characters that dynamically change and flow down the screen, mimicking the visual style from the Matrix movies.
![Branching](https://github.githubassets.com/images/Screenshot20250308132546.png)

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
### Header 3

```c++
void flip()
{
	WriteConsoleOutput(
		buffer[++bufferIndex & 0x01],  //selects next buffer
		charInfoBuffer,
		bufferSize,
		{ 0, 0 },
		&writeRect);
	SetConsoleActiveScreenBuffer(buffer[bufferIndex & 0x01]);
}
```

```ruby
# Ruby code with syntax highlighting
GitHubPages::Dependencies.gems.each do |gem, version|
  s.add_dependency(gem, "= #{version}")
end
```

#### Header 4

*   This is an unordered list following a header.
*   This is an unordered list following a header.
*   This is an unordered list following a header.

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
