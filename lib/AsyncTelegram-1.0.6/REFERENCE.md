# Reference
Here you can find an explanation of the functionalities provided and how to use the library. 
Check the [examples folder](https://github.com/cotestatnt/AsyncTelegram/tree/master/examples) for demos and examples.
___
## Table of contents
+ [Introduction and quick start](#introduction-and-quick-start)
+ [Inline Keyboards](#inline-keyboards)
  + [Using Inline Keyboards into AsyncTelegram class](#using-inline-keyboards-into--class)
  + [Handling callback messages](#handling-callback-messages)
+ [Data types](#data-types)
  + [TBUser](#tbuser)
  + [TBLocation](#tblocation)
  + [TBGroup](#tbgroup)
  + [TBContact](#tbcontact)
  + [TBMessage](#tbmessage)
+ [Enumerators](#enumerators)
  + [MessageType](#messagetype)
  + [InlineKeyboardButtonType](#inlinekeyboardbuttontype)
+ [Basic methods](#basic-methods)
  + [AsyncTelegram::setTelegramToken()](#settelegramtoken)
  + [AsyncTelegram::testConnection()](#testconnection)
  + [AsyncTelegram::getNewMessage()](#getnewmessage)
  + [AsyncTelegram::sendMessage()](#sendmessage)
  + [AsyncTelegram::endQuery()](#endquery)
  + [AsyncTelegram::removeReplyKeyboard()](#removereplykeyboard)
  + [InlineKeyboard::addButton()](#inlinekeyboardaddbutton)
  + [InlineKeyboard::addRow()](#inlinekeyboardaddrow)
  + [InlineKeyboard::flushData()](#inlinekeyboardflushdata)
  + [InlineKeyboard::getJSON()](#inlinekeyboardgetjson)
+ [Configuration methods](#configuration-methods)
  + [AsyncTelegram::useDNS()](#usedns)
  + [AsyncTelegram::enableUTF8Encoding()](#enableutf8encoding)
  + [AsyncTelegram::setFingerprint()](#setfingerprint)
  + [AsyncTelegram::updateFingerprint()](#updatefingerprint)
___
## Introduction and quick start
Once installed the library, you have to load it in your sketch...
```c++
#include "AsyncTelegram.h"
```
...and instantiate a `AsyncTelegram` object
```c++
AsyncTelegram myBot;
```
...Use the `setTelegramToken()` member function to set your Telegram Bot token in order establish connections with the bot
```c++
myBot.setTelegramToken("myTelegramBotToken");
```
In order to receive messages, declare a `TBMessage` variable...
```c++
TBMessage msg;
```
...and execute the `getNewMessage()` member fuction. 
The `getNewMessage()` return a non-zero value if there is a new message and store it in the `msg` variable. See the [TBMessage](#tbmessage) data type for further details.
```c++
myBot.getNewMessage(msg);
```
To send a simple message to a Telegram user, use the `sendMessage(TBMessage msg, String text )` member function 
```c++
myBot.sendMessage(msg, "message");
```
See the [echoBot example](https://github.com/cotestatnt/AsyncTelegram/blob/master/examples/echoBot/echoBot.ino) for further details.

[back to TOC](#table-of-contents)
___
## Inline Keyboards
The Inline Keyboards are special keyboards integrated directly into the messages they belong to: pressing buttons on inline keyboards doesn't result in messages sent to the chat. Instead, inline keyboards support buttons that work behind the scenes.
AsyncTelegram class implements the following buttons:
+ URL buttons: these buttons have a small arrow icon to help the user understand that tapping on a URL button will open an external link. A confirmation alert message is shown before opening the link in the browser.
+ Callback buttons: when a user presses a callback button, no messages are sent to the chat. Instead, the bot simply receives the relevant query. Upon receiving the query, the bot can display some result in a notification at the top of the chat screen or in an alert. It's also possible associate a callback function that will be executed when user press the inline keyboard button.

[back to TOC](#table-of-contents)

### Using Inline Keyboards into AsyncTelegram class
In order to show an inline keyboard, use the method [sendMessage()](#sendmessage) specifing the parameter `keyboard`.
The `keyboard` parameter is a string that contains a JSON structure that define the inline keyboard. See [Telegram docs](https://core.telegram.org/bots/api#sendmessage).<br>
To simplify the creation of an inline keyboard, there is an helper class called `InlineKeyboard`.
Creating an inline keyboard with a `InlineKeyboard` is straightforward:

Fristly, instantiate a `InlineKeyboard` object:
```c++
InlineKeyboard kbd;
```
then add new buttons in the first row of the inline keyboard using the member fuction `addButton()` (See [addButton()](#addbutton) member function).
```c++
kbd.addButton("First Button label", "URL for first button", KeyboardButtonURL);                // URL button
kbd.addButton("Second Button label", "Data for second button", KeyboardButtonQuery, onPress);  // callback button
...
```
If a new row of buttons is needed, call the addRow() member function...
```c++
kbd.addRow();
```
... and add buttons to the just created row:
```c++
kbd.addButton("New Row Button label", "URL for the new row button", KeyboardButtonURL); // URL button
...
```
Once finished, send the inline keyboard using the `sendMessage` method:
```c++
myBot.sendMessage(<msg>, "message", kbd);
...
```
[back to TOC](#table-of-contents)

### Handling callback messages
Everytime an inline keyboard button is pressed, a special message is sent to the bot: the `getNewMessage()` returns `MessageQuery` value and the `TBMessage` data structure is filled with the callback data.
When query button is pressed, is mandatory to notify the Telegram Server the end of the query process by calling the `endQuery()` method.
Here an example:
```c++
#include "AsyncTelegram.h"
#define CALLBACK_QUERY_DATA  "QueryData"  // callback data sent when the button is pressed
AsyncTelegram myBot;
InlineKeyboard myKbd;  // custom inline keyboard object helper

void setup() {
   Serial.begin(115200); // initialize the serial
   WiFi.mode(WIFI_STA); 	
   WiFi.begin(ssid, pass);
   myBot.setTelegramToken("myTelegramBotToken"); // set the telegram bot token

	// inline keyboard - only a button called "My button"
	myKbd.addButton("My button", CALLBACK_QUERY_DATA, KeyboardButtonQuery);
}

void loop() {
	TBMessage msg; // a variable to store telegram message data

	// if there is an incoming message...
	if (myBot.getNewMessage(msg)) {
		// ...and if it is a callback query message
	    if (msg.messageType == CTBotMessageQuery) {
			// received a callback query message, check if it is the "My button" callback
			if (msg.callbackQueryData.equals(CALLBACK_QUERY_DATA)) {
				// pushed "My button" button --> do related things...

				// close the callback query
				myBot.endQuery(msg, "My button pressed");
			}
		} else {
			// the received message is a text message --> reply with the inline keyboard
			myBot.sendMessage(msg, "Inline Keyboard", myKbd);
		}
	}
	delay(500); // wait 500 milliseconds
}
```
See the [keyboards example](https://github.com/shurillu/AsyncTelegram/blob/master/examples/keyboards/keyboards.ino) for further details. <br>

[back to TOC](#table-of-contents)
___
## Data types
There are several usefully data structures used to store data typically sent by the Telegram Server.
### `TBUser`
`TBUser` data type is used to store user data like Telegram userID. The data structure contains:
```c++
uint32_t     id;
bool         isBot;
const char*  firstName;
const char*  lastName;
const char*  username;
const char*  languageCode;
```
where:
+ `id` is the unique Telegram user ID
+ `isBot` tells if the user ID `id` refers to a bot (`true` value) or not (`false ` value)
+ `firstName` contains the first name (if provided) of the user ID `id`
+ `lastName` contains the last name (if provided) of the user ID `id`
+ `username` contains the username of the user ID `id`
+ `languageCode` contains the country code used by the user ID `id`

Typically, you will use predominantly the `id` field.

[back to TOC](#table-of-contents)
### `TBLocation`
`TBLocation` data type is used to store the longitude and the latitude. The data structure contains:
```c++
float longitude;
float latitude;
```
where:
+ `longitude` contains the value of the longitude
+ `latitude` contains the value of the latitude

For localization messages, see [TBMessage](#tbmessage)

[back to TOC](#table-of-contents)
### `TBGroup`
`TBGroup` data type is used to store the group chat data. The data structure contains:
```c++
int64_t       id;
const char*   title;
```
where:
+ `id` contains the ID of the group chat
+ `title` contains the title of the group chat

[back to TOC](#table-of-contents)


### `TBContact`
`TBContact` data type is used to store the contact data. The data structure contains:
```c++
const char*   phoneNumber;
const char*   firstName;
const char*   lastName;
int32_t       id;
const char*   vCard;
```
where:
+ `phoneNumber` contains the phone number of the contact
+ `firstName` contains the first name of the contact
+ `lastName` contains the last name of the contact
+ `id` contains the ID of the contact
+ `vCard` contains the vCard of the contact

[back to TOC](#table-of-contents)


### `TBMessage`
`TBMessage` data type is used to store new messages. The data structure contains:
```c++
uint32_t         messageID;
TBUser           sender;
TBGroup          group;
uint32_t         date;
const char*      text;
const char*      chatInstance;
const char*      callbackQueryData;
const char*      callbackQueryID;
TBLocation       location;
TBcontact        contact;
MessageType      messageType;
```
where:
+ `messageID` contains the unique message identifier associated to the received message
+ `sender` contains the sender data in a [TBUser](#tbuser) structure
+ `group` contains the group chat data in a [TBGroup](#tbgroup) structure
+ `date` contains the date when the message was sent, in Unix time
+ `text` contains the received message (if a text message is received - see [AsyncTelegram::getNewMessage()](#getnewmessage))
+ `chatInstance` contains the unique ID corresponding to the chat to which the message with the callback button was sent
+ `callbackQueryData` contains the data associated with the callback button
+ `callbackQueryID` contains the unique ID for the query
+ `location` contains the location's longitude and latitude (if a location message is received - see [AsyncTelegram::getNewMessage()](#getnewmessage))
+ `contact` contains the contact information a [TBContact](#tbcontact) structure
+ `messageType` contains the message type. See [CTBotMessageType](#messagetype)

[back to TOC](#table-of-contents)
___
## Enumerators
There are several usefully enumerators used to define method parameters or method return value.

### `MessageType`
Enumerator used to define the possible message types received by [getNewMessage()](#getnewmessage) method. Used also by [TBMessage](#tbmessage).
```c++
enum MessageType {
	MessageNoData   = 0,
	MessageText     = 1,
	MessageQuery    = 2,
	MessageLocation = 3,
	MessageContact  = 4
};
```
where:
+ `MessageNoData`: error - the [TBMessage](#tbmessage) structure contains no valid data
+ `MessageText`: the [TBMessage](#tbmessage) structure contains a text message
+ `MessageQuery`: the [TBMessage](#tbmessage) structure contains a calback query message (see [Inline Keyboards](#inline-keyboards))
+ `MessageLocation`: the [TBMessage](#tbmessage) structure contains a localization message
+ `MessageContact`: the [TBMessage](#tbmessage) structure contains a contact message

[back to TOC](#table-of-contents)

### `InlineKeyboardButtonType`
Enumerator used to define the possible button types. Button types are used when creating an inline keyboard with [addButton()](#addbutton) method.
```c++
enum InlineKeyboardButtonType {
	KeyboardButtonURL    = 1,
	KeyboardButtonQuery  = 2
};

```
where:
+ `KeyboardButtonURL`: define a URL button. When pressed, Telegram client will ask if open the URL in a browser
+ `KeyboardButtonQuery`: define a calback query button. When pressed, a callback query message is sent to the bot

[back to TOC](#table-of-contents)


___
## Basic methods
Here you can find the basic member function. First you have to instantiate a AsyncTelegram object, like ` myBot`, then call the desired member function as `myBot.myDesiredFunction()`

[back to TOC](#table-of-contents)
### `AsyncTelegram::setTelegramToken()`
`void AsyncTelegram::setTelegramToken(String token)` <br><br>
Set the Telegram Bot token. If you need infos about Telegram Bot and how to obtain a token, take a look  [here](https://core.telegram.org/bots#6-botfather). <br>
Parameters:
+ `token`: the token that identify the Telegram Bot

Returns: none. <br>
Example:
+ `setTelegramToken("myTelegramBotToken")`


[back to TOC](#table-of-contents)
### `AsyncTelegram::begin()`
`bool AsyncTelegram::begin(void)` <br><br>
Check the connection between ESP8266 board and the Telegram server. <br>
Parameters: none <br>
Returns: `true` if the ESP8266 is able to send/receive data to/from the Telegram server. <br>
Example:
```c++
#include "AsyncTelegram.h"
AsyncTelegram myBot;
void setup() {
   Serial.begin(115200); // initialize the serial
   myBot.wifiConnect("mySSID", "myPassword"); // connect to the WiFi Network
   myBot.setTelegramToken("myTelegramBotToken"); // set the telegram bot token
   if(myBot.begin())
      Serial.println("Connection OK");
   else
      Serial.println("Connectionk NOK");
}
void loop() {
}
```

[back to TOC](#table-of-contents)
### `AsyncTelegram::getNewMessage()`

`AsyncTelegramMessageType AsyncTelegram::getNewMessage(TBMessage &message)` <br><br>
Get the first unread message from the message queue. Fetch text message and callback query message (for callback query messages, see [Inline Keyboards](#inline-keyboards)). This is a destructive operation: once read, the message will be marked as read so a new `getNewMessage` will fetch the next message (if any). <br>
Parameters:
+ `message`: a `TBMessage` data structure that will contains the message data retrieved

Returns:
+ `MessageNoData` if an error occurred
+ `MessageText` if the message received is a text message 
+ `MessageQuery` if the message received is a callback query message (see [Handling callback messages](#handling-callback-messages))
+ `MessageLocation` if the message received is a location message
+ `MessageContact` if the message received is a contact message


[back to TOC](#table-of-contents)
### `AsyncTelegram::sendMessage()`
`void sendMessage(const TBMessage &msg, const char* message, String keyboard = "");` <br>
`void sendMessage(const TBMessage &msg, String &message, String keyboard = "");` <br>
`void sendMessage(const TBMessage &msg, const char* message, ReplyKeyboard  &keyboard);` <br>
`void sendMessage(const TBMessage &msg, const char* message, InlineKeyboard &keyboard);	` <br><br>

Send a message to the Telegram user ID associated with recevied msg. <br>
If `keyboard` parameter is specified, send the message and display the custom keyboard (inline or reply). 
+ Inline keyboard are defined by a JSON structure (see the Telegram API documentation [InlineKeyboardMarkup](https://core.telegram.org/bots/api#inlinekeyboardmarkup))<br>
You can also use the helper class InlineKeyboard for creating inline keyboards.<br> 
+ Reply keyboard are define by a JSON structure (see Telegram API documentation [ReplyKeyboardMarkup](https://core.telegram.org/bots/api#replykeyboardmarkup))<br>
You can also use the helper class ReplyKeyboard for creating inline keyboards.<br> 

Parameters:
+ `msg`: the TBMessage recipient structure
+ `message`: the message to send
+ `keyboard`: (optional) the inline/reply keyboard

[back to TOC](#table-of-contents)





### `AsyncTelegram::removeReplyKeyboard()`
`bool removeReplyKeyboard(int64_t id, String message, bool selective = false)` <br><br>
Remove an active replyKeyboard for a specified user by sending a message. <br>
Parameters:
+ `msg`: the TBMessage recipient structure
+ `message`: the message to be show to the selected user ID
+ `selective`: (optional) enable the selective mode (hide the keyboard for specific users only). Useful for hiding the keyboard for users that are @mentioned in the text of the Message object or if the bot's message is a reply (has reply_to_message_id), sender of the original message

Returns: `true` if no error occurred. <br>

[back to TOC](#table-of-contents)




### `InlineKeyboard::addButton()`
`bool addButton(const char* text, const char* command, InlineKeyboardButtonType buttonType, CallbackType onClick = nullptr)` <br><br>
Add a button to the current keyboard row of an InlineKeyboard object. For a description of button types, see [Inline Keyboards](#inline-keyboards).<br>
Parameters: 
+ `text`: the botton text (label) displayed on the inline keyboard
+ `command`: depending on the button type, 
  + on URL buttons, contain the URL
  + on a query button, contain the query data
+ `buttonType`: set the behavior of the button. It can be:
  + `KeyboardButtonURL` - the added button will be a URL button
  + `KeyboardButtonQuery` - the added button will be a query button
+ `onClick`: pointer to callback function
Returns: `true` if no error occurred. <br>

[back to TOC](#table-of-contents)

### `InlineKeyboard::addRow()`
`bool InlineKeyboard::addRow(void)` <br><br>
Add a new empty row of buttons to the inline keyboard: all the new keyboard buttons will be added to this new row.
Parameters: none <br>
Returns: `true` if no error occurred. <br>


[back to TOC](#table-of-contents)

### `InlineKeyboard::getJSON()`
`String InlineKeyboard::getJSON(void)` <br><br>
Create a string that containsthe inline keyboard formatted in a JSON structure. Useful sending the inline keyboard with [sendMessage()](#sendmessage).
Parameters: none <br>
Returns: the JSON of the inline keyboard <br>

[back to TOC](#table-of-contents)

___
## Configuration methods
With the following methods, is possible to change the behavior of the AsyncTelegram instantiated object.

[back to TOC](#table-of-contents)
### `AsyncTelegram::useDNS()`
`void AsyncTelegram::useDNS(bool value)` <br><br>
Define which kind of address (symbolic address or fixed IP) will be used to establish connections with the Telegram server. <br>
Default value is `false` (use fixed IP) <br>
Is better to use fixed IP when no DNS server are provided. <br>
Parameters:
+ `value`: set `true` if you want to use the URL style address "api.telegram.org" or set `false` if you want to use the fixed IP address "149.154.167.198".

Returns: none. <br>
Examples:
+ `useDNS(true)`: for every connection with the Telegram server, will be used the URL style address "api.telegram.org"
+ `useDNS(false)`: for every connection with the Telegram server, will be used the fixed IP address "149.154.167.198"

[back to TOC](#table-of-contents)
### `AsyncTelegram::enableUTF8Encoding()`
`void AsyncTelegram::enableUTF8Encoding(bool value)` <br><br>
Tipically, Telegram server encodes messages with an UNICODE like format. This mean for example that a '€' character is sent by Telegram server encoded in this form \u20AC (UNICODE). For some weird reasons, the backslash character disappears and the message you get is u20AC thus is impossible to correctly decode an incoming message.
Encoding the received message with UTF8 encoding format will solve the problem.
Encoding messages in UTF8 format will consume a bit of CPU time. <br>
Default value is `false` (no UTF8 conversion). <br>
Parameters:
+ `value`: set `true`to enable the UTF8 encoding for all incoming messages; set `false`to disable this feature.

Returns: none. <br>
Examples:
+ `enableUTF8Encoding(true)`: every incoming message will be encoded in UTF8
+ `enableUTF8Encoding(false)`: every incoming message is encoded as Telegram server do

[back to TOC](#table-of-contents)
### `AsyncTelegram::setFingerprint()`
`void AsyncTelegram::setFingerprint(const uint8_t *newFingerprint)` <br><br>
Set the new Telegram API server fingerprint overwriting the default one.
The fingerprint can be obtained by [this service](https://www.grc.com/fingerprints.htm) provided by Gibson Research Corporation. To obtain the new fingerprint, just query for `api.telegram.org`

Default value is `BB:DC:45:2A:07:E3:4A:71:33:40:32:DA:BE:81:F7:72:6F:4A:2B:6B`.<br>
Parameters:
+ `newFingerprint`: the 20 bytes array that contains the new fingerprint.

Returns: none. <br>
Example:
```c++
void setup() {
   ...
   uint8_t telegramFingerprint [20] = { 0xBB, 0xDC, 0x45, 0x2A, 0x07, 0xE3, 0x4A, 0x71, 0x33, 0x40, 0x32, 0xDA, 0xBE, 0x81, 0xF7, 0x72, 0x6F, 0x4A, 0x2B, 0x6B };
   myBot.setFingerprint(telegramFingerprint);
   ...
}

void loop(){
   ...
}
```
[back to TOC](#table-of-contents)



[back to TOC](#table-of-contents)
### `AsyncTelegram::updateFingerprint()`
`bool AsyncTelegram::updateFingerPrint(void);` <br><br>
Parse the reply obtained from online service [this service](https://www.grc.com/fingerprints.htm?chain=api.telegram.org) and set the new Telegram API server fingerprint overwriting the default one.

Returns: none. <br>
Example:
```c++
void setup() {
   ...
   myBot.updateFingerprint();
   ...
}

void loop(){
   ...
}
```
[back to TOC](#table-of-contents)


