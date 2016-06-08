#include <Colorduino.h>

#define SEED_ROUNDS			1000	// number of rounds for initial rng seeding
#define DELAY				1000	// delay between steps
#define AUTO_RESET			120		// auto reset after # of steps
#define RESET_ALIVE_RATIO	40		// chance of randomly generated cell to be alive in %
#define MINIMAL_BRIGHTNESS	8		// minimal total brightness for random color

long totalRounds = 0;				// total # of steps performed

// This method will run only once and contains initialization code
void setup() {
	// Initialize display
	Colorduino.Init();

	// Set white balance
	unsigned char whiteBalVal[3] = { 16, 63, 63 };
	Colorduino.SetWhiteBal(whiteBalVal);

	// Initialize and seed the random number generator
	// See note in getRandomSeed method!
	randomSeed(getRandomSeed());

	// Randomly fill the screen
	setRandomState();
	Colorduino.FlipPage();
}

// This method will run in infinite loop and contains code for updates
void loop() {
	// Copy display buffer to offscreen one
	for (int i = 0; i < ColorduinoScreenWidth * ColorduinoScreenHeight; i++) {
		Colorduino.curWriteFrame[i] = Colorduino.curDrawFrame[i];
	}

	// Increment total rounds counter
	totalRounds++;

	// Compute new state
	int totalChanges = 0;
	for (int x = 0; x < ColorduinoScreenWidth; x++) {
		for (int y = 0; y < ColorduinoScreenHeight; y++) {
			bool oldState = getCurrentState(x, y);
			bool newState = getNewState(x, y);

			if (oldState != newState) {
				// State was changed
				totalChanges++;
				if (newState) {
					setPixelAlive(x, y);
				} else {
					setPixelDead(x, y);
				}
			}
		}
	}

	// If field reached stable state or auto rounds threshold, reset with new seed
	if (totalChanges == 0 || totalRounds % AUTO_RESET == 0) {
		setRandomState();
	}

	// Wait before performing change
	delay(DELAY);
	Colorduino.FlipPage();
}

// Computes new state of cell, based on life of its neighbors
bool getNewState(int x, int y) {
	// Get number of alive neighbours
	int aliveNeighbors = 0;
	if (getCurrentState(x - 1, y - 1)) aliveNeighbors++;
	if (getCurrentState(x - 1, y)) aliveNeighbors++;
	if (getCurrentState(x - 1, y + 1)) aliveNeighbors++;
	if (getCurrentState(x, y - 1)) aliveNeighbors++;
	if (getCurrentState(x, y + 1)) aliveNeighbors++;
	if (getCurrentState(x + 1, y - 1)) aliveNeighbors++;
	if (getCurrentState(x + 1, y)) aliveNeighbors++;
	if (getCurrentState(x + 1, y + 1)) aliveNeighbors++;

	if (getCurrentState(x, y)) {
		// Currently alive
		if (aliveNeighbors < 2) return false;	// died from underpopulation
		if (aliveNeighbors > 3) return false;	// died from overpopulation
		return true;							// remain alive
	} else {
		// Currently dead
		if (aliveNeighbors == 3) return true;	// alive by reproduction
		return false;							// remain dead
	}
}

// Gets current state of cell and handles x, y out of bounds
bool getCurrentState(int x, int y) {
	// Shift x, y if out of bounds
	if (x < 0) x = ColorduinoScreenWidth - 1;
	if (y < 0) y = ColorduinoScreenHeight - 1;
	if (x >= ColorduinoScreenWidth) x = 0;
	if (y >= ColorduinoScreenHeight) y = 0;

	// Read pixel status
	PixelRGB* status = Colorduino.GetDrawPixel(x, y);

	// Return if its black (false) or not (true)
	return (status->r | status->g | status->b) > 0;
}

// Tries to create seed for random number generator
unsigned long getRandomSeed() {
	// This is actually very bad way to get random seed, but we simply don't
	// have any others left, since the board does not have any useable ports
	// or source of entropy. The A6/A7 values seem to fluctulate a bit, so
	// I'm using them. It's probably good for Game of Life, but don't do that
	// for anything more important

	unsigned long seed = 0;
	for (int i = 0; i < SEED_ROUNDS; i++) {
		seed += analogRead(A6) * analogRead(A7);
	}
	return seed;
}

// Sets the system to fully random (initial) state
void setRandomState() {
	// Initialize screen with random state
	for (int x = 0; x < ColorduinoScreenWidth; x++) {
		for (int y = 0; y < ColorduinoScreenHeight; y++) {
			bool isAlive = random(1, 101) <= RESET_ALIVE_RATIO;
			if (isAlive) {
				setPixelAlive(x, y);
			} else {
				setPixelDead(x, y);
			}
		}
	}
}

// Sets the pixel as dead (turns it off)
void setPixelDead(int x, int y) {
	// Turn off the LED
	Colorduino.SetPixel(x, y, 0x00, 0x00, 0x00);
}

// Sets the pixel as alive (lights random color)
void setPixelAlive(int x, int y) {
	// Turn on the LED
	byte r = 0, g = 0, b = 0;

	// Get reasonably bright random color (total value must be over 16)
	while (r + g + b < MINIMAL_BRIGHTNESS) {
		r = random();
		g = random();
		b = random();
	}

	// Turn on the LED
	Colorduino.SetPixel(x, y, r, g, b);
}