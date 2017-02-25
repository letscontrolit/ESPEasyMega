//#######################################################################################################
//#################################### Plugin 038: NeoPixel Basic #######################################
//#######################################################################################################

// List of Command 
// (1) NeoPixel,<led nr>,<red 0-255>,<green 0-255>,<blue 0-255>
// (2) NeoPixelAll,<red 0-255>,<green 0-255>,<blue 0-255>
// (3) NeoPixelLine,<start led nr>,<stop led nr>,<red 0-255>,<green 0-255>,<blue 0-255>

// Usage:
// (1): Set RGB Colour to specified LED number (eg. NeoPixel,5,255,255,255)
// (2): Set all LED to specified colour (eg. NeoPixelAll,255,255,255)
//		If you use 'NeoPixelAll' this will off all LED (like NeoPixelAll,0,0,0)
// (3): Set colour LED between <start led nr> and <stop led nr> to specified colour (eg. NeoPixelLine,1,6,255,255,255)


#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel *Plugin_038_pixels;

#define PLUGIN_038
#define PLUGIN_ID_038         38
#define PLUGIN_NAME_038       "NeoPixel - Basic"
#define PLUGIN_VALUENAME1_038 ""
boolean Plugin_038(byte function, struct EventStruct *event, String& string)
{
	boolean success = false;

	switch (function)
	{

		case PLUGIN_DEVICE_ADD:
		{
			Device[++deviceCount].Number = PLUGIN_ID_038;
			Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
			Device[deviceCount].Custom = true;
			Device[deviceCount].TimerOption = false;
		break;
		}

		case PLUGIN_GET_DEVICENAME:
		{
			string = F(PLUGIN_NAME_038);	
		break;
		}

		case PLUGIN_GET_DEVICEVALUENAMES:
		{
			strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_038));
		break;
		}

		case PLUGIN_WEBFORM_LOAD:
		{
			char tmpString[128];
			sprintf_P(tmpString, PSTR("<TR><TD>Led Count:<TD><input type='text' name='plugin_038_leds' size='3' value='%u'>"), Settings.TaskDevicePluginConfig[event->TaskIndex][0]);
			string += tmpString;

			string += F("<TR><TD>GPIO:<TD>");
			addPinSelect(false, string, "taskdevicepin1", Settings.TaskDevicePin1[event->TaskIndex]);

			success = true;
		break;
		}

		case PLUGIN_WEBFORM_SAVE:
		{
			String plugin1 = WebServer.arg(F("plugin_038_leds"));
			Settings.TaskDevicePluginConfig[event->TaskIndex][0] = plugin1.toInt();
			success = true;
		break;
		}

		case PLUGIN_INIT:
		{
			if (!Plugin_038_pixels)
			{
				Plugin_038_pixels = new Adafruit_NeoPixel(Settings.TaskDevicePluginConfig[event->TaskIndex][0], Settings.TaskDevicePin1[event->TaskIndex], NEO_GRB + NEO_KHZ800);
				Plugin_038_pixels->begin();
			}
			success = true;
		break;
		}

		case PLUGIN_WRITE:
		{
			if (Plugin_038_pixels)
			{
				String tmpString  = string;
				int argIndex = tmpString.indexOf(',');
				if (argIndex) tmpString = tmpString.substring(0, argIndex);

				if (tmpString.equalsIgnoreCase(F("NeoPixel")))
				{
					char Line[80];
					char TmpStr1[80];
					TmpStr1[0] = 0;
					string.toCharArray(Line, 80);
					int Par4 = 0;
					if (GetArgv(Line, TmpStr1, 5)) Par4 = str2int(TmpStr1);
					Plugin_038_pixels->setPixelColor(event->Par1 - 1, Plugin_038_pixels->Color(event->Par2, event->Par3, Par4));
					Plugin_038_pixels->show();
					success = true;
				}

				if (tmpString.equalsIgnoreCase(F("NeoPixelAll")))
				{
					char Line[80];
					char TmpStr1[80];
					TmpStr1[0] = 0;
					string.toCharArray(Line, 80);
					for (int i = 0; i < (Settings.TaskDevicePluginConfig[event->TaskIndex][0]); i++)
					{
						Plugin_038_pixels->setPixelColor(i, Plugin_038_pixels->Color(event->Par1, event->Par2, event->Par3));
					}
					Plugin_038_pixels->show();
					success = true;  
				}

				if (tmpString.equalsIgnoreCase(F("NeoPixelLine")))
				{
					char Line[80];
					char TmpStr1[80];
					TmpStr1[0] = 0;
					string.toCharArray(Line, 80);

					int Par4 = 0;
					int Par5 = 0;
					if (GetArgv(Line, TmpStr1, 5)) Par4 = str2int(TmpStr1);
					if (GetArgv(Line, TmpStr1, 6)) Par5 = str2int(TmpStr1);

					for (int i = event->Par1 - 1; i < event->Par2; i++)
					{
						Plugin_038_pixels->setPixelColor(i, Plugin_038_pixels->Color(event->Par3, Par4, Par5));
					}
					Plugin_038_pixels->show();
					success = true;
				}
			}
		break;
		}
	}
return success;
}
