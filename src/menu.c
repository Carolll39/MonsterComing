#ifndef MENU_C
#define MENU_C

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "mainGame.h"
#include "mainPlayer.h"
#include "menuPages.h"
#include "zombies.h"

// Layout Settings
int BUTTON_X;
int BUTTON_Y;
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_GAP (20 + BUTTON_HEIGHT)

int TOGGLE_X;
int TOGGLE_Y;
int currentCharacter;
#define TOGGLE_WIDTH 200
#define TOGGLE_HEIGHT 50
#define CHARACTERNUM 2
#define CHARACTERS_CHAR_LIST "WARRIOR;NINJA;WITCH"
Texture2D charactersPreview[CHARACTERNUM];

// Title Settings and Fonts
// By https://www.dafont.com/stranger-back-in-the-night.font personal use only
Font TITLEFONT;
#define TITLE_FONT_SIZE 50
#define TITLE_FONT_SPACING 5.0f
#define TITLE "Zombie Coming"
Vector2 titleDim;
Vector2 TITLE_POS;

bool gameStart;
bool gameExit;

enum PAGES {mainMenu, setting, credit};
enum PAGES currentPage;

// Sound Effects
Sound click;

// User Selections
enum characterSelections selectedCharacter;

// functions
void freeMenu();

// load style from file and initialize variables
void initMenu(){
    // Game Start Detection
    gameStart = false;
    gameExit = false;

    // Load Style For UI
    GuiLoadStyle("resources/jungle.rgs");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    // Load Title Font
    TITLEFONT = LoadFont("resources/titleFont.ttf");

    // Initialize Title Location
    titleDim = MeasureTextEx(GetFontDefault(), TITLE, TITLE_FONT_SIZE, TITLE_FONT_SPACING);
    TITLE_POS.x = GetScreenWidth()/2 - titleDim.x;
    TITLE_POS.y = GetScreenHeight()/4;

    // Initialize Button Location
    BUTTON_X = GetScreenWidth()/2 - BUTTON_WIDTH/2;
    BUTTON_Y = GetScreenHeight()/2 - BUTTON_HEIGHT/2;

    // Initialize Toggele Location
    TOGGLE_X = GetScreenWidth()/1.5;
    TOGGLE_Y = GetScreenHeight()/1.5;

    // Load Character Preview Images
    currentCharacter = 0;
    charactersPreview[0] = LoadTexture("resources/characters/64TempCh1.png");
    charactersPreview[1] = LoadTexture("resources/characters/64TempCh2.png");
    charactersPreview[1] = LoadTexture("resources/characters/64TempCh3.png");

    // Initialize Sound
    click = LoadSound("resources/soundEffect/select.wav");

    // Initialize Pages and Careers
    currentPage = mainMenu;
    selectedCharacter = WARRIOR;
    
    return;
}

// Draw Menu and User Input Detections
void drawMenu(){
    // Draw Background
    ClearBackground(BLACK);

    // Draw Title
    DrawTextEx(GetFontDefault(), "Zombie Coming", TITLE_POS, TITLE_FONT_SIZE, TITLE_FONT_SIZE, RED);
    
    // Draw Buttons and Detections Player Input
    bool startButtonPress = GuiButton((Rectangle){BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT}, "Start Game"); 
    bool settingButtonPress = GuiButton((Rectangle){BUTTON_X, BUTTON_Y+BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT}, "Settings");
    bool creditButtonPress = GuiButton((Rectangle){BUTTON_X, BUTTON_Y+2*BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT}, "Credit");
    bool quitButtonPress = GuiButton((Rectangle){BUTTON_X, BUTTON_Y+3*BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT}, "Quit");

    // Character Selector
    DrawTexture(charactersPreview[currentCharacter], TOGGLE_X + (TOGGLE_WIDTH-charactersPreview[currentCharacter].width)/2, TOGGLE_Y - charactersPreview[currentCharacter].height-BUTTON_GAP, WHITE);
    GuiComboBox((Rectangle){TOGGLE_X, TOGGLE_Y, TOGGLE_WIDTH, TOGGLE_HEIGHT}, CHARACTERS_CHAR_LIST, &currentCharacter);

    // Button Press Function Call and Current Page Drawing
    if(startButtonPress == true){
        // Load Main Game
        PlaySound(click);
        initMainGame();
        initMainPlayer(currentCharacter);
        gameStart = true;
        freeMenu();
    }
    else if(settingButtonPress == true || currentPage == setting){
        // Load Settings Menu
        PlaySound(click);
        currentPage = setting;
        drawSettingsPage();
        if(shouldQuitSettingsPage()) currentPage = mainMenu;
    }
    else if(creditButtonPress == true || currentPage == credit){
        // Load Credit Page
        PlaySound(click);
        currentPage = credit;
        drawCreditsPage();
        if(shouldQuitCreditsPage()) currentPage = mainMenu;
    }
    else if(quitButtonPress == true){
        // Exit
        gameExit = true;
    }

    return;
}

bool shouldStartGame(){
    return gameStart;
}

bool shouldExitGame(){
    return gameExit;
}

void freeMenu(){
    for(int i=0; i<sizeof(charactersPreview)/sizeof(charactersPreview[0]); i++){
        UnloadTexture(charactersPreview[i]);
    }
}

#endif