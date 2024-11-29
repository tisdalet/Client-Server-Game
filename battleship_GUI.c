// TO DO:
// Save raylib a or whatever linux file it is to a header file
// Test that the program can successfully spawn a window without needing a specific compiler
// Set up communication between the player and window so that it updates as the match goes
//      Text files?
// Each square should save a set of coordinates, depending on which square is selected
// The square should then change to a different square depending on if its a hit or a miss
// Each unchosen square should change color, showing it can be interacted with?

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Battleship client-server game");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // format is X axis, Y axis, specific size, then color
            DrawText("BATTLESHIP", 320, 100, 50, BLACK);
            DrawText("A     B     C     D     E", 270, 250, 40, BLACK);
            DrawText("1", 190, 330, 40, BLACK);
            DrawText("2", 185, 440, 40, BLACK);
            DrawText("3", 185, 550, 40, BLACK);
            DrawText("4", 185, 660, 40, BLACK);
            DrawText("5", 185, 770, 40, BLACK);
            
            int Ypos = 300;
            
            for(int i = 0; i < 5; i++)
            {
                DrawRectangle(screenWidth/2-50, Ypos, 100, 100, RED);
                DrawRectangle(screenWidth/2+60, Ypos, 100, 100, RED);
                DrawRectangle(screenWidth/2+170, Ypos, 100, 100, RED);
                DrawRectangle(screenWidth/2-160, Ypos, 100, 100, RED);
                DrawRectangle(screenWidth/2-270, Ypos, 100, 100, RED);
                Ypos = Ypos + 110;
            }
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
