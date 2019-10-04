# RPi
import time 
import paho.mqtt.client as mqtt 


flag=0
# Setup callback functions that are called when MQTT events happen like 
# connecting to the server or receiving data from a subscribed feed. 
def on_connect(client, userdata, flags, rc): 
   print("Connected with result code " + str(rc)) 
   # Subscribing in on_connect() means that if we lose the connection and 
   # reconnect then subscriptions will be renewed. 
   client.subscribe("/leds/pi") 




# The callback for when a PUBLISH message is received from the server. 
def on_message(client, userdata, msg):
    global flag
    flag=1 
    print(msg.topic+" "+str( msg.payload))
    # Check if this is a message for the Pi LED. 
    if msg.topic == '/leds/pi': 
       # Look at the message data and perform the appropriate action.
        print("thats for pi? thanks\n")
    flag=0




# Create MQTT client and connect to localhost, i.e. the Raspberry Pi running 
# this script and the MQTT server. 
client = mqtt.Client() 
client.on_connect = on_connect 
client.on_message = on_message 
client.connect('localhost', 1883, 60) 
# Connect to the MQTT server and process messages in a background thread. 
client.loop_start() 
# Main loop to listen for button presses. 
print('Script is running, press Ctrl-C to quit...') 
while True: 
   # Look for a change from high to low value on the button input to 
   time.sleep(2)
   client.publish('/leds/esp8266', 'TOGGLE') 
