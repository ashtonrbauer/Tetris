/*
 * ARDUINO TETRIS 
 * "Struct and Array Implementation"
 * Created by Ashton Reed Bauer
 * Other Group Members: Rex Petersen, Mina Taheb
 * 
 * First game implementation: Rex Petersen
 * Menu Creation, C Language Design/Final Game, & Hardware design: Ashton Reed Bauer
 * Video Editing/DJ/Early Game Logic: Mina Taheb
 * 
 * 
 * School: Georgia State University
 * Time created: January-April 2016
 * Completed: April 15th, 2016
 * For Dr. Michael Weeks' Embedded Systems CSc 4110 course.
 *  
 *
 */


#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library


/////////////////////////////////////////////////////////////////////////////////////
//////////////////DEFINES PINS FOR THE LED MATRIX////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////END////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
///////////////////THESE VARIABLES ARE FOR THE TETRIS LOGIC AND LIGHTING//////////////
//////////////////////////////////////////////////////////////////////////////////////
#define SIZE_Y  16
#define SIZE_X  32
struct Shape{
  int x1;
  int x2;
  int x3;
  int x4;
  int y1;
  int y2;
  int y3;
  int y4;
  int state; //THIS WILL BE FOR PIVOTS/TURNs
  char myShape; //Tells us shape
};

struct boardPiece{
  byte lit;
  byte color_R;
  byte color_G;
  byte color_B; 
};

byte currBoard[SIZE_Y][SIZE_X] = {0};             //Mirrors our LED board as the tetris game
int start_x;                                      //X element used to write Tetris menu
int start_y;                                      //Y element ""  
byte tet_x;                                       //X element for tetris pieces
byte tet_y = random(2,14);                        //Y element ""
byte orig_x;                                      //Backup for tet_x, redundant.
byte orig_y = random(2,14);                       //""         tet_y ""
byte clr_key_R;                                   //Int between 0-7 for matrix.Color333(U_INT8, U_INT8, U_INT8)
byte clr_key_G;                                   //""
byte clr_key_B;                                   //""
//byte choose_color;                                
int clear_pix = 0;                                //Clear color from certain pixel or line
int delay_time = 150;                             //Delay control for menu and other aspects of program
byte randomShape = random(5);                     //random number to decide which of the 5 pieces to write.


boolean needNewShape = true;                      //used to make sure a new piece is needed or not

Shape currShape = {0, 0, 0 ,0 ,0 ,0 ,0, 0, 0};    //Will be the Shape we write to board and store in currBoard
Shape oldShape = {0, 0, 0, 0, 0, 0, 0, 0, 0};     //Write currShape here to clear it from board.




//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////END////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
//////////////////INITIALIZES THE BOARD///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////END////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
//////////////////PINS FOR THE CONTROLLER/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
int joys_sw_pin =  12;
int joys_x_pin = 4;
int joys_y_pin = 5;
//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////END////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


int delay_amount = 30;          //Delay for menu



