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
const unsigned long BALL_DELAY = 50;
const unsigned long INVADER_DELAY = 1750;

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
      y++;
    }
    
    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      if (strength == 1){
        draw_with_rgb(RED, BLUE);
      }
      else if (strength == 2){
        draw_with_rgb(ORANGE, BLUE);
      }
      else if (strength == 3){
        draw_with_rgb(YELLOW, BLUE);
      }
      else if (strength == 4){
        draw_with_rgb(GREEN, BLUE);
      }
      else if (strength == 5){
        draw_with_rgb(BLUE, BLUE);
      }
      else if (strength == 6){
        draw_with_rgb(PURPLE, BLUE);
      }
      else if (strength == 7){
        draw_with_rgb(WHITE, BLUE);
      }
    }
    
    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK, BLACK);
    }    
    
    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      
      strength--;
      erase();
      if (strength != 0) {
        draw();
      }

    }

    bool covers(int px, int py) {
      if (strength <= 0) {
        return false;
      }
      int col = px - x;
      int row = py - y;
      if (col > 3 || col < 0 || row > 3 || row < 0) {
        return false;
      }
      if (row == 0) {
        if (col == 1 || col == 2) {
          return true;
        }
        return false;
      }
      if (row == 3) {
        if (col == 0 || col == 3) {
          return true;
        }
        return false;
      }
      return true;
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
      erase();
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
      x = x_arg;
      y = y_arg;
      fired = true;
    }
    
    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      erase();
      y--;
      if (y < 0) {
        fired = false;
      }
    }
    
    // resets private data members to initial values
    void hit() {
      erase();
      fired = false;
    }
    
    // draws the Cannonball, if it is fired
    void draw() {
      if (fired) {
        matrix.drawPixel(x, y, ORANGE.to_333());
        matrix.drawPixel(x, y + 1, ORANGE.to_333());
      }
    }
    
    // draws black where the Cannonball used to be
    void erase() {
      if (fired) {
        matrix.drawPixel(x, y, BLACK.to_333());
        matrix.drawPixel(x, y + 1, BLACK.to_333());
      }
    }

  private:
    int x;
    int y;
    bool fired;
};

