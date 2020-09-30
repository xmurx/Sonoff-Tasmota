#include "InlineKeyboard.h"



InlineKeyboard::InlineKeyboard()
{
	m_json = "{\"inline_keyboard\":[[]]}\"";

}

InlineKeyboard::~InlineKeyboard(){} 


bool InlineKeyboard::addRow()
{
	if(m_jsonSize < BUFFER_SMALL) m_jsonSize = BUFFER_SMALL;	
	DynamicJsonBuffer buffer(m_jsonSize + 64);	 // Current size + space for new row (empty)
	JsonObject& root = buffer.parseObject(m_json);
	JsonArray& rows = root["inline_keyboard"];	
	rows.createNestedArray();
	m_json.clear();
	root.printTo(m_json);
	m_jsonSize = m_json.length();
	return true;
}


bool InlineKeyboard::addButton(const char* text, const char* command, InlineKeyboardButtonType buttonType, CallbackType onClick)
{
	if ((buttonType != KeyboardButtonURL) && (buttonType != KeyboardButtonQuery))
		return false;
	
	InlineButton *inlineButton = new InlineButton();
	if (_firstButton == nullptr) 
		_firstButton = inlineButton;
	else
		_lastButton->nextButton = inlineButton;
	inlineButton->argCallback = onClick;
	inlineButton->btnName = (char*)command;
	_lastButton = inlineButton;
	m_buttonsCounter++;
	
	// As reccomended use local JsonDocument instead global
	// inline keyboard json structure will be stored in a String var
	if(m_jsonSize < BUFFER_SMALL) m_jsonSize = BUFFER_SMALL;	
	DynamicJsonBuffer buffer(m_jsonSize + 128); // Current size + space for new object (button)
	JsonObject& root = buffer.parseObject(m_json);

	JsonArray& rows = root["inline_keyboard"];
  JsonObject& button = rows.createNestedObject();

	button["text"] = text ; 
	if(KeyboardButtonURL == buttonType) 
		button["url"] = command;	
	else if (KeyboardButtonQuery == buttonType) 		
		button["callback_data"] = command;	

	// Store inline keyboard json structure
	m_json.clear();
	root.printTo(m_json);
	m_jsonSize = m_json.length();
	return true;	
}


// Check if a callback function has to be called for this button query message
void InlineKeyboard::checkCallback( const TBMessage &msg)  {
	char* buttonName = (char*) msg.callbackQueryData;
	for(InlineButton *_button = _firstButton; _button != nullptr; _button = _button->nextButton){
		if( strstr(_button->btnName, buttonName) != nullptr && _button->argCallback != nullptr)	
			_button->argCallback(msg);
	}
} 


// Get total number of keyboard buttons
int InlineKeyboard::getButtonsNumber() 
{
	return m_buttonsCounter;
}



String InlineKeyboard::getJSON() const
{
	return m_json;
}


String InlineKeyboard::getJSONPretty() const
{
	uint16_t jsonSize;
	if(m_jsonSize < BUFFER_SMALL) jsonSize = BUFFER_SMALL;	
	DynamicJsonBuffer buffer(jsonSize + 64);	// Current size + space for new lines
	JsonObject& root = buffer.parseObject(m_json);
	
	String serialized;		
	root.prettyPrintTo(serialized);
	return serialized;
}

