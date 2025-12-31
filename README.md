⚙️ Required Components

**Hardware:**
- 2 × Arduino Nano
- 2 × DHT11 Temperature & Humidity Sensor
- 2 × NRF24L01 Transceiver Module (with antennas for better range)
- 1 × DS3231 Real Time Clock Module
- 1 × CR2032 battery (for DS3231 backup)
- 1 × 0.96" OLED Display (SSD1306, 128×64, I2C)
- 1 × 10 µF electrolytic capacitor
- 1 × 4.7 kΩ resistor (pull-up for DHT11)
- Jumper wires / breadboard or PCB for connections
- Power supplies:
  - 12 V DC adapter (for indoor/receiver unit)
  - 2 × 3.7 V Li-ion batteries (e.g., 18650) + holder (~7.5 V total for outdoor/transmitter unit)

**Software & Tools:**
- Arduino IDE (latest version recommended)
- Mini-USB cables (for programming both Arduino Nanos)

🛠 Working:
The outdoor transmitter periodically reads temperature and humidity from its DHT11 sensor and sends the data wirelessly via NRF24L01. The indoor receiver reads its local DHT11, retrieves the current time from the DS3231 RTC, receives remote data, and displays everything on the OLED. A 5-second timeout detects lost connections and shows "Not Detected". Low-power delays in the transmitter extend battery life.

🚀 Steps
1. **Wiring the Circuits**  
   Build according to the build photo

2. **Installing Libraries**  
   In Arduino IDE → Sketch → Include Library → Manage Libraries, install:  
   - RF24 (by TMRh20)  
   - DHT sensor library (by Adafruit)  
   - Adafruit GFX  
   - Adafruit SSD1306  
   - RTClib (by Adafruit)

3. **Uploading Code**  
   - Open `receiver.ino` and upload to the indoor Arduino Nano  
   - Open `transmitter.ino` and upload to the outdoor Arduino Nano  
   - Optional: Set initial time in receiver code with `rtc.adjust(DateTime(YYYY, MM, DD, HH, MM, SS));` (comment out after first upload)

4. **Powering the Units**  
   - Indoor/receiver: 12 V DC adapter  
   - Outdoor/transmitter: 2 × Li-ion batteries (~7.5 V total)

The display updates every second with time, local readings, and remote readings (or "Not Detected" if signal lost).

🔧 Tips & Troubleshooting
- **NRF24L01 unstable?** Always use the 10 µF capacitor across VCC/GND for stable power.
- **DHT11 inaccurate?** Use the 4.7 kΩ pull-up resistor. The code applies a -2 °C offset on display—adjust if needed.
- **No remote data?** Check power on both units, distance, interference, or wiring (especially antennas on NRF24L01).
- **Wrong time on RTC?** Set it once via the `rtc.adjust()` line, upload, then comment it out.

📚 Applications
- Personal home indoor/outdoor weather monitoring
- Gardening and plant care (track humidity/temperature)
- Educational projects (learn sensors, wireless communication, displays)
- Agriculture/greenhouse automation
- Environmental data logging in remote locations

📝 Author
**Saptarshi Das**

📜 License
This project is licensed under the MIT License - see the 
