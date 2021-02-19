/*
  Sixfab_Tracker.h 
  -
  Library for Sixfab Tracker Shield for Arduino.
  -
  Created by Yasin Kaya (selengalp), September 10, 2018.
*/

#ifndef _SIXFAB_TRACKER_H
#define _SIXFAB_TRACKER_H

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
    #include "pins.h"    
#endif

#include <stdio.h>
#include <string.h>
#include <SoftwareSerial.h>

// determine board type
// Arduino Geniuno / Uno or Mega
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define L96 Serial // 9600 baud rate
  #define DEBUG Serial // 115200 baud rate
// Tinylab, Arduino Leonardo or Micro  
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  #define L96 Serial1 // 9600 baud rate
  #define DEBUG Serial // 115200 baud rate
#endif

extern SoftwareSerial M95_AT;   //9600 baudrate

// Peripheral Pin Definations
#define USER_BUTTON 9
#define USER_LED 6
#define FORCE_ON 4
#define GEO_FENCE 3
#define L96_RESET 2
#define M95_POWERKEY A3 
#define STATUS A2 
#define ENABLE A1
#define THREE_D_FIX A0 

// Constants  
#define TIMEOUT 1000
#define IP_ADDRESS_LEN 30
#define DOMAIN_NAME_LEN 50
#define PORT_NUMBER_LEN 8  
#define AT_COMM_LEN 100
#define AT_RESPONSE_LEN 100
#define DATA_COMPOSE_LEN 100
#define DATA_LEN_LEN 3  

class SixfabTracker
{
  public:
    
    /*
    Default constructer with no parameter

    [no-return]
    ---
    [no-param]
    */
    SixfabTracker();
    
/******************************************************************************************
 *** Base Functions : Set or Clear Hardwares - Status Controls - Helper AT Functions  *****
 ******************************************************************************************/
    
    /*
    Function for initializing module and peripherals
    This function do things below:
    * enables M95 module
    * sets pin directions

    [no-return]
    ---
    [no-param]
    */
    void init(); // initialize

    /*
    Function for powering up M95 module and all peripherals from voltage regulator 

    [no-return]
    ---
    [no-param]
    */
    void enable();
    
    /*
    Function for powering down M95 module and all peripherals from voltage regulator 

    [no-return]
    ---
    [no-param]
    */
    void disable();

    /*
    Function for powering up or down M95 module

    [no-return]
    ---
    [no-param]
    */
    void powerUp();

        /*
    Function for getting modem status (is powered up?).
    
    [return] :  uint8_t modem status (0 : powered up) - (1: powered down)
    ---
    [no-param]
    */
    uint8_t getModemStatus();

    /*
    Function for sending AT [param #1] command to M95.
    
    [no-return]
    ---
    [param #1] : const char* AT command word
    */
    void sendATCommOnce(const char *);

    /*
    Function for getting response from modem
    
    [no-return]
    ---
    [param #1] : const char* Desired response
    */
    const char* getResponse(const char *);

    /*
    Function for sending AT [param #1] command to M95. If the desired [param #2] 
    response isn't recevived, function resend the AT command wait a time as [timeout].
    
    [return] : const char* response of AT command that received from M95 modem
    ---
    [param #1] : const char* AT command word
    [param #2] : const char* AT desired_response word
    [param #3] : const char* AT response word
    */
    const char* sendATComm(const char *, const char *); 
	
	/*
    Function for sending Data [param #1] to M95. If the desired [param #2] 
    response isn't recevived, function resend the Data wait a time as [timeout].
    
    [return] : const char* response of Data that received from M95 modem
    ---
    [param #1] : const char* Data word
    [param #2] : const char* Data desired_response word
    [param #3] : const char* Data response word
    */
	const char* sendDataComm(const char *, const char *);

    /*
    Function for resetting M95 module and all peripherals.

    [no-return]
    ---
    [no-param]
    */
    void resetModule();

   /*
    Function to save configurations that be done in current session.   
    
    [no-return]
    --
    [no-param]
    */
    void saveConfigurations();

    /*
    Function for getting IMEI number

    [return] : const char* IMEI number
    [no-param]
    */
    const char* getIMEI();

    /*
    Function for getting firmware info

    [return] : const char* firmware info
    ---
    [no-param]
    */
    const char* getFirmwareInfo();

    /*
    Function for getting hardware info

    [return] : const char* hardware info
    ---
    [no-param]
    */
    const char* getHardwareInfo();

    /*
    Function for getting described ip address

    [return] : const char* IP Address
    ---
    [no-param]
    */
    const char* getIPAddress();

    /*
    Function for setting desired ip address

    [no-return]
    ---
    [param #1] : char * desired ip address
    */
    void setIPAddress(char *);

