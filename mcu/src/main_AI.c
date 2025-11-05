///*
//File: Lab_6_JHB.c
//Author: Josh Brake
//Email: jbrake@hmc.edu
//Date: 9/14/19

//modified 10/29/2025
//corey hickson chickson@hmc.edu
//*/


//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include "main.h"

///////////////////////////////////////////////////////////////////
//// Provided Constants and Functions
///////////////////////////////////////////////////////////////////

////Defining the web page in two chunks: everything before the current time, and everything after the current time

//char* AI_page = "\
//<!DOCTYPE html>\
//<html lang=\"en\">\
//<head>\
//  <meta charset=\"UTF-8\" />\
//  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>\
//  <title>Temperature Sensor Dashboard</title>\
//  <style>\
//    body {\
//      font-family: 'Segoe UI', sans-serif;\
//      background: #f4f7f9;\
//      color: #333;\
//      margin: 0;\
//      padding: 0;\
//    }\
//    header {\
//      background: #0078d4;\
//      color: white;\
//      padding: 1rem;\
//      text-align: center;\
//    }\
//    .container {\
//      max-width: 600px;\
//      margin: 2rem auto;\
//      padding: 2rem;\
//      background: white;\
//      border-radius: 8px;\
//      box-shadow: 0 2px 8px rgba(0,0,0,0.1);\
//    }\
//    .section {\
//      margin-bottom: 2rem;\
//    }\
//    .section h2 {\
//      margin-bottom: 1rem;\
//      font-size: 1.2rem;\
//      color: #0078d4;\
//    }\
//    .temperature-display {\
//      font-size: 2.5rem;\
//      font-weight: bold;\
//      text-align: center;\
//      margin: 1rem 0;\
//    }\
//    .controls {\
//      display: flex;\
//      justify-content: space-between;\
//      align-items: center;\
//    }\
//    .controls label {\
//      margin-right: 0.5rem;\
//    }\
//    .led-toggle {\
//      display: flex;\
//      align-items: center;\
//    }\
//    button {\
//      padding: 0.5rem 1rem;\
//      background: #0078d4;\
//      color: white;\
//      border: none;\
//      border-radius: 4px;\
//      cursor: pointer;\
//    }\
//    button:hover {\
//      background: #005fa3;\
//    }\
//    select {\
//      padding: 0.4rem;\
//    }\
//  </style>\
//</head>\
//<body>\
//  <header>\
//    <h1>Temperature Sensor Dashboard</h1>\
//  </header>\
//  <div class=\"container\">\
//    <div class=\"section\">\
//      <h2>Current Temperature</h2>\
//      <div class=\"temperature-display\" id=\"tempValue\">-- °C</div>\
//    </div>\
//    <div class=\"section controls\">\
//      <div class=\"led-toggle\">\
//        <label for=\"ledSwitch\">LED:</label>\
//        <button id=\"ledSwitch\">Toggle</button>\
//      </div>\
//      <div>\
//        <label for=\"precisionSelect\">Precision:</label>\
//        <select id=\"precisionSelect\">\
//          <option value=\"0\">0 decimal</option>\
//          <option value=\"1\" selected>1 decimal</option>\
//          <option value=\"2\">2 decimals</option>\
//        </select>\
//      </div>\
//    </div>\
//  </div>\
//  <script>\
//    let ledState = false;\
//    let precision = 1;\
//    function fetchTemperature() {\
//      // Simulate fetching temperature from a sensor\
//      const rawTemp = 22.3456 + Math.random(); // Simulated temperature\
//      const temp = rawTemp.toFixed(precision);\
//      document.getElementById('tempValue').textContent = `${temp} °C`;\
//    }\
//    document.getElementById('ledSwitch').addEventListener('click', () => {\
//      ledState = !ledState;\
//      // Replace with actual API call to toggle LED\
//      alert(`LED turned ${ledState ? 'ON' : 'OFF'}`);\
//    });\
//    document.getElementById('precisionSelect').addEventListener('change', (e) => {\
//      precision = parseInt(e.target.value);\
//      fetchTemperature();\
//    });\
//    // Initial fetch and update every 5 seconds\
//    fetchTemperature();\
//    setInterval(fetchTemperature, 5000);\
//  </script>\
//</body>\
//</html>";

////determines whether a given character sequence is in a char array request, returning 1 if present, -1 if not present
//int inString(char request[], char des[]) {
//	if (strstr(request, des) != NULL) {return 1;}
//	return -1;
//}

