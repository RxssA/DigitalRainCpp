/*
	Digital Rain
	Ross Amritage
	G00401655
	16/02/2025
*/

#include "RainDrop.h"

void matrix::RainDrop::fall() {
	for (auto& droplet : rainDropElements)
		droplet.changeRandomly(changeChance); //iterates through all existing raindrop elements, each has a changce to change (changeChance)
	rainDropElements.push_front({ x,y });  // add rain drop at the top of screen

	if (rainDropElements.size() > static_cast<size_t>(size)) {
		rainDropElements.pop_back();  // removes characters that exceed size of screen.
	}

	if (++y >= maxDepthY) // makes rain fall
		y = 0;  // reset
}