void setup() {
    matrix.begin();


   
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    /////////////TETRIS INTRO MENU BEGIN//////////////////////////
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    
    //////////LETTER T START//////////////////////
    start_y = 3;
    //TOP OF THE LETTER T
    for(start_x = 0; start_x < 6; start_x++)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 0));
         delay(delay_amount);
    }
    start_y = 4;
    //SECOND ROW OF TOP OF T, RIGHT HALF
    for(start_x = 5; start_x > 2; start_x--)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 0));
         delay(delay_amount);
    }
    //RIGHT HALF OF VERTICAL BASE OF T
    start_x = 3;
    for(start_y = 5; start_y < 11; start_y++)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 0));
         delay(delay_amount);
    }
    //LEFT HALF OF VERTICAL BASE OF T
    start_x = 2;
    for(start_y = 10; start_y > 4; start_y--)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 0));
         delay(delay_amount);
    }
    start_y = 4;
    //SECOND ROW OF TOP OF T, LEFT HALF
    for(start_x = 2; start_x > -1; start_x--)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 0));
         delay(delay_amount);
    }
    /////////////////LETTER T END////////////////////////////////


    
    
    
    
    //////////////////LETTER E START////////////////////////////
    
    //TOP ROW OF LETTER E (L TO R)
    start_y = 5;
    for(start_x = 5; start_x < 11; start_x++)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 2, 0));
         delay(delay_amount);
    }
    //SECOND ROW OF LETTER E, EXCEPT FOR ONE PIXEL OF VERTICAL BASE (R TO L)
    start_y = 6;
    for(start_x = 10; start_x > 5; start_x--){ 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 2, 0));
         delay(delay_amount);
    }
    //INNER PIXELS OF E AS WELL AS BOTTOM BASE
    start_x = 6;
    for(start_y = 7; start_y < 13; start_y++)
    { 
         //TOP OF INNER ARM OF E (L TO R)
         if(start_y == 8)
         {
            while(start_x < 9)
            {
                matrix.drawPixel(start_x++,start_y, matrix.Color333(7, 2, 0));
                delay(delay_amount);
            }
            start_x--; 
         }
         //BOTTOM INNER ARM OF E (R TO L)
         else if(start_y == 9)
         {
            while(start_x > 5)
            {
                matrix.drawPixel(start_x--,start_y, matrix.Color333(7, 2, 0));
                delay(delay_amount);
            }
            start_x++;
         }
         //TOP OF THE BOTTOM BASE OF E (L TO R)
         else if(start_y == 11)
         {
            while(start_x < 11)
            {
                matrix.drawPixel(start_x++,start_y, matrix.Color333(7, 2, 0));
                delay(delay_amount);
            }
            start_x--;
         }
         //BOTTOM OF THE BOTTOM BASE OF E (R TO L)
         else if(start_y == 12)
         {
            while(start_x > 4)
            {
                matrix.drawPixel(start_x--,start_y, matrix.Color333(7, 2, 0));
                delay(delay_amount);
            }
            start_x++;
         }
         //INNER TWO PIXELS ABOVE AND BELOW INNER ARM OF E
         else
         {
            matrix.drawPixel(start_x,start_y, matrix.Color333(7, 2, 0));
            delay(delay_amount);
         }   
    }
    //LEFT SIDE OF VERTICAL BASE OF E (BOTTOM TO TOP)
    start_x = 5;
    for(start_y = 11; start_y > 4; start_y--)
    {
        matrix.drawPixel(start_x,start_y, matrix.Color333(7, 2, 0));
        delay(delay_amount);
    }
    /////////////////LETTER E END///////////////////////////////


    
    
    
    
    /////////////////LETTER T START///////////////////////////////
    start_y = 3;
    //TOP OF THE LETTER T
    for(start_x = 10; start_x < 16; start_x++)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 7, 0));
         delay(delay_amount);
    }
    start_y = 4;
    //SECOND ROW OF TOP OF T, RIGHT HALF
    for(start_x = 15; start_x > 12; start_x--)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 7, 0));
         delay(delay_amount);
    }
    //RIGHT HALF OF VERTICAL BASE OF T
    start_x = 13;
    for(start_y = 5; start_y < 11; start_y++)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 7, 0));
         delay(delay_amount);
    }
    //LEFT HALF OF VERTICAL BASE OF T
    start_x = 12;
    for(start_y = 10; start_y > 4; start_y--)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 7, 0));
         delay(delay_amount);
    }
    //SECOND ROW OF TOP OF T, LEFT HALF
    start_y = 4;
    for(start_x = 12; start_x > 9; start_x--)
    { 
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 7, 0));
         delay(delay_amount);
    }
    /////////////////LETTER T END///////////////////////////////





    /////////////////LETTER R START///////////////////////////////
    //VERTICAL BASE LEFT SIDE AND HORIZONTAL TOP OF R
    start_x = 15;
    for(start_y = 12; start_y > 4; start_y--)
    {
         matrix.drawPixel(start_x,start_y, matrix.Color333(0, 7, 0));
         delay(delay_amount);
         //TOP OF R
         while(start_y == 5 && start_x < 20)
         {
              matrix.drawPixel(start_x++,start_y, matrix.Color333(0, 7, 0));
              delay(delay_amount);     
         }
    }
    //OUTER THREE PIXELS OF R'S CIRCULAR TOP
    matrix.drawPixel(20,6, matrix.Color333(0, 7, 0));
    delay(delay_amount);
    matrix.drawPixel(20,7, matrix.Color333(0, 7, 0));
    delay(delay_amount);
    matrix.drawPixel(19,8, matrix.Color333(0, 7, 0));
    delay(delay_amount);
    //DIAGONAL LEG OF R
    //RIGHT SIDE
    start_x = 18;
    start_y = 9;
    while(start_x < 23 && start_y < 14)
    {
         matrix.drawPixel(start_x++,start_y++, matrix.Color333(0, 7, 0));
         delay(delay_amount);      
    }
    //LEFT SIDE
    start_x-=2;
    start_y--;
    while(start_x > 16 && start_y > 8)
    {
         matrix.drawPixel(start_x--,start_y--, matrix.Color333(0, 7, 0));
         delay(delay_amount);
    }
    //FOUR INNER PIXELS OF R'S CIRCULAR TOP
    matrix.drawPixel(17,8, matrix.Color333(0, 7, 0));
    delay(delay_amount);
    matrix.drawPixel(18,8, matrix.Color333(0, 7, 0));
    delay(delay_amount);
    matrix.drawPixel(19,7, matrix.Color333(0, 7, 0));
    delay(delay_amount);
    //THE BOTTOM OR R'S TOP HORIZONTAL BAR AND INNER VERTICAL BASE
    start_x = 19;
    for(start_y = 6; start_y < 13; start_y++)
    {
         while(start_y == 6 && start_x > 15)
         {
              matrix.drawPixel(start_x--,start_y, matrix.Color333(0, 7, 0));
              delay(delay_amount);     
         }
         start_x = 16;
         matrix.drawPixel(start_x,start_y, matrix.Color333(0, 7, 0));
         delay(delay_amount);
    }
    /////////////////LETTER R END///////////////////////////////



    
    
    
    
    /////////////////LETTER I START///////////////////////////////
    //TOP OF LETTER I
    start_y = 3;
    for(start_x = 20; start_x < 26; start_x++)
    {
         matrix.drawPixel(start_x,start_y, matrix.Color333(0, 0, 7));
         delay(delay_amount);
    }
    //RIGHT SIDE, BOTTOM OF HORIZONTAL TOP OF I
    start_y = 4;
    for(start_x = 25; start_x > 22; start_x--)
    {
         matrix.drawPixel(start_x,start_y, matrix.Color333(0, 0, 7));
         delay(delay_amount);
    }
    start_x = 23;
    for(start_y = 5; start_y < 11; start_y++)
    {
         if(start_x > 25)
               start_x--;
         //VERTICAL BASE OF I, RIGHT SIDE
         matrix.drawPixel(start_x,start_y, matrix.Color333(0, 0, 7));
         delay(delay_amount);

         //TOP OF HORIZONTAL BOTTOM/BASE, RIGHT SIDE
         while(start_y == 9 && start_x < 26)
         {
               matrix.drawPixel(start_x++,start_y, matrix.Color333(0, 0, 7));
               delay(delay_amount); 
         }
         //BOTTOM OF HORIZONTAL BASE
         while(start_y == 10 && start_x > 19)
         {
               matrix.drawPixel(start_x--,start_y, matrix.Color333(0, 0, 7));
               delay(delay_amount); 
         }
         
    }
    //TOP OF HORIZONTAL BOTTOM/BASE, LEFT SIDE
    start_y = 9;
    for(start_x = 20; start_x < 23; start_x++)
    {
         matrix.drawPixel(start_x,start_y, matrix.Color333(0, 0, 7));
         delay(delay_amount);
    }
    //VERTICAL BASE OF I, LEFT SIDE AND BOTTOM HALF OF HORIZONTAL TOP, LEFT SIDE
    start_x = 22;
    for(start_y = 8; start_y > 3; start_y--)
    {
         matrix.drawPixel(start_x,start_y, matrix.Color333(0, 0, 7));
         delay(delay_amount);
         while(start_y == 4 && start_x > 19)
         {
               matrix.drawPixel(start_x--,start_y, matrix.Color333(0, 0, 7));
               delay(delay_amount);
         }
    }
    /////////////////LETTER I END///////////////////////////////






    /////////////////LETTER S START///////////////////////////////
    //TOP OF LETTER S
    start_y = 5;
    for(start_x = 27; start_x < 31; start_x++)
    {
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 7));
         delay(delay_amount);
    }
    //BOTTOM OF TOP LAYER OF S
    start_y = 6;
    for(start_x = 31; start_x > 26; start_x--)
    {
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 7));
         delay(delay_amount);      
    }
    //INNER PIXEL, TOP HALF OF S, TOP OF MIDDLE LAYER OF S, & LOWER OUTER CURVE
    start_y = 7;
    for(start_x = 26; start_x< 32; start_x++)
    {
         if(start_x > 26 && start_x < 30)
               start_y = 8;
         if(start_x == 30)
               start_y = 9;
         if(start_x == 31)
               start_y = 10;
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 7));
         delay(delay_amount);    
    }
    //BOTTOM BASE OF S
    start_y = 11;
    for(start_x = 30; start_x > 24; start_x--)
    {
         if(start_x > 25 && start_x < 30)
               start_y = 12;
         if(start_x == 25)
               start_y = 11;
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 7));
         delay(delay_amount);    
    }
    //TOP OF BOTTOM BASE OF S & INNER PIXEL OF CURVE OF BOTTOM HALF
    for(start_x = 26; start_x < 31; start_x++)
    {
         if(start_x == 30)
             start_y--;
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 7));
         delay(delay_amount); 
    }
    //BOTTOM OF MIDDLE LAYER OF S & OUTER CURVE OF TOP HALF
    start_y = 9;
    for(start_x = 29; start_x > 24; start_x--)
    {
         if(start_x == 25)
               start_y = 7;
         if(start_x == 26)
               start_y = 8;
         matrix.drawPixel(start_x,start_y, matrix.Color333(7, 0, 7));
         delay(delay_amount);    
    }
    //LAST PIXEL
    matrix.drawPixel(26, 6, matrix.Color333(7,0,7));
    /////////////////LETTER S END/////////////////////////////////


    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    /////////////TETRIS INTRO MENU ENDING/////////////////////////
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    
    delay(2000);    //Hold the tetris menu for 2 seconds
    
    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    /////////////////////JOYSTICK INITIALIZATION/////////////////
    /////////////////////////////////////////////////////////////
    pinMode(joys_sw_pin, INPUT);
    digitalWrite(joys_sw_pin, HIGH);
    //orig_x, orig_y, tet_x, tet_y = 0;
    tet_y, orig_y = random(2,14);
    tet_x, orig_x = 0;
    //tet_y = 5;
    //delay(5000);
    matrix.fillScreen(0);
    
    
}

