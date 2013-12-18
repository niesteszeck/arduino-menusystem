/*
  .ino - 
 
 Created by Niesteszeck, Dec 8th 2013.
 Released into the public domain.
 
 License: GPL 3
 */

#include <MenuSystem.h>

// Menu variables
MenuSystem ms;
Menu mm("Main Menu");

Menu sm1("Setup Values");
SetupItem sm_mi0("value1", 5);
SetupItem sm_mi1("value2", 5);
SetupItem sm_mi2("value3", 5);
SetupItem sm_mi3("value4", 5);

MenuItem mm_mi1("Read Analog 0 A0");
MenuItem mm_mi2("Blink led");

Menu mu1("Date & Time");
MenuItem mu1_mi1("Show current");
MenuItem mu1_mi2("Set");
MenuItem mu1_mi3("Go back");

#define BUTTON_CANCEL    6
#define BUTTON_SELECT    5
#define BUTTON_LEFT      4
#define BUTTON_DOWN      3
#define BUTTON_UP        2
#define BUTTON_RIGHT     1
#define BUTTON_NONE      0

//Example related
#define SELECTED_DISPLAY_DELAY 1500

int led = 13; // connect a led+resistor on pin 41 or change this number to 13 to use default arduino led on board
int ledState = LOW;

void serialPrintHelp() {
  Serial.println("***************");
  Serial.println("w: up");
  Serial.println("s: down");
  Serial.println("a: right");
  Serial.println("d: left");
  Serial.println("q: Select");
  Serial.println("e: Cancel");
  Serial.println("?: print this help");
  Serial.println("h: print this help");
  Serial.println("***************");

}
byte serialHandler() {
  char inChar;
  if ((inChar = Serial.read()) > 0) {

    switch (inChar) {
    case 'w':
      return BUTTON_UP;
      break;
    case 's':
      return BUTTON_DOWN;
      break;
    case 'a':
      return BUTTON_LEFT;
    case 'd':
      return BUTTON_RIGHT;
      break;
    case 'e':
      return BUTTON_CANCEL;
      break;
    case 'q':
      return BUTTON_SELECT;
    case '?':
    case 'h': // Display help
      serialPrintHelp();
      break;
    default:
      break;
    }
  }
  return BUTTON_NONE;
}

void react(byte op) {
  /*
     BUTTON_CANCEL
   BUTTON_SELECT
   BUTTON_LEFT
   BUTTON_DOWN_UP
   BUTTON_RIGHT
   BUTTON_NONE
   */
  if (op == BUTTON_NONE)
    return;
  switch (op) {
  case BUTTON_CANCEL:
    break;
  case BUTTON_SELECT:
    ms.select(false);
    break;
  case BUTTON_LEFT:
    ms.prev();
    break;
  case BUTTON_DOWN:
    ms.decerment();
    break;
  case BUTTON_UP:
    ms.increment();
    break;
  case BUTTON_RIGHT:
    ms.next();
    break;
  }
  displayMenu();
}

// Menu callback functions
void on_item1_selected(MenuItem* p_menu_item) {
  Serial.print("Analog A0: ");
  Serial.println(analogRead(A0));
  delay(SELECTED_DISPLAY_DELAY);
}

void on_item2_selected(MenuItem* p_menu_item) {
  ledState = !ledState;
  digitalWrite(led, ledState);
  Serial.print("Led state: ");
  if (ledState)
    Serial.println("ON");
  else
    Serial.println("OFF");
  delay(SELECTED_DISPLAY_DELAY);
}
// on_back_selected is usefull if you don't have a button to make the back function
void on_back_selected(MenuItem* p_menu_item) {
  ms.back();
}
void on_time_show_selected(MenuItem* p_menu_item) {

}
void on_time_set_selected(MenuItem* p_menu_item) {


}

//The setup function is called once at startup of the sketch
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  serialPrintHelp();
  Serial.println("Setting up the menu.");

  // Menu setup
  /*
     Menu Structure:
   -Analog read
   -Led blink
   -Date & Time
   --Show current
   --Set
   --Back
   
   */
  // Set the callbacks
  mm_mi1.set_select_function(&on_item1_selected);
  mm_mi2.set_select_function(&on_item2_selected);
  mu1_mi1.set_select_function(&on_time_show_selected);
  mu1_mi2.set_select_function(&on_time_set_selected);
  mu1_mi3.set_select_function(&on_back_selected);

  mm.add(&sm1);
  sm1.add(&sm_mi0);
  sm1.add(&sm_mi1);
  sm1.add(&sm_mi2);
  sm1.add(&sm_mi3);

  mm.add(&mm_mi1);
  mm.add(&mm_mi2);

  mm.add(&mu1);
  mu1.add(&mu1_mi1);
  mu1.add(&mu1_mi2);
  mu1.add(&mu1_mi3);

  ms.set_root_menu(&mm);

  Serial.println("Menu setted.");

  displayMenu();

}

void displayMenu() {

  // Display the menu
  Menu const* cp_menu = ms.get_current_menu();
  Serial.println(cp_menu->get_selected()->get_name());
}

// The loop function is called in an endless loop
void loop() {
  int buttonPresed;
  // Handle commands
  buttonPresed = serialHandler();
  react(buttonPresed);
}