//int updateLEDStatus(char request[], int prev_led_status) {

//	int led_status = 0;
//	// The request has been received. now process to determine whether to turn the LED on or off
//	if (inString(request, "ledoff")==1) {
//		digitalWrite(LED_PIN, GPIO_LOW);
//		led_status = 0;
//	}
//	else if (inString(request, "ledon")==1) {
//		digitalWrite(LED_PIN, GPIO_HIGH);
//		led_status = 1;
//	} else {
//                led_status = prev_led_status;
//        }

//	return led_status;
//}

//int updateTempStatus(char request[]) {

//  volatile int temp_status = 0;

//  if      (inString(request, "resolution8")==1) {
//		spiWrite(CONFIG_WADDR, RESOLUTION_8);
//	}  
//  else if (inString(request, "resolution9")==1) {
//		spiWrite(CONFIG_WADDR, RESOLUTION_9);
//	}  
//  else if (inString(request, "resolution10")==1) {
//		spiWrite(CONFIG_WADDR, RESOLUTION_10);
//	}  
//  else if (inString(request, "resolution11")==1) {
//		spiWrite(CONFIG_WADDR, RESOLUTION_11);
//	}  
//  else if (inString(request, "resolution12")==1) {
//		spiWrite(CONFIG_WADDR, RESOLUTION_12);
//	}  

//  delay_millis(TIM15, 200);
//  temp_status = spiRead(TMPMSB_RADDR);
//  temp_status = temp_status << 8;
//  temp_status = temp_status | spiRead(TMPLSB_RADDR);

//  return temp_status;

//}

///////////////////////////////////////////////////////////////////
//// Solution Functions
///////////////////////////////////////////////////////////////////

//int main(void) {

//  int led_status = 0;

//  configureFlash();
//  configureClock();

//  gpioEnable(GPIO_PORT_A);
//  gpioEnable(GPIO_PORT_B);
//  gpioEnable(GPIO_PORT_C);

//  pinMode(LED_PIN, GPIO_OUTPUT);
//  digitalWrite(LED_PIN, 0);
  
//  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
//  initTIM(TIM15);
  
//  USART_TypeDef * USART = initUSART(USART1_ID, 125000);
  
//  // SPI initialization code
//  spiInit(0b111 , 0, 1);

//  while(1) {
//    /* Wait for ESP8266 to send a request.
//    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
//    Therefore the request[] array must be able to contain 18 characters.
//    */

//    // Receive web request from the ESP
//    char request[BUFF_LEN] = "                  "; // initialize to known value
//    int charIndex = 0;
  
//    // Keep going until you get end of line character
//    while(inString(request, "\n") == -1) {
//      // Wait for a complete request to be transmitted before processing
//      while(!(USART->ISR & USART_ISR_RXNE));
//      request[charIndex++] = readChar(USART);
//    }

//    //  SPI code for reading temperature
//    volatile int temp_status = updateTempStatus(request);
//    //int temp_status = 0;
//    int sign = (temp_status >> 15) & 1;

//    char tempStatusStr[50];

//    temp_status &= ~(1 << 15);               // mask off sign bit to find magnitude
//    float temperature = temp_status / 256.0f; // convert fixed pt to floating pt
//    temperature = sign ? -temperature : temperature; 
//    sprintf(tempStatusStr, "Temperature: %.4f deg. C", temperature);
    

//    // Update string with current LED state
  
//    led_status = updateLEDStatus(request, led_status);

//    char ledStatusStr[20];
//    if (led_status == 1)
//      sprintf(ledStatusStr,"LED is on!");
//    else if (led_status == 0)
//      sprintf(ledStatusStr,"LED is off!");

//    digitalWrite(LED_PIN, led_status);

    

//    // finally, transmit the webpage over UART
//    //sendString(USART, webpageStart); // webpage header code
//    //sendString(USART, ledStr); // button for controlling LED
//    //sendString(USART, tempStr); // button for controlling temp sensor
    

//    //sendString(USART, "<h2>LED Status</h2>");

//    //sendString(USART, "<p>");
//    //sendString(USART, ledStatusStr);
//    //sendString(USART, "</p>");


//    //sendString(USART, "<h2>Temperature Status</h2>");

//    //sendString(USART, "<p>");
//    //sendString(USART, tempStatusStr);
//    //sendString(USART, "</p>");  

  
//    //sendString(USART, webpageEnd);

//    sendString(USART, AI_page);
//  }
//}