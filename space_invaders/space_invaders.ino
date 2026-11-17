#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();

class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      strength = 0;
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_strength() const {
      return strength;
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
    }
    
    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
    }
    
    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
    }    
    
    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      
      strength--;
      invader.erase();
      if (strength != 0) {
        invader.draw();
      }

    }

  private:
    int x;
    int y;
    int strength;
    
    /* 
    * Requires: body_color & eye_color to be valid colors
    *
    * Modifies: Matrix

    * Effects: 
    * Draws the invader using x & y coordinates. 
    * The project spec recommended making the (0,0) coordinate the top
    * left corner of the pixel screen, so the x and y coordinates of the invader
    * also start at the top left corner of the invader.
    * The invader should look like this:
    * 0 - the x,y coordinate of the invader (NOT DRAWN ON SCREEN)
    * # - body pixels * - eye pixels
    * 
    *   0##
    *   #**#
    *   ####
    *   #  #
    * 
    * NOTE: This code may not work as it assumes the top left corner of the pixel screen is (0,0)
    *       and to move right and down on the screen you would need to add to x and y, so 
    *       if the orientation is actually different this function will not work
    * - Nate
    */
    void draw_with_rgb(Color body_color, Color eye_color) {

      //Draws top row
      matrix.drawPixel(x+1, y, body_color.to_333());
      matrix.drawPixel(x+2, y, body_color.to_333());

      //Draws eye row
      matrix.drawPixel(x, y+1, body_color.to_333());
      matrix.drawPixel(x+1, y+1, eye_color.to_333());
      matrix.drawPixel(x+2, y+1, eye_color.to_333());
      matrix.drawPixel(x+3, y+1, body_color.to_333());

      //Draws mid bottom row
      matrix.drawPixel(x, y+2, body_color.to_333());
      matrix.drawPixel(x+1, y+2, body_color.to_333());
      matrix.drawPixel(x+2, y+2, body_color.to_333());
      matrix.drawPixel(x+3, y+2, body_color.to_333());

      //Draws legs
      matrix.drawPixel(x, y+3, body_color.to_333());
      matrix.drawPixel(x+3, y+3, body_color.to_333());
    }
};

class Cannonball {
  public:
    Cannonball() {
      x = 0;
      y = 0;
      fired = false;
    }
    
    // resets private data members to initial values
    void reset() {
      x = 0;
      y = 0;
      fired = false;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y; 
    }
    bool has_been_fired() const {
      return fired;
    }
    
    // sets private data members
    void fire(int x_arg, int y_arg) {
    }
    
    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
    }
    
    // resets private data members to initial values
    void hit() {
      ball = Cannonball();
    }
    
    // draws the Cannonball, if it is fired
    void draw() {
      /*
      * I assume that this may be more complicated based on moving the cannonball,
      * but drawing the cannonball based on its x and y coordinates with the color orange is done by this code:
      * 
      * matrix.drawPixel(x, y, ORANGE.to_333());
      * matrix.drawPixel(x, y+1, ORANGE.to_333());
      * 
      * This draws the cannon ball like this:
      *   0
      *   #
      * With the top of the cannonball being represented by x,y
      * and the bottom being represented by x, y+1.
      * This also may not work based on the orientation of the pixel screen
      * - Nate
      */
    }
    
    // draws black where the Cannonball used to be
    void erase() {
      // To erase the previous cannonball, use matrix.drawPixel() and color BLACK at the previous x and y coordinates - Nate
    }

  private:
    int x;
    int y;
    bool fired;
};

class Player {
  public:
    Player() {
      x = 0;
      y = 0;
      lives = 3;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_lives() const {
      return lives;
    }
    
    // setter
    void set_x(int x_arg) {
      x = x_arg;
    }
    
    // Modifies: lives
    void die() {
      lives--;
    }
    
    // draws the Player
    // calls: draw_with_rgb
    void draw() {
    }
    
    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    

    /* 
    * Requires: color to be a valid color
    *
    * Modifies: Matrix

    * Effects: 
    * Draws the player using x & y coordinates. 
    * The anchor cordinate of the player is the tip of the cannon, with the the rest of
    * the cannon drawn around that coordinate
    * The player should look like this:
    * 0 - the x,y coordinate of the player (DRAWN ON SCREEN TOO (as apposed to the invader class version))
    * # - pixels
    * 
    *    0
    *   ###
    * 
    * NOTE: This code may not work as it assumes the top left corner of the pixel screen is (0,0)
    *       and to move right and down on the screen you would need to add to x and y, so 
    *       if the orientation is actually different this function will not work properly
    *       Also, the player should be able to move its center portion all the way to the edge of the screen,
    *       meaning that the right or left side of the player will be off the screen. The way the code is 
    *       written right now may result in an out-of-bounds error but we'll address that when we run into it
    * -Nate 
    */
    void draw_with_rgb(Color color) {
      //Draws top part
      matrix.drawPixel(x, y, color.to_333());

      //Draws base
      matrix.drawPixel(x-1, y+1, color.to_333());
      matrix.drawPixel(x, y+1, color.to_333());
      matrix.drawPixel(x+1, y+1, color.to_333());

    }
};

class Game {
  public:
    Game() {
      level = 0;
      time = 0;
    }
    
    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    void setupGame() {
    }
    
    // advances the game simulation one step and renders the graphics
    // see spec for details of game
    // Modifies: global variable matrix
    void update(int potentiometer_value, bool button_pressed) {
    }

  private:
    int level;
    unsigned long time;
    Player player;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];

    // check if Player defeated all Invaders in current level
    bool level_cleared() {
    }

    // set up a level
    void reset_level() {
    }
};

// a global variable that represents the game Space Invaders
Game game;

// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);

  game.update(potentiometer_value, button_pressed);


  //TESTS PRINTING FUNCTIONS
  print_level(1);
  delay(500);
  //Clears screen
  matrix.fillScreen(BLACK.to_333());

  print_lives(3);
  delay(500);
  //Clears screen
  matrix.fillScreen(BLACK.to_333());

  game_over();
  delay(500);
  //Clears screen
  matrix.fillScreen(BLACK.to_333());

  //Cannot test draw_with_rgb yet because they are private functions and we haven't implemented of the other class functions
}

// displays Level
void print_level(int level) {

  //Sets starting point for text
  matrix.setCursor(0,8);
  matrix.setTextColor(PURPLE.to_333());
  //Sets text to 8 LEDS tall, hence why we set the cursor to (0,8)
  matrix.setTextSize(1);

  String s = "Level " + level;
  matrix.print(s);

}

// displays number of lives
void print_lives(int lives) {
  //Sets starting point for text
  matrix.setCursor(0,8);
  matrix.setTextColor(PURPLE.to_333());
  //Sets text to 8 LEDS tall, hence why we set the cursor to (0,8)
  matrix.setTextSize(1);
  
  String s = "lives " + lives;
  matrix.print(s);
}

// displays "game over"
void game_over() {
  //Sets starting point for text
  matrix.setCursor(0,8);
  matrix.setTextColor(RED.to_333());
  //Sets text to 8 LEDS tall, hence why we set the cursor to (0,8)
  matrix.setTextSize(1);
  
  matrix.print("GAME OVER");
}



