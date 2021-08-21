# Universidad del Valle de Guatemala
# Electrónica Digital II
# Luis Pedro Molina
# Carné 18822
# Laboratorio No. 5 - Adafruit IO

# Adafruit IO
# https://io.adafruit.com/

from Adafruit_IO import Client, Feed

# Declaración de usuario y llave de Adafruit IO

ADAFRUIT_IO_USERNAME = "Luispedro00"
ADAFRUIT_IO_KEY      = "aio_kout971PO1w4VYhYjUzts1MHwJAX"
aio                  = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Conexión de PORTD

digital_feed = aio.feeds('contador-portd')      # Feed receptor de datos
aio.send_data(digital_feed.key, 78)             # Dato enviado --> 15
digital_data = aio.receive(digital_feed.key)    # Recepción de datos
print(f'digital signal: {digital_data.value}')  #

# Conexión de PORTA

digital_feed = aio.feeds('contador-porta')      # Feed receptor de datos
aio.send_data(digital_feed.key, 56)             # Dato enviado --> 18
digital_data = aio.receive(digital_feed.key)    # Recepción de datos
print(f'digital singal: {digital_data.value}')  #

send_feed = aio.feeds('send')                   #
send_data = aio.receive(send_feed.key)          #
print(f'send signal: {send_data.value}')        #



