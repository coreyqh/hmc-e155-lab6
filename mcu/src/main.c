/*
File: Lab_6_JHB.c
Author: Josh Brake
Email: jbrake@hmc.edu
Date: 9/14/19

modified 10/29/2025
corey hickson chickson@hmc.edu
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

/////////////////////////////////////////////////////////////////
// Provided Constants and Functions
/////////////////////////////////////////////////////////////////

//Defining the web page in two chunks: everything before the current time, and everything after the current time
char* webpageStart = "<!DOCTYPE html><html><head><title>E155 Lab 6 Webpage</title>\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	</head>\
	<body><h1>E155 Lab 6 Webpage</h1>";

char* ledStr = "<p>LED Control:</p><form action=\"ledon\"><input type=\"submit\" value=\"Turn the LED on!\"></form>\
	<form action=\"ledoff\"><input type=\"submit\" value=\"Turn the LED off!\"></form>";

char* tempStr = "<p>Temperature Control:</p><form action=\"resolution8\"><input type=\"submit\" value=\"Set 8 bit resolution!\"></form>\
	                                          <form action=\"resolution9\"><input type=\"submit\" value=\"Set 9 bit resolution!\"></form>\
	                                          <form action=\"resolution10\"><input type=\"submit\" value=\"Set 10 bit resolution!\"></form>\
	                                          <form action=\"resolution11\"><input type=\"submit\" value=\"Set 11 bit resolution!\"></form>\
	                                          <form action=\"resolution12\"><input type=\"submit\" value=\"Set 12 bit resolution!\"></form>";

char* webpageEnd   = "</body></html>";

//determines whether a given character sequence is in a char array request, returning 1 if present, -1 if not present
int inString(char request[], char des[]) {
	if (strstr(request, des) != NULL) {return 1;}
	return -1;
}

int updateLEDStatus(char request[]) {

	int led_status = 0;
	// The request has been received. now process to determine whether to turn the LED on or off
	if (inString(request, "ledoff")==1) {
		digitalWrite(LED_PIN, GPIO_LOW);
		led_status = 0;
	}
	else if (inString(request, "ledon")==1) {
		digitalWrite(LED_PIN, GPIO_HIGH);
		led_status = 1;
	}

	return led_status;
}

int updateTempStatus(char request[]) {

  int temp_status = 0;

  if      (inString(request, "resolution8")==1) {
		spiWrite(CONFIG_WADDR, RESOLUTION_8)
	}  
  else if (inString(request, "resolution9")==1) {
		spiWrite(CONFIG_WADDR, RESOLUTION_9)
	}  
  else if (inString(request, "resolution10")==1) {
		spiWrite(CONFIG_WADDR, RESOLUTION_10)
	}  
  else if (inString(request, "resolution11")==1) {
		spiWrite(CONFIG_WADDR, RESOLUTION_11)
	}  
  else if (inString(request, "resolution12")==1) {
		spiWrite(CONFIG_WADDR, RESOLUTION_12)
	}  

  temp_status = spiRead(TMPMSB_RADDR);
  temp_status = temp_status << 8;
  temp_status = temp_status | spiRead(TMPLSB_RADDR);

  return temp_status;

}

/////////////////////////////////////////////////////////////////
// Solution Functions
/////////////////////////////////////////////////////////////////

int main(void) {
  configureFlash();
  configureClock();

  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  pinMode(LED_PIN, GPIO_OUTPUT);
  
  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);
  
  USART_TypeDef * USART = initUSART(USART1_ID, 125000);
  
  // TODO: Add SPI initialization code
  spiInit();

  while(1) {
    /* Wait for ESP8266 to send a request.
    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
    Therefore the request[] array must be able to contain 18 characters.
    */

    // Receive web request from the ESP
    char request[BUFF_LEN] = "                  "; // initialize to known value
    int charIndex = 0;
  
    // Keep going until you get end of line character
    while(inString(request, "\n") == -1) {
      // Wait for a complete request to be transmitted before processing
      while(!(USART->ISR & USART_ISR_RXNE));
      request[charIndex++] = readChar(USART);
    }

    // TODO: Add SPI code here for reading temperature
    int temp_status = updateTempStatus(request);
    int sign = (temp_status >> 15) & 1;

    char tempStatusStr[50];

    temp_status &= ~(1 << 15);               // mask off sign bit to find magnitude
    float temperature = temp_status / 16.0f; // convert fixed pt to floating pt
    temperature = sign ? -temperature : temperature; 
    sprintf(tempStatusStr, "Temperature: %.4f deg. C", temperature);
    

    // Update string with current LED state
  
    int led_status = updateLEDStatus(request);

    char ledStatusStr[20];
    if (led_status == 1)
      sprintf(ledStatusStr,"LED is on!");
    else if (led_status == 0)
      sprintf(ledStatusStr,"LED is off!");

    

    // finally, transmit the webpage over UART
    sendString(USART, webpageStart); // webpage header code
    sendString(USART, ledStr); // button for controlling LED
    sendString(USART, tempStr); // button for controlling temp sensor
    

    sendString(USART, "<h2>LED Status</h2>");

    sendString(USART, "<p>");
    sendString(USART, ledStatusStr);
    sendString(USART, "</p>");


    sendString(USART, "<h2>Temperature Status</h2>");

    sendString(USART, "<p>");
    sendString(USART, ledStatusStr);
    sendString(USART, "</p>");  

  
    sendString(USART, webpageEnd);
  }
}