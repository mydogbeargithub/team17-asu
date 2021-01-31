%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Title: EEE 488 Break Alert System Matlab Testing
%
% Description: This file serves as an initial test of the Break Alert
% System (BAS). 3 LED's are used in place of the 8 LED and WARNING/STOP
% setup. It is asumed that the sensor used consists of a 2 pin setup (trigger 
% pin and echo pin). 
%
% Author: Kayan Yazdani Farsad
%
% Date: 1/31/2021
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ON=1;
a = arduino();
OFF=0;
ON=1;
SpeedofSound=344;
% arduino instance and ON/OFF variables initialized. Speed of sound assumed
% to be 344 m/s.
 
arduinoObj = arduino('COM9','Uno','Libraries','Ultrasonic');
ultrasonicObj = ultrasonic(arduinoObj,'D2','D3','OutputFormat','double')
% Sensor code is set up assuming that the ultrasonic sensor has 2 signal pins
% and sound travels at 344 m/s. Trigger pin is D2 and Echo pin is D3. COM9
% may have to be changed to another port depending on the arduino used.


while ON==1
    %Loop will run forever as long as power runs to the arduino/BAS.
 
Time = readEchoTime(ultrasonicObj);
Distance= SpeedofSound*Time/2;
Speed= Distance/Time
% Time, Distance and speed variables calculated. Final Distance value is divided by 
% 2 as the echo measurement includes the distance from the onboard vehicle 
% to the trailing vehicle and back.
    
  
    
 if(Speed > 5 && Distance <= 6)

    
 writeDigitalPin(a, 'D19', ON);
 writeDigitalPin(a, 'D10', ON);
 writeDigitalPin(a, 'D11', ON);
% 3/3 LED's on for STOP branch. In the final build, 8/8 LED's would turn
% ON, along with the STOP message LED.

 else 
    if (Speed > 10 && Distance > 6)
        
writeDigitalPin(a, 'D9', ON);
writeDigitalPin(a, 'D10', OFF);
writeDigitalPin(a, 'D11', OFF);
% 1/3 LED's on for CAUTION branch. In the final build, 4/8 LED's would turn
% ON, along with the VAUTION message LED.

    else
writeDigitalPin(a,'D9', OFF);
writeDigitalPin(a, 'D10', OFF);
writeDigitalPin(a, 'D11', OFF);
% 0/3 LED's on for STANDBY branch.

    end
 end