# Seven-Segment-Display
Seven-Segment Display code for STM32VLDISCOVERY board.

If you are looking for how to control a 7-segment display, you are in the right place. In this project, I controlled a four-digit seven-segment display using an STM32VLDISCOVERY board. Nevertheless, any microcontroller board with enough GPIO Pins could be used to get the job done and you can scale the project according to your needs. Even though the display I used is an eight-segment display capable of handling float numbers, for this project, I won't be utilizing the 'dot' segment, essentially making it function as a 7-segment display. I used the first digit to display the sign and the other three digits to represent hundreds, tens, and ones places. If you implement this method, you'll be able to effectively display numbers ranging from -999 to 999. Within this setup, I programmed the controller to effectively count within this specified range. Additionally, I incorporated three buttons: one for adjusting the counting speed, another for initiating or pausing the count, and the last one for changing the counting direction, whether it's ascending or descending.

A detailed description of the project is presented here:
Link: 

Fun Fact About this project:
Project files are named "Odev1" and it means "Homework1" in Turkish. This project was homework in a course that I took and I named the project as it is.


