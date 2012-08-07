#include <arduiparam.h>

int TIME_VAR = 5;
int TIME_CPT = 0;


ArduiParam ListOfCommand;

bool cbtime(ARDUI_COMMAND_TYPE_e type , int param)
{
  switch(type)
  {
    case ARDUI_PARAM_WRITE :
    Serial.print("\r\nset time var to : ");
    Serial.println (param , DEC );
    TIME_VAR = param;
    break;
    
    case ARDUI_PARAM_READ :
    Serial.print("\r\nget time var: ");
    Serial.println (TIME_VAR , DEC );
    break;
    
    case ARDUI_PARAM_ACT :
    Serial.print("\r\ntime no action: ");
    break;
    
    default : break;
  }
   Serial.print("\r\n");
  return true;
}

bool cbHelp(ARDUI_COMMAND_TYPE_e type , int param)
{
  char *str = ListOfCommand.GetFirstItemCmd();
  Serial.print("\r\n help : \r\n");
  while(str){
    Serial.print("\r\n");
    Serial.print(str);
    str =  ListOfCommand.GetNextItemCmd();
  }
}

bool cbAAA(ARDUI_COMMAND_TYPE_e type , int param)
{
  Serial.print("\r\n cmd AAA type :");
  Serial.println (type , DEC );
  Serial.print("\r\nparam :");
  Serial.println (param , DEC );
}

bool cbZZZ(ARDUI_COMMAND_TYPE_e type , int param)
{
    Serial.print("\r\n cmd ZZZ type :");
  Serial.println (type , DEC );
  Serial.print("\r\nparam :");
  Serial.println (param , DEC );
}

bool cbBBB(ARDUI_COMMAND_TYPE_e type , int param)
{
    Serial.print("\r\n cmd BBB type :");
  Serial.println (type , DEC );
  Serial.print("\r\nparam :");
  Serial.println (param , DEC );
}

bool cbFFF(ARDUI_COMMAND_TYPE_e type , int param)
{
    Serial.print("\r\n cmd FFF type :");
  Serial.println (type , DEC );
  Serial.print("\r\nparam :");
  Serial.println (param , DEC );
}


ArduiParamItem CmdAAA("AAA",cbAAA);
ArduiParamItem CmdBBB("BBB",cbBBB);
ArduiParamItem CmdZZZ("ZZZ",cbZZZ);
ArduiParamItem CmdFFF("FFF",cbFFF);
ArduiParamItem CmdTime("led",cbtime);
ArduiParamItem CmdHelp("help",cbHelp);

void setup ()
{
  Serial.begin(9600);
  ListOfCommand.AddCommand(&CmdTime);
  ListOfCommand.AddCommand(&CmdZZZ);
  ListOfCommand.AddCommand(&CmdBBB);
  ListOfCommand.AddCommand(&CmdAAA);
  ListOfCommand.AddCommand(&CmdFFF);
  ListOfCommand.AddCommand(&CmdHelp);
   pinMode(13, OUTPUT);  
}



void loop () 
{
   if (Serial.available() > 0) 
   {
     ARDUI_PARAM_ERROR_e err;
     err = ListOfCommand.PutChar(Serial.read());
     if (err > 0 )
     {
        Serial.print("\r\nArduiparam error: ");
        Serial.println (err , DEC );
     }   
   }
   else
   {
       TIME_CPT++;
       if (TIME_CPT > TIME_VAR*100)
       {
         static int led = 0;
         digitalWrite(13, led);
         if (led)
           led = 0;
          else
            led = 1;
            
          TIME_CPT = 0;
       }      
   }
   delay(1);
}
