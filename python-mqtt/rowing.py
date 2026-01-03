# python 3.11

import random
import json
import time
from paho.mqtt import client as mqtt_client # pyright: ignore[reportMissingImports]
import socket
import sys
import random
from math import ceil
  
UDP_IP = "127.0.0.1"
UDP_PORT = 10110
#MESSAGE = "$GPVTG,44.0,T,,M,21.7,N, 1.3,K*46"
KNOTS_TO_KMHR_CONVERSION: float = 1.852
CHECKSUM_HEX_LENGTH: int = 2

broker = '192.168.1.165' # Edit for ip address of pc
port = 1883
topic = "OpenRowingMonitor/opencpn/metrics"
# Generate a Client ID with the subscribe prefix.
client_id = f'subscribe-{random.randint(0, 100)}'

# Edit stroke_length for actual stroke length from machine
stroke_length = 1
# stroke_length

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    #client = mqtt_client.Client(client_id)
    client = mqtt_client.Client(mqtt_client.CallbackAPIVersion.VERSION1, client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def subscribe(client: mqtt_client):
    
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        json_data = json.loads(msg.payload.decode())
        td = json_data.pop('strokeRate')

        global my_message  
        global enc_str

        try:
            if td is None: # The variable
                print('It is None')
                return
        except NameError:
            print ("This variable is not defined")
        else:
            spd_kts = (float(td) * 60 * stroke_length) / 1852
            #print(spd_kts)
            kmhr = round(spd_kts * KNOTS_TO_KMHR_CONVERSION, 1)
            my_message = Gpvtg(heading_true=0.0, sog_knots=spd_kts, sog_kmhr= kmhr) 
            enc_str = str(my_message).encode()
            print(enc_str)

            udp(UDP_IP, UDP_PORT, 30, 3, 5)       
        
    client.subscribe(topic)
    client.on_message = on_message
    
def udp(Dest, Port, Delay, Repeat, Speed):
    
    if Dest is None:
        Dest = socket.gethostbyname(socket.gethostname())
    
    try:
               
        sock = socket.socket(socket.AF_INET,    # Internet
                            socket.SOCK_DGRAM)  # UDP
        # Allow UDP broadcast
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        # Add socket reuse for better cross-platform compatibility
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                 
        # Send message to client with reasonable
        # number of retries before giving up.
        try:
            sock.sendto(enc_str, (Dest, Port))
            consecutive_errors = 0  # Reset error counter on success
        except socket.error as e:
            consecutive_errors += 1
            print(f"\nSocket error: {e} (attempt {consecutive_errors})")
            if consecutive_errors >= 5:
                print("Too many consecutive socket errors, exiting...")
                return False
            # Brief pause before retry
            time.sleep(0.1)
                      
    except KeyboardInterrupt:
        print("\nKeyboardInterrupt.")
        return True
    # End except KeyboardInterrupt

    except Exception as ex:
        print("Exception...")
        print(ex)
        raise ex
    # End except Exception

    finally:
        if sock:
            sock.close()        
        #End if
    # End try
# End udp()   


class Gpvtg :
    """Track Made Good and Ground Speed.

    Provides course and speed information including true and magnetic headings
    and speed over ground in both knots and kilometers per hour.

    Example: $GPVTG,360.0,T,348.7,M,000.0,N,000.0,K*43
    """

    sentence_id: str = "GPVTG"
    
    def __init__(
        self,
        heading_true: float,
        sog_knots: float,
        sog_kmhr: float,
        heading_magnetic: float | str = "",        
    ) -> None:
        """Initialize GPVTG sentence with heading and speed data.

        Args:
            heading_true: True heading in degrees
            sog_knots: Speed over ground in knots
            heading_magnetic: Magnetic heading in degrees (empty if not available)

        Returns:
            None

        """
        self.heading_true: float = heading_true
        self.heading_magnetic: float | str = heading_magnetic
        self.sog_knots: float = sog_knots
        self.sog_kmhr: float  = sog_kmhr

    def __str__(self) -> str:
        """Return formatted GPVTG sentence string.

        Returns:
            Complete GPVTG NMEA sentence with checksum and line terminators

        """
        nmea_output = (
            f"{self.sentence_id},{self.heading_true},T,{self.heading_magnetic},M,{self.sog_knots},N, {self.sog_kmhr},K"
        )

        enc = f"${nmea_output}*{self.check_sum(nmea_output)}\r\n"

        return enc
    
    @staticmethod
    def check_sum(data: str):
        """Calculate NMEA checksum for given data string.

        Performs XOR operation on all bytes between $ and * delimiters
        and returns checksum in hexadecimal notation.

        Args:
            data: NMEA sentence data (without $ prefix and * suffix)

        Returns:
            Two-character uppercase hexadecimal checksum string

        """
        check_sum: int = 0
        for char in data:
            num = bytearray(char, encoding="utf-8")[0]
            # XOR operation.
            check_sum = check_sum ^ num
        # Returns only hex digits string without leading 0x.
        hex_str: str = str(hex(check_sum))[CHECKSUM_HEX_LENGTH:]
        if len(hex_str) == CHECKSUM_HEX_LENGTH:
            return hex_str.upper()
        return f"0{hex_str}".upper()

def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()    

if __name__ == '__main__':
    run()