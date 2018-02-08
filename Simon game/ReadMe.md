
# My Simon Game Project

The Simon Game is an electronic game of memory skill. The device creates a random sequence of light and sound and then ask the user to 
repeat the same sequence.

I implemented my version of the Simon Game in the context of my Embedded System class. Here is my final Simon Game : 

![MySimon](images/simongame.png)

My project complies with the requirements with 4LEDs, 4 analog inputs (FSRs), an 16x2 LCD screen and a piezzo buzzer for the audio output.
The game speed is modifiable.

Here is the sketch of my Simon Game Project : 

![Fritzing](images/fritzing.png)

## How to play

Once the Arduino is powered, a Welcome message is displayed on the screen. Then, the user has to push the button to start a new game.

Once the game is started, the user has to repeat the led and sound pattern using the analog inputs. One just have to follow the instructions
on the LED screen "Memorize" or "Your turn".

If the user is right, one more led and sound combination is added to the pattern to memorize.
If the user fails, the game stops immediately. The Game Over message appears along with the player's score, which is the number of
sucessfully passed turns.