void loop() 
{
  
  /////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////INITIALIZE A NEW RANDOM SHAPE//////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  
  if(needNewShape){
    randomShape = random(5);
    clr_key_R = random(8);
    clr_key_G = random(8);
    clr_key_B = random(8);
    switch(randomShape){
     //Uses array/matrices mathematics to generate shapes. Where each block is an (x,y) pair in relation to a pivot, tet_x/tet_y.
     case 0://tee shape
     {
        currShape = {tet_x+0, tet_x+0, tet_x+1, tet_x+0, tet_y+1, tet_y+0, tet_y+1,tet_y+2, 0, 'T'};
        break;
     } 
     case 1://L
     {
        currShape = {tet_x+0, tet_x+0, tet_x+0, tet_x+1, tet_y+2, tet_y+1, tet_y+0,tet_y+2, 0, 'L'};
        break;
     }
     case 2://square
     {
        currShape = {tet_x+0, tet_x+0, tet_x+1, tet_x+1, tet_y+0, tet_y+1, tet_y+0,tet_y+1, 0, 'S'};
        break;      
     }
     case 3://zig-zag
     {
        currShape = {tet_x+0, tet_x+0, tet_x+1, tet_x+1, tet_y+0, tet_y+1, tet_y+0,tet_y-1, 0, 'Z'};
        break;      
     }
     case 4://straight line
     {
        currShape = {tet_x+0, tet_x+0, tet_x+0, tet_x+0, tet_y+0, tet_y+1, tet_y+2,tet_y+3, 0, 'I'};
        break;      
     }
     
    }
     writeCurr();
     needNewShape = false;
  }
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////END SHAPE INIT.////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////


 //"Interrupt" and make sure no collisions 
  checkForCollision();




 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////CONTROLLER SECTION/////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if(digitalRead(joys_sw_pin) == 0)       //PUSH BUTTON
  {
    tet_x = currShape.x1;                 //X = curr X value at pivot location
    tet_y = currShape.y1;                 //Y = curr Y ""
    clr_key_R = random(8);                //New random color when rotated
    clr_key_G = random(8);
    clr_key_B = random(8);
    setOldShape();                        //set all oldShape X and Y values from all values of currShape
    rotate();                             //change state
    removeOld();                          //clear board of oldShape
    writeCurr();                          //write new shape
    setOldShape();                        //set oldShape = to currShape
  }

  if(analogRead(joys_y_pin) > 0)          //Left and Right. Board is technically sideways. So, X is up and down and Y is left and right.
  {
    if(analogRead(joys_y_pin) < 100)      //RIGHT
    {
      if(currShape.y1 > 0 && currShape.y2 > 0 && currShape.y2 > 0 && currShape.y3 > 0 && currShape.y4 > 0){
        setOldShape();
        removeOld(); 
        dec_Curr_Y();                     //Decrement all Y values of currShape
        checkForCollision();
        setOldShape(); //TEST ME REED
        removeOld();
        writeCurr();
         
              
      }
    }
    else if(analogRead(joys_y_pin) > 1000)    //LEFT
    {
      if(currShape.y1 < 15 && currShape.y2 < 15 && currShape.y2 < 15 && currShape.y3 < 15 && currShape.y4 < 15)
      {
        setOldShape();
        removeOld(); 
        inc_Curr_Y();
        checkForCollision();
        setOldShape(); 
        removeOld();
        writeCurr();
            
      }
    }
    
  }
  if(analogRead(joys_x_pin) > 1000)       //DOWN
    {
      if(currShape.x1 < 31 && currShape.x2 < 31 && currShape.x2 < 31 && currShape.x3 < 31 && currShape.x4 < 31)
      {
        setOldShape();
        removeOld();
        inc_Curr_X();
        checkForCollision();
        setOldShape(); 
        removeOld();
        writeCurr();
      }
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////END CONTROLLER SECTION////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  setOldShape();
  checkForCollision();
  

  if(currShape.x1 < 31 && currShape.x2 < 31 && currShape.x2 < 31 && currShape.x3 < 31 && currShape.x4 < 31)     //Increments the board every pass through.
  {
        setOldShape();
        removeOld();
        inc_Curr_X();
        checkForCollision();
        setOldShape(); 
        removeOld();
        writeCurr();
        
  }


//DELETE THE PREVIOUS SHAPE DRAWN FROM THE BOARD. THIS PROVIDES ILLUSION OF MOVEMENT
  removeOld();


//DRAW INCREMENTED SHAPE ONTO BOARD.
  writeCurr();
  
  if(currShape.x1 == 31 || currShape.x2 == 31 || currShape.x3 == 31 || currShape.x4 == 31)        //If we reach the bottom of the board, set it into the game array.
  {
    setShapeOnBoard();
  }
  

  //Check our board and see if we need to delete a row!
  checkForRemoval();

  delay(delay_time);
  
}









///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////FUNCTION SECTION: ALL REQUIRED FUNCTIONS////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Using the same logic to generate a shape, this evaluates the "state" of the shape and will increment its state to the next state. It simply
 * uses logic that all pieces surround a pivot and just changes to orintation of pieces to the pivot.
 */
void rotate()
{
  currShape.state++;
  
  if(currShape.myShape == 'T')
  {
    //'T' shape has 4 states: Straight down, left oriented, up, and right oriented.
    if(currShape.state > 3)
    {
    currShape.state = 0;
    }
    if(currShape.state == 0)       
    {       
              currShape = {tet_x+0, tet_x+0, tet_x+1, tet_x+0, tet_y+1, tet_y+0, tet_y+1,tet_y+2, 0, 'T'};
    }
    else if(currShape.state == 1)
    {
              currShape = {tet_x+0, tet_x+0, tet_x-1, tet_x+1, tet_y+0, tet_y+1, tet_y+0,tet_y+0, 1, 'T'};
    }
    else if(currShape.state == 2)
    {
      
             currShape = {tet_x+0, tet_x-1, tet_x+0, tet_x+0, tet_y+0, tet_y+0, tet_y+1,tet_y-1, 2, 'T'};
             
    }
    else
    {
              currShape = {tet_x+0, tet_x+0, tet_x+1, tet_x-1, tet_y+0, tet_y-1, tet_y+0,tet_y+0, 3, 'T'};
              
    }
  }
  else if(currShape.myShape == 'L')
  {
    //'L' has 4 states. Starting left oriented horizontal, then vertically left oriented, th horizontally right oriented, and finally vertically right orieted.
    if(currShape.state > 3)
    {
    currShape.state = 0;
    }
    if(currShape.state == 0)
    {
              currShape = {tet_x+0, tet_x+0, tet_x+0, tet_x+1, tet_y+2, tet_y+1, tet_y+0,tet_y+2, 0, 'L'};
              
    }
    else if(currShape.state == 1)
    {
              currShape = {tet_x+0, tet_x+1, tet_x+2, tet_x+0, tet_y+0, tet_y+0, tet_y+0,tet_y+1, 1, 'L'};
              
    }
    else if(currShape.state == 2)
    {
      
             currShape = {tet_x+0, tet_x+0, tet_x+0, tet_x-1, tet_y+0, tet_y+1, tet_y+2,tet_y+0, 2, 'L'};
            
    }
    else
    {
              currShape = {tet_x+0, tet_x-1, tet_x-2, tet_x+0, tet_y+0, tet_y+0, tet_y+0,tet_y-1, 3, 'L'};
              
    }
  }
  else if(currShape.myShape == 'I')
  {
    //the straight line only ahs two states. Vertical and Horizontal.
    if(currShape.state > 1)
    {
    currShape.state = 0;
    }
    if(currShape.state == 0)
    {
              currShape = {tet_x+0, tet_x+0, tet_x+0, tet_x+0, tet_y+0, tet_y+1, tet_y+2,tet_y+3, 0, 'I'};
              
    }
    else if(currShape.state == 1)
    {
              currShape = {tet_x+0, tet_x+1, tet_x+2, tet_x+3, tet_y+0, tet_y+0, tet_y+0,tet_y+0, 1, 'I'};
              
    }
  } 
  else if(currShape.myShape == 'Z')       
  {
    //zig-zag only has two states. horizontal and vertical
    if(currShape.state > 1)
    {
    currShape.state = 0;
    }
    if(currShape.state == 0)
    {
              currShape = {tet_x+0, tet_x+0, tet_x+1, tet_x+1, tet_y+0, tet_y+1, tet_y+0,tet_y-1, 0, 'Z'};
              
    }
    else if(currShape.state == 1)
    {
              currShape = {tet_x+0, tet_x+0, tet_x-1, tet_x+1, tet_y+0, tet_y-1, tet_y-1,tet_y+0, 1, 'Z'};
             
    }
  }   
}


/*
 * This will remove the indicated index of row from the board and then increment down all values of the array.
 * Straight forward for loop logic to iterate through multidimensional array.
 */
void removeRow(int index)
{
    matrix.drawLine(index, 0, index, 15, matrix.Color333(0,0,0));
    for(int col = 0; col < SIZE_Y; col++)
    {
      currBoard[col][index] = 0;
    }    
    
    for(int r = SIZE_X - 1 ; r > 0; r--)//row
    {
        for(int c = 0; c < SIZE_Y; c++)//columns
        {
          if(currBoard[c][r] == 1)
          {
            continue;
          }
          else
          {
            currBoard[c][r] = currBoard[c][r-1];
            currBoard[c][r-1] = 0;
            if(currBoard[c][r] == 0 && r >= 2)
            {
              currBoard[c][r] = currBoard[c][r-2];
              currBoard[c][r-2] = 0; 
            }
          }
        }
    }
     
    testBoardArray();           //originally a test function, but actually creates the random lighting of the board every time a peice is set or a row's removed.
}



/*
 * Takes the current shape and solidifies its place in the currBoard array and draws the pixel in place 
 */
void setShapeOnBoard()
{
    matrix.drawPixel(currShape.x1,currShape.y1, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));    //Draw onto board
    matrix.drawPixel(currShape.x2,currShape.y2, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
    matrix.drawPixel(currShape.x3,currShape.y3, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
    matrix.drawPixel(currShape.x4,currShape.y4, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
   // matrix.fillScreen(0);
    
    
    currBoard[currShape.y1][currShape.x1] = 1;          //Set to array
    currBoard[currShape.y2][currShape.x2] = 1;
    currBoard[currShape.y3][currShape.x3] = 1;
    currBoard[currShape.y4][currShape.x4] = 1;


    testBoardArray();       //generates random lighting of the entire array.

    //delay(1000);
/*
    matrix.drawPixel(currShape.x1,currShape.y1, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
    matrix.drawPixel(currShape.x2,currShape.y2, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
    matrix.drawPixel(currShape.x3,currShape.y3, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
    matrix.drawPixel(currShape.x4,currShape.y4, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
*/
    

    tet_x = 0;
    tet_y = random(2,14);
    clr_key_R = random(8);
    clr_key_G = random(8);
    clr_key_B = random(8);
    needNewShape = true;
}
/*
 * This will make sure that there are no occupied spaces below the current piece we are using
 */
void checkForCollision(){
  if(currBoard[currShape.y1][currShape.x1 + 1] == 1 ||
     currBoard[currShape.y2][currShape.x2 + 1] == 1 ||
     currBoard[currShape.y3][currShape.x3 + 1] == 1 ||
     currBoard[currShape.y4][currShape.x4 + 1] == 1 )
  {
            setShapeOnBoard();
  }
}

//Write our currShape to the board. Gives the illusion of movement.
void writeCurr()
{
    matrix.drawPixel(currShape.x1,currShape.y1, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
    matrix.drawPixel(currShape.x2,currShape.y2, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
    matrix.drawPixel(currShape.x3,currShape.y3, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
    matrix.drawPixel(currShape.x4,currShape.y4, matrix.Color333(clr_key_R, clr_key_G, clr_key_B));
}
//Pairs with currShape to represent movement. oldShape is removed and currShape is written, and it looks like it moves bc it happens so quickly.
void removeOld()
{
  //DELETE THE PREVIOUS SHAPE DRAWN FROM THE BOARD. THIS PROVIDES ILLUSION OF MOVEMENT
  matrix.drawPixel(oldShape.x1,oldShape.y1, matrix.Color333(clear_pix, clear_pix, clear_pix));
  matrix.drawPixel(oldShape.x2,oldShape.y2, matrix.Color333(clear_pix, clear_pix, clear_pix));
  matrix.drawPixel(oldShape.x3,oldShape.y3, matrix.Color333(clear_pix, clear_pix, clear_pix));
  matrix.drawPixel(oldShape.x4,oldShape.y4, matrix.Color333(clear_pix, clear_pix, clear_pix));
}

//Set all x and y values of oldShape to the values of currShape's x and y values.
void setOldShape()
{
  oldShape.x1 = currShape.x1;
  oldShape.x2 = currShape.x2;
  oldShape.x3 = currShape.x3;
  oldShape.x4 = currShape.x4;
  oldShape.y1 = currShape.y1;
  oldShape.y2 = currShape.y2;
  oldShape.y3 = currShape.y3;
  oldShape.y4 = currShape.y4;
}
//Increment all x values of currShape by 1
void inc_Curr_X()
{
        currShape.x1++;
        currShape.x2++;
        currShape.x3++;
        currShape.x4++;
}
//Decrement all y values of currShape by 1
void dec_Curr_Y()
{
        currShape.y1--;
        currShape.y2--;
        currShape.y3--;
        currShape.y4--;
}
//increment all y values of currShape by 1
void inc_Curr_Y()
{
        currShape.y1++;
        currShape.y2++;
        currShape.y3++;
        currShape.y4++;
}
//This will iterate through the board and check if any row is occupied by all 1s, if so remove it.
void checkForRemoval()
{
  //Check our board and see if we need to delete a row!
  for(int i = 0; i < SIZE_X; i++)//row
    {
        for(int j = 0; j < SIZE_Y; j++)//column
        {
            if(currBoard[j][i] == 1)
            {
                if(j == 15)
                {
                    removeRow(i);
                }
                continue;
            }
            else
            {
                break;
            }
        }
    }
}
//Generates the random lighting of the board anytime a piece is set.
void testBoardArray(){

    for(int i = 0; i < SIZE_X; i++)//row
    {
        for(int j = 0; j < SIZE_Y; j++)//column
        {
           if(currBoard[j][i] == 1)
           {
              matrix.drawPixel(i, j, matrix.Color333(random(8), random(8), random(8)));
           }
           else
           {
              matrix.drawPixel(i, j, matrix.Color333(0,0,0));
           }
        }
  
    }
}















//Will cause memory overflow, but worth trying to implement to store colors. Maybe a future update.
/* THIS IS ONLY IF I HAVE TIME TO WRITE AND STORE THE COLORS OF EACH PIECE TO MAKE IT LOOK BETTER
    currBoard[currShape.y1][currShape.x1].lit = 1;
    currBoard[currShape.y2][currShape.x2].lit = 1;
    currBoard[currShape.y3][currShape.x3].lit = 1;
    currBoard[currShape.y4][currShape.x4].lit = 1;
    
    currBoard[currShape.y1][currShape.x1].color_R = clr_key_R;
    currBoard[currShape.y1][currShape.x1].color_G = clr_key_G;
    currBoard[currShape.y1][currShape.x1].color_B = clr_key_B;

    currBoard[currShape.y2][currShape.x2].color_R = clr_key_R;
    currBoard[currShape.y2][currShape.x2].color_G = clr_key_G;
    currBoard[currShape.y2][currShape.x2].color_B = clr_key_B;

    currBoard[currShape.y3][currShape.x3].color_R = clr_key_R;
    currBoard[currShape.y3][currShape.x3].color_G = clr_key_G;
    currBoard[currShape.y3][currShape.x3].color_B = clr_key_B;

    currBoard[currShape.y4][currShape.x4].color_R = clr_key_R;
    currBoard[currShape.y4][currShape.x4].color_G = clr_key_G;
    currBoard[currShape.y4][currShape.x4].color_B = clr_key_B;
    */




























  
*/  