    /*
    Function for getting described domain name

    [return] : const char* domain name
    ---
    [no-param]
    */
    const char* getDomainName();

    /*
    Function for setting desired domain name

    [no-return]
    ---
    [param #1] : char * desired domain name
    */
    void setDomainName(char *);

    /*
    Function for getting described port number

    [return] : const char* port number
    ---
    [no-param]
    */
    const char* getPort();

    /*
    Function for setting desired port number

    [no-return]
    ---
    [param #1] : char * desired port number
    */
    void setPort(char *);

    /*
    Function for getting described timeout

    [return] : uint16_t described timeout in ms
    ---
    [no-param]
    */
    uint16_t getTimeout();

    /*
    Function for setting desired timeot

    [no-return]
    ---
    [param #1] : uint16_t desired timeout in ms
    */
    void setTimeout(uint16_t);

/******************************************************************************************
 *** SIM Related commands Functions ************************************************************
 ******************************************************************************************/ 


    const char* showICCID(void);
    const char* simStatus(void);
	const char* enterPinCode(const char *pinCode);	// MSMORKAN 02.2021
/******************************************************************************************
 *** Network Service Functions ************************************************************
 ******************************************************************************************/ 

    /*
    Function for getting signal quality

    [return] : const char* signal quality
    ---
    [no-param]
    */
    const char* getSignalQuality();

   /*
    Function for connecting to base station of operator

    [no-return]
    ---
    [no-param]
    */
    void connectToOperator();

    void checkOperator();
    void sendSMS(const char *, const char *);
	String readSMS(uint8_t index);				// MSMORKAN 02.2021
	void deleteAllSMS(void);					// MSMORKAN 02.2021

/******************************************************************************************
 *** L96 GNSS Functions ***********************************************************************
 ******************************************************************************************/
    /* 
    function for getting raw nmea messages 

    [return] char
    ---
    [no-param]
    */ 
    char getRawCharFromL96();
    
      /* 
    function for getting raw nmea sentence 

    [return] String
    ---
    [no-param]
    */ 
    String getNmeaSentence(void);
      /* 
    function for checking the 3D Fix 

    [return] bool
    ---
    [no-param]
    */ 

    bool is3DFixed(void);
/******************************************************************************************
 *** TCP & UDP Protocols Functions ********************************************************
 ******************************************************************************************/
    /* 
    function for configurating and activating TCP context 

    [no-return]
    ---
    [no-param]
    */ 
    void activateContext();

    /* 
    function for deactivating TCP context

    [no-return]
    ---
    [no-param]
    */ 
    void deactivateContext();

    /* 
    function for connecting to server via TCP
    just buffer access mode is supported for now.

    [no-return]
    ---
    [no-param]
    */
    void connectToServerTCP();

        /*
    Function for sending data via TCP protocol. 
    First use setIPAddress and setPort functions before 
    try to send data with this function.  

    [param #1] : const char* data word
    */
    void sendDataTCP(const char *);

    /* 
    // function for connecting to server via UDP

    [no-return]
    ---
    [no-param]
    */
    void startUDPService();

    /*
    Function for sending data via UDP protocol. 
    First use setIPAddress and setPort functions before 
    try to send data with this function.  

    [param #1] : const char* data word
    */
    void sendDataUDP(const char *);

    /* 
    Function for closing server connection
    
    [no-return]
    ---
    [no-param]
    */
    void closeConnection();


/******************************************************************************************
 *** Peripheral Devices' Functions : Read sensors - Set Relay and LEDs ********************
 ******************************************************************************************/    
    /* 
    Function for reading button position.
    
    [return] : uint8_t button position 0 or 1
    ---
    [no-param]
    */
    uint8_t readUserButton();

    /* 
    Function for turning on LED.
    
    [no-return]
    ---
    [no-param]
    */
    void turnOnUserLED();

    /* 
    Function for turning off LED.
    
    [no-return]
    ---
    [no-param]
    */
    void turnOffUserLED();

  private:
    char compose[100];
	char url[68];

    char ip_address[IP_ADDRESS_LEN]; //ip address       
    char domain_name[DOMAIN_NAME_LEN]; // domain name   
    char port_number[PORT_NUMBER_LEN]; // port number 
    uint16_t timeout = TIMEOUT; // default timeout for function and methods on this library.
    String gps;
/******************************************************************************************
 *** Private Functions that be used in public methods, in order to ease the operations ****
 ******************************************************************************************/
    /* 
    Function for clear command buffer #private param : compose[100].
    
    [no-return]
    ---
    [no-param]
    */
    void clear_compose()
    {
        memset(compose,0,sizeof(compose));
    }
};
#endif