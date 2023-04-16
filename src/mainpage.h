/** @mainpage AcePlays Documentation
*
* @authors AcePlay's Workforce
*
* @section intro Introduction
* This documentation covers the classes and their interactions for our blackjack decison maker. 
* Our code is as realtime as possible thanks to our outputs immediately notifying 
* the user of the optimum strategy that should be played. Our camera (detector) 
* can detect cards in each frame straight away before passing this information 
* to our StrategyEngine class via a callback. The decision from the StrategyEngine
* is then passed to the ToggleLED class to turn on a specific LED to notify
* the player of the optimum blackjack decision.
* 
* Check out our wiki which can be accessed via the README file on our Github landing
* page.
* 
* Here is the state diagram that describes our system:
* \image html SystemOverviewDiagram.svg
* 
* 
*/ 