class Player {
  public:
    Player() {
      x = 16;
      y = 14;
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
      draw_with_rgb(GREEN);
    }
    
    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK);
    }

    void set_lives(int lives_in){
      lives = lives_in;
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
      last_ball_move = 0;
      last_invader_move = 0;
    }
    
    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    void setupGame() {
      matrix.fillScreen(BLACK.to_333());
      time = millis();
      player.draw();
      level = 1;
      player.set_lives(3);
      set_level();
    }
    
    // advances the game simulation one step and renders the graphics
    // see spec for details of game
    // Modifies: global variable matrix
    void update(int potentiometer_value, bool button_pressed) {

      //Updates time
      time = millis();

      //Ends game if player has no more lives
      if(player.get_lives() > 0){
        //Draws player
        player.erase();
        int player_x = map(potentiometer_value, 0, 1023, 0, 30);
        player.set_x(player_x);
        player.draw();

        //Gets cannonball input
        if(button_pressed && !ball.has_been_fired()){
          ball.fire(player.get_x(), player.get_y()-2);
          check_ball_overlap();
        }

        if(player_hit_by_invader()){
          player.die();
          reset_level();

          if(player.get_lives() > 0){
            set_level();
          }

        }
      
        if (ball.has_been_fired() && time - last_ball_move >= BALL_DELAY) {
          last_ball_move = time;
          int target = invader_at(ball.get_x(), ball.get_y() - 1);
          if (target != -1) {
            ball.hit();
            enemies[target].hit();
          }
          else {
            ball.move();
          }
        }

        //Moves invaders
        if (time - last_invader_move >= INVADER_DELAY) {
          last_invader_move = time;
          int first = 8;
          if (!bottom_row_alive()) {
            first = 0;
          }
          for (int i = first; i < first + 8; i++) {
            if (enemies[i].get_strength() > 0) {
              enemies[i].erase();
              enemies[i].move();
              enemies[i].draw();
            }
          }
          check_ball_overlap();
        }

        ball.draw();

        //Advances to next level if level was cleared
        if(level_cleared()){
          level++;
          set_level();
        }
      }
      else{
        //Displays "game over" for 3 seconds, then resets the game
        matrix.fillScreen(BLACK.to_333());
        game_over();
        delay(3000);
        reset_enemy_positions();
        player.set_lives(3);
        level = 1;
        set_level();
      }
    }
    

  private:
    int level;
    unsigned long time;
    unsigned long last_ball_move;
    unsigned long last_invader_move;
    Player player;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];

    // check if Player defeated all Invaders in current level
    bool level_cleared() {
      for(int i = 0; i < NUM_ENEMIES; i++){
        if(enemies[i].get_strength() > 0){
          return false;
        }
      }

      return true;
    }

    // set up a level
    void reset_level() {
      ball.reset();
      last_ball_move = millis();
      last_invader_move = millis();
    }

    void reset_enemy_positions(){

      int x_spawn = 0;
      int y_spawn = 0;
      //Initializes the first 8 invaders
        for(int i = 0; i < NUM_ENEMIES / 2; i++){
          enemies[i].initialize(x_spawn, y_spawn, 1);
          x_spawn += 4;
        }

        //Increments the y_spawn coordinate to spawn next row
        y_spawn += 4;
        //resets x_spawn
        x_spawn = 0;

        //Initializes the second row of 8 invaders
        for(int i = 8; i < NUM_ENEMIES; i++){
          enemies[i].initialize(x_spawn, y_spawn, 0);
          x_spawn += 4;
        }
    }
    // Initializes each level
    void set_level() {

      //Displays level and lives before level starts
      matrix.fillScreen(BLACK.to_333());
      print_level(level);
      delay(1500);
      matrix.fillScreen(BLACK.to_333());
      print_lives(player.get_lives());
      delay(1500);
      matrix.fillScreen(BLACK.to_333());

      int x_spawn = 0;
      int y_spawn = 0;

      //Level 1 setup
      if(level == 1){
        //Initializes and draws 8 invaders at strength 1 for level 1
        for(int i = 0; i < NUM_ENEMIES / 2; i++){
          enemies[i].initialize(x_spawn, y_spawn, 1);
          enemies[i].draw();
          x_spawn += 4;
        }
      }

      //Level 2 setup
      if(level == 2){

        //Initializes and draws the first 8 invaders at strength 1 & 2 for level 2
        for(int i = 0; i < NUM_ENEMIES / 2; i++){
          //If the invader is at an even index, spawn it with 1 strength, else spawn it at 2
          if(i % 2 == 0){
            enemies[i].initialize(x_spawn, y_spawn, 1);
          }
          else{
            enemies[i].initialize(x_spawn, y_spawn, 2);
          }
          enemies[i].draw();
          x_spawn += 4;
        }

        //Increments the y_spawn coordinate to spawn next row
        y_spawn += 4;
        //resets x_spawn
        x_spawn = 0;

        //Initializes and draws the second row of 8 invaders at strength 1 & 2 for level 2
        for(int i = 8; i < NUM_ENEMIES; i++){
          //If the invader is at an even index, spawn it with 2 strength, else spawn it at 1
          if(i % 2 == 0){
            enemies[i].initialize(x_spawn, y_spawn, 2);
          }
          else{
            enemies[i].initialize(x_spawn, y_spawn, 1);
          }
          enemies[i].draw();
          x_spawn += 4;
        }
      }

      if(level == 3){

        int strengthIndex = 1;


        //Initializes and draws the first 8 invaders at strength index
        for(int i = 0; i < NUM_ENEMIES / 2; i++){
          //Resets the strength index if it gets too high
          if(strengthIndex > 5){
            strengthIndex = 1;
          }
          enemies[i].initialize(x_spawn, y_spawn, strengthIndex);
          enemies[i].draw();
          x_spawn += 4;
          strengthIndex++;
        }

        //Increments the y_spawn coordinate to spawn next row
        y_spawn += 4;
        //resets x_spawn
        x_spawn = 0;

        //Initializes and draws the second row of 8 invaders at strength index
        for(int i = 8; i < NUM_ENEMIES; i++){
          if(strengthIndex > 5){
            strengthIndex = 1;
          }
          enemies[i].initialize(x_spawn, y_spawn, strengthIndex);
          enemies[i].draw();
          x_spawn += 4;
          strengthIndex++;
        }
      }

      if(level == 4){
        //Initializes and draws the first 8 invaders at strength 4 & 5
        for(int i = 0; i < NUM_ENEMIES / 2; i++){
          //If the invader is at an even index, spawn it with 5 strength, else spawn it at 4
          if(i % 2 == 0){
            enemies[i].initialize(x_spawn, y_spawn, 5);
          }
          else{
            enemies[i].initialize(x_spawn, y_spawn, 4);
          }
          enemies[i].draw();
          x_spawn += 4;
        }

        //Increments the y_spawn coordinate to spawn next row
        y_spawn += 4;
        //resets x_spawn
        x_spawn = 0;

        //Initializes and draws the second row of 8 invaders at strength 2 & 3
        for(int i = 8; i < NUM_ENEMIES; i++){
          //If the invader is at an even index, spawn it with 2 strength, else spawn it at 3
          if(i % 2 == 0){
            enemies[i].initialize(x_spawn, y_spawn, 2);
          }
          else{
            enemies[i].initialize(x_spawn, y_spawn, 3);
          }
          enemies[i].draw();
          x_spawn += 4;
        }
      }

      if(level >= 5){
        //Initializes and draws the first 8 invaders with random strengths from 1-7
        for(int i = 0; i < NUM_ENEMIES / 2; i++){
          enemies[i].initialize(x_spawn, y_spawn, random(1,8));
          enemies[i].draw();
          x_spawn += 4;
        }

        //Increments the y_spawn coordinate to spawn next row
        y_spawn += 4;
        //resets x_spawn
        x_spawn = 0;

        //Initializes and draws the second row of 8 invaders with random strengths
        for(int i = 8; i < NUM_ENEMIES; i++){
          enemies[i].initialize(x_spawn, y_spawn, random(1,8));
          enemies[i].draw();
          x_spawn += 4;
        }
      }
    }

    bool bottom_row_alive() {
      for (int i = 8; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() > 0) {
          return true;
        }
      }
      return false;
    }

    int invader_at(int px, int py) {
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].covers(px, py)) {
          return i;
        }
      }
      return -1;
    }

    bool player_hit_by_invader(){
      int px = player.get_x();
      int py = player.get_y();

      for(int i = 0; i < NUM_ENEMIES; i++){
        if(enemies[i].get_strength() > 0 && 
        (enemies[i].covers(px, py) || 
         enemies[i].covers(px - 1, py + 1) ||
         enemies[i].covers(px, py + 1) || 
         enemies[i].covers(px + 1, py + 1))){
          Serial.println("PLAYER HIT");
          return true;
        }
      }
      return false;
    }

    void check_ball_overlap() {
      if (!ball.has_been_fired()) {
        return;
      }
      int target = invader_at(ball.get_x(), ball.get_y());
      if (target == -1) {
        target = invader_at(ball.get_x(), ball.get_y() + 1);
      }
      if (target != -1) {
        ball.hit();
        enemies[target].hit();
      }
    }
};

// a global variable that represents the game Space Invaders
Game game;

// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  game.setupGame();
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);

  game.update(potentiometer_value, button_pressed);
}

// displays Level
void print_level(int level) {

  //Sets starting point for text
  matrix.setCursor(0,0);
  matrix.setTextColor(PURPLE.to_333());
  //Sets text to 8 LEDS tall, hence why we set the cursor to (0,8)
  matrix.setTextSize(1);

  String s = "Level " + String(level);
  matrix.print(s);

}

// displays number of lives
void print_lives(int lives) {
  //Sets starting point for text
  matrix.setCursor(0,0);
  matrix.setTextColor(PURPLE.to_333());
  //Sets text to 8 LEDS tall, hence why we set the cursor to (0,8)
  matrix.setTextSize(1);
  
  String s = "lives " + String(lives);
  matrix.print(s);
}

// displays "game over"
void game_over() {
  //Sets starting point for text
  matrix.setCursor(0,0);
  matrix.setTextColor(RED.to_333());
  //Sets text to 8 LEDS tall, hence why we set the cursor to (0,8)
  matrix.setTextSize(1);
  
  matrix.print("GAME OVER");
}



