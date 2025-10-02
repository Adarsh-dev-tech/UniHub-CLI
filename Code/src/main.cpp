/*
    main.cpp

    This is the main entry point for the UniHub-CLI application. It initializes
    the program and starts the application's menu-driven interface by calling
    uni::runApp(). The main function is responsible for launching the core
    workflow and returning the program's exit status.
*/

#include "menu.h" // Includes the menu interface for running the application

int main() {
    uni::runApp(); // Starts the UniHub-CLI application and its menu system
    return 0;      // Returns 0 to indicate successful program termination
}
