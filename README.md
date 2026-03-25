# **NetPuck 5G: A Specialized 5GHz Wireless & Network Auditor**

# **0\. Introduction**

The NetPuck 5G is a handheld device, purpose-built to connect the worlds of software-driven network diagnostics and hardware-focused wireless analysis.

## **0.1 Why did I build this?**

* **I needed a network auditing device.** As a beginner IT Support Technician, I have a vast variety of tools for every job, but I've been lacking tools (and knowledge) regarding networking infrastructure. I've been making do with my phone and generic, no-brand WiFi strength scanning applications on my phone, but that is just not reliable for my purpose.  
* **I wanted to learn about hardware engineering.** I've always been interested in the “guts” of the devices around me. From a very young age, with a screwdriver in hand, I started taking devices that would otherwise be trashed apart, curious to see what makes them work. A few months ago, I also bought a soldering iron, and learned the skill of soldering and desoldering. In my curious journey of hardware engineering, the next step would be designing and having a custom PCB made.  
* **I wanted something to showcase in my portfolio.** Projects and contributions are largely important factors in one's portfolio, especially at an age where they cannot work, like me. I wanted to enrich my portfolio with personal projects, and Hack Club’s blueprint was the perfect, guided opportunity for me to do so.

## **0.2 What is it made of?**

It uses:

* the Waveshare ESP32-S3 Pico, as the main processing unit for the entire device  
* a BW16, solely for Access Point scanning, thanks to its 5GHz capabilities   
* a W5500 Ethernet module, for listing details about the network it is connected to  
* a 1.8’ inch LCD module, for providing all information gathered from the aforementioned boards to the user  
* a low-profile 5-way digital navigation joystick, for navigating the menus of the device  
* a MH-CD42 charging and boost-up module and two Samsung 18650 batteries, for powering the device from batteries, reliably.

## **0.3 So, what does it do?**

The NetPuck 5G has two functions; Wireless Access Point strength scanning, and Wired Network details.

* Wireless Access Point strength scanning  
  * This function uses the BW16, which communicates with the ESP32 via UART to list all 2.4GHz and 5GHz Access Points within reach, and the strength of each one, measured in dBm.  
* LAN details  
  * This function detects specific details about the network the W5500 module is plugged into. Specifically: ICMP Echo to 8.8.8.8, Gateway IP (from DHCP Lease), ICMP Echo to Gateway (Confirms local LAN integrity), LLDP (Passive sniff for Link Layer Discovery Protocol packets), Reverse DNS lookup of the Gateway’s Public IP, Gateway Domain Name (from DHCP Option 15), NLA Information, Gateway MAC (ARP-resolved)

# **1\. Pinout**

Grounds not listed; all grounds connect to each other  
The Pin/As/To/For tables are read as: The Pin \[Pin\] functions as/is labeled as \[As\], and goes to pin \[To\], (and is used) for \[For\].   
 

## **1.1 ESP32-S3**

![](https://github.com/leonampa/netpuck/blob/5f600c0f0242c0ecc39c4f41bfe9c777f7e87631/Media/1.1_PINOUT.jpg)

| Pin | As | To | For |
| :---- | :---- | :---- | :---- |
| Vbus/40 | \+5V | MH-CD42 @ 5V OUT | Powering the ESP |
| GP5/27 | LCD\_BL | LCD @ BL | Controlling the backlight |
| GP6/29 | W5500\_RST | W5500 @ RST | Resetting the W5500 |
| GP7/31 | LCD\_RST | LCD @ RST | LCD’s Resetting the LCD |
| GP8/32 | LCD\_DC | LCD @ DC | Data/Command |
| GP9/33 | W5500\_CS | W5500 @ CS | W5500’s SPI Chip Select  |
| GP10/34 | LCD\_CS | LCD @ CS | LCD’s SPI Chip Select |
| 3V3/36 | 3V3 | LCD @ VCC, W5500 @ V | Powering the LCD and W5500 |
| GP11/1 | SPI\_MOSI | LCD @ DIN, W5500 @ MO | Transferring data via SPI from the ESP32 (Master), to the LCD and W5500 (Slaves) |
| GP12/2 | SPI\_SCK | LCD @ CLK, W5500 @ SCK | Serial Clock for SPI |
| GP13/4 | SPI\_MISO | W5500 @ MI | Transferring data via SPI from the W5500 (Slave) to the ESP32 (Master) |
| GP14/5 | BW16\_EN | BW16 @ EN | BW16’s Enable Pin |
| GP17/9 | BW16\_RX | BW16 @ 7 | UART (ESP32 → BW16) |
| GP18/10 | BW16\_TX | BW16 @ 8 | UART (ESP32 ← BW16) |
| GP34/12 | MID | STICK @ MID | Joystick’s Click |
| GP35/14 | UP | STICK @ UP | Joystick’s Up |
| GP36/15 | DOWN | STICK @ DOWN | Joystick’s Down |
| GP37/16 | LEFT | STICK @ LEFT | Joystick’s Left |
| GP38/17 | RIGHT | STICK @ RIGHT | Joystick’s Right |

## **1.2 BW16**

![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/1.2_PINOUT.webp)

| Pin | As | To | For |
| :---- | :---- | :---- | :---- |
| 5V | \+5V | MH-CD42 @ 5V OUT | Powering the BW16 |
| PA7/TX | BW16\_TX | ESP @ GP17/9 | UART (ESP32 ← BW16) |
| PA8/RX | BW16\_RX | ESP @ GP18/10 | UART (ESP32 → BW16) |
| EN | BW16\_EN | ESP @ GP14/5 | Enable Pin |

## **1.3 W5500**

![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/1.3_PINOUT.jpg)

| Pin | As | To | For |
| :---- | :---- | :---- | :---- |
| RST | W5500\_RST | ESP @ GP6/29 | Resetting the W5500 |
| CS | W5500\_CS | ESP @ GP9/34 | W5500’s SPI Chip Select  |
| V | 3V3 | ESP @ 3V3 | Powering the LCD and W5500 |
| SCK | SPI\_SCK | ESP @ GP12/2 | Serial Clock for SPI |
| MI | SPI\_MISO | ESP @ GP13/4 | Transferring data via SPI from the W5500 (Slave) to the ESP32 (Master) |
| MO | SPI\_MOSI | ESP @ GP11/1 | Transferring data via SPI from the ESP32 (Master), to theW5500 (Slave) |

## **1.4 LCD**

![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/1.4_PINOUT.jpg)

| Pin | As | To | For |
| :---- | :---- | :---- | :---- |
| BL  | LCD\_BL | ESP @ GP5/27 | Controlling the backlight |
| RST  | LCD\_RST | ESP @ GP7/31 | Resetting the LCD |
| DC  | LCD\_DC | ESP @ GP8/32 | LCD’s Data/Command |
| CS  | LCD\_CS | ESP @ GP10/34 | LCD’s SPI Chip Select |
| VCC  | 3V3 | ESP @ 3V3/36 | Powering the LCD |
| DIN  | SPI\_MOSI | ESP @ GP11/1 | Transferring data via SPI from the ESP32 (Master), to the LCD (Slave) |
| CLK  | SPI\_SCK | ESP @ GP12/2 | Serial Clock for SPI |

## **1.5 Joystick** 

![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/1.5_PINOUT.jpg)

| Pin | As | To | For |
| :---- | :---- | :---- | :---- |
| MID | MID | ESP @ GP34/12 | Joystick’s Click |
| UP | UP | ESP @ GP35/14 | Joystick’s Up |
| DOWN | DOWN | ESP @ GP36/15 | Joystick’s Down |
| LEFT | LEFT | ESP @ GP37/16 | Joystick’s Left |
| RIGHT | RIGHT | ESP @ GP38/17 | Joystick’s Right |

## **1.6 MH-CD42 Charging Module**

![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/1.6_PINOUT.jpg)

| Pin | As | To | For |
| :---- | :---- | :---- | :---- |
| 5V OUT | \+5V | ESP @ Vbus/40, BW16 @ 5V | Powering the ESP and BW16 |
| VIN  | VUSB | USB @ V | Powering the MH-CD42 |
| BAT | VBAT | (external batteries) | Powering the MH-CD42 |

## **1.7 USB Port**

| Pin | As | To | For |
| :---- | :---- | :---- | :---- |
| V | VUSB | VIN | Powering the MH-CD42 |

# **2\. PCB**

I used KiCad to design the PCB. Through a few struggles (as described on Journal), I landed on this design:

![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/2_front.png)
![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/2_back.png)

# **3\. Case**

I used Autodesk Fusion to make the case (as described on Journal). The enclosure is designed for field durability, featuring a 'Sandwich Mount' architecture. It utilizes M3 heat-set inserts and M3 Nylon screws, instead of Stainless Steel, to avoid signal interference.

![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/3_1.png)
![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/3_2.png)
![](https://github.com/leonampa/netpuck/blob/b12948caa2debe75fde5efd32e5c7002bff795d0/Media/3_3.png)

# **4\. Bill Of Materials**

| Item | €/pcs | Qty | € Total | Link |
| :---- | ----- | ----- | ----- | :---- |
| **GRobotronics** |  |  |  |  |
| [eSUN PETG+HS 1.75mm 1kg \- Solid Black](https://grobotronics.com/esun-petghs-1-75mm-1kg-solid-black.html) | €19.90 | 1 | €19.90 | [https://grobotronics.com/esun-petghs-1-75mm-1kg-solid-black.html](https://grobotronics.com/esun-petghs-1-75mm-1kg-solid-black.html) |
| [Soldering Wire Brofil 60 100g 1mm](https://grobotronics.com/soldering-wire-brofil-60-100g-1mm.html) | €7.20 | 1 | €7.20 | [https://grobotronics.com/soldering-wire-brofil-60-100g-1mm.html](https://grobotronics.com/soldering-wire-brofil-60-100g-1mm.html) |
| [Waveshare ESP32-S3 Pico With Pre-soldered Headers](https://grobotronics.com/waveshare-esp32-s3-development-board.html?variation_id=18355) | €13.60 | 1 | €13.60 | [https://grobotronics.com/waveshare-esp32-s3-development-board.html?variation\_id=18355](https://grobotronics.com/waveshare-esp32-s3-development-board.html?variation_id=18355) |
| [Battery Lithium 18650 3.6V 3400mAh \- Samsung INR18650-35E](https://grobotronics.com/battery-lithium-18650-3.6v-3400mah-8a-samsung-inr18650-35e.html) | €5.80 | 2 | €11.60 | [https://grobotronics.com/battery-lithium-18650-3.6v-3400mah-8a-samsung-inr18650-35e.html](https://grobotronics.com/battery-lithium-18650-3.6v-3400mah-8a-samsung-inr18650-35e.html) |
| [Battery Holder 2x18650 \- with Wires & Switch](https://grobotronics.com/battery-holder-2x18650-with-wires-and-switchoem.html) | €1.20 | 1 | €1.20 | [https://grobotronics.com/battery-lithium-18650-3.6v-3400mah-8a-samsung-inr18650-35e.html](https://grobotronics.com/battery-lithium-18650-3.6v-3400mah-8a-samsung-inr18650-35e.html) |
| [Joystick Module 5-Way](https://grobotronics.com/5-way-joystick-module.html) | €2.60 | 1 | €2.60 | [https://grobotronics.com/5-way-joystick-module.html](https://grobotronics.com/5-way-joystick-module.html) |
| [Waveshare LCD 1.8" 128×160 SPI](https://grobotronics.com/display-1.8-128x160-lcd-module.html) | €11.20 | 1 | €11.20 | [https://grobotronics.com/display-1.8-128x160-lcd-module.html](https://grobotronics.com/display-1.8-128x160-lcd-module.html) |
| [USB Type-C Breakout Board \- Straight Header](https://grobotronics.com/usb-type-c-breakout-board-straight-header.html) | €0.80 | 1 | €0.80 | [https://grobotronics.com/usb-type-c-breakout-board-straight-header.html](https://grobotronics.com/usb-type-c-breakout-board-straight-header.html) |
| [Pin Header 1x40 Male 2.54 mm Black](https://grobotronics.com/pin-header-1x40-male-2.54-mm-black.html) | €0.10 | 1 | €0.10 | [https://grobotronics.com/pin-header-1x40-male-2.54-mm-black.html](https://grobotronics.com/pin-header-1x40-male-2.54-mm-black.html) |
| [Pin Header 1x10 Female 2.54mm](https://grobotronics.com/pin-header-1x10-female-2.54mm.html) | €0.17 | 4 | €0.68 | [https://grobotronics.com/pin-header-1x10-female-2.54mm.html](https://grobotronics.com/pin-header-1x10-female-2.54mm.html) |
| [Pin Header 1x8 Female 2.54mm](https://grobotronics.com/pin-header-1x8-female-2.54mm.html) | €0.10 | 3 | €0.30 | [https://grobotronics.com/pin-header-1x8-female-2.54mm.html](https://grobotronics.com/pin-header-1x8-female-2.54mm.html) |
| [Arduino Long Header 8pins](https://grobotronics.com/arduino-long-header-8pins.html) | €0.25 | 1 | €0.25 | [https://grobotronics.com/arduino-long-header-8pins.html](https://grobotronics.com/arduino-long-header-8pins.html) |
| [Pin Header Female 1x6 2.54mm](https://grobotronics.com/pin-header-1x6-female-2.54mm.html) | €0.08 | 3 | €0.24 | [https://grobotronics.com/pin-header-1x6-female-2.54mm.html](https://grobotronics.com/pin-header-1x6-female-2.54mm.html) |
| [Pin Header 1x4 Long Stackable](https://grobotronics.com/pin-header-1x4-long-stackable.html) | €0.20 | 1 | €0.20 | [https://grobotronics.com/pin-header-1x4-long-stackable.html](https://grobotronics.com/pin-header-1x4-long-stackable.html) |
| [Threaded Insert M3 \- 5.7x4.6mm](https://grobotronics.com/threaded-insert-m3-5-7x4-6mm.html) | €0.15 | 10 | €1.50 | [https://grobotronics.com/threaded-insert-m3-short-3x4-6.html](https://grobotronics.com/threaded-insert-m3-short-3x4-6.html) |
| [Bolt M3 \- Nylon (20mm)](https://grobotronics.com/m3.html?variation_id=2943) | €0.30 | 10 | €3.00 | [https://grobotronics.com/m3.html?variation\_id=12363](https://grobotronics.com/m3.html?variation_id=12363) |
| [High Temperature Adhesive Tape 50mm \- 33m](https://grobotronics.com/high-temperature-adhesive-tape-50mm-33m.html) | €7.90 | 1 | €7.90 | [https://grobotronics.com/high-temperature-adhesive-tape-50mm-33m.html](https://grobotronics.com/high-temperature-adhesive-tape-50mm-33m.html) |
| [YATO YT-2081 Side Cutting Pliers 115mm](https://grobotronics.com/yato-cutting-pliers-115mm.html) | €3.60 | 1 | €3.60 | [https://grobotronics.com/yato-cutting-pliers-115mm.html](https://grobotronics.com/yato-cutting-pliers-115mm.html) |
| Shipping (BOXNOW) | €0.00 | 0 | €0.00 |  |
| **Subtotal** |  | 44 | €85.87 |  |
|  |  |  |  |  |
| **AliExpress** |  |  |  |  |
| [Dual-band WiFi \+ Bluetooth 5.0 IoT Core Board (BW16 RTL8720DN) Suitable for Arduino and Smart Home Projects (IPEX Antenna)](https://www.aliexpress.com/item/1005010064459718.html?spm=a2g0o.detail.0.0.7236U0QJU0QJSg&mp=1&pdp_npi=6%40dis%21EUR%21EUR%209.55%21EUR%201.95%21%21EUR%201.95%21%21%21%402103835e17741877141951141e5be7%2112000051007214201%21ct%21GR%21-1%21%211%210%21) | €7.24 | 1 | €7.24 | [https://www.aliexpress.com/item/1005010064459718.html?spm=a2g0o.detail.0.0.7236U0QJU0QJSg\&mp=1\&pdp\_npi=6%40dis%21EUR%21EUR%209.55%21EUR%201.95%21%21EUR%201.95%21%21%21%402103835e17741877141951141e5be7%2112000051007214201%21ct%21GR%21-1%21%211%210%21](https://www.aliexpress.com/item/1005010064459718.html?spm=a2g0o.detail.0.0.7236U0QJU0QJSg&mp=1&pdp_npi=6%40dis%21EUR%21EUR%209.55%21EUR%201.95%21%21EUR%201.95%21%21%21%402103835e17741877141951141e5be7%2112000051007214201%21ct%21GR%21-1%21%211%210%21) |
| [1PCS W5500 Module TCP/IP Ethernet Module Compatible with WIZ820io Network Interface RC5 IoT for Arduino](https://www.aliexpress.com/item/1005004679288698.html?spm=a2g0o.detail.0.0.7236U0QJU0QJSg&mp=1&pdp_npi=6%40dis%21EUR%21EUR%204.54%21EUR%200.88%21%21EUR%200.88%21%21%21%402103835e17741877141951141e5be7%2112000030074096066%21ct%21GR%21-1%21%211%210%21) | €3.91 | 1 | €3.91 | [https://www.aliexpress.com/item/1005004679288698.html?spm=a2g0o.detail.0.0.7236U0QJU0QJSg\&mp=1\&pdp\_npi=6%40dis%21EUR%21EUR%204.54%21EUR%200.88%21%21EUR%200.88%21%21%21%402103835e17741877141951141e5be7%2112000030074096066%21ct%21GR%21-1%21%211%210%21](https://www.aliexpress.com/item/1005004679288698.html?spm=a2g0o.detail.0.0.7236U0QJU0QJSg&mp=1&pdp_npi=6%40dis%21EUR%21EUR%204.54%21EUR%200.88%21%21EUR%200.88%21%21%21%402103835e17741877141951141e5be7%2112000030074096066%21ct%21GR%21-1%21%211%210%21) |
| [5/10pcs DC 5V 2A Mobile Power Diy Board 4.2V Charge/Discharge(boost)/battery protection/indicator module 3.7V lithium 18650](https://www.aliexpress.com/item/1005007432099008.html?spm=a2g0o.productlist.main.1.6d655d0d201MlN&algo_pvid=0b38d990-1f78-4ed4-ab89-ce53223c6fe6&algo_exp_id=0b38d990-1f78-4ed4-ab89-ce53223c6fe6-8&pdp_ext_f=%7B%22order%22%3A%22861%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%213.13%212.71%21%21%213.53%213.06%21%4021039a5b17741899750876391e3fa0%2112000040730748938%21sea%21GR%216308147579%21X%211%210%21n_tag%3A-29919%3Bd%3A73d1907e%3Bm03_new_user%3A-29895%3BpisId%3A5000000202472607&curPageLogUid=w2UUTZk6Qdsr&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005007432099008%7C_p_origin_prod%3A) (MH-CD42) | €2.71 | 1 | €2.71 | [https://www.aliexpress.com/item/1005007432099008.html?spm=a2g0o.productlist.main.1.6d655d0d201MlN\&algo\_pvid=0b38d990-1f78-4ed4-ab89-ce53223c6fe6\&algo\_exp\_id=0b38d990-1f78-4ed4-ab89-ce53223c6fe6-8\&pdp\_ext\_f=%7B%22order%22%3A%22861%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D\&pdp\_npi=6%40dis%21EUR%213.13%212.71%21%21%213.53%213.06%21%4021039a5b17741899750876391e3fa0%2112000040730748938%21sea%21GR%216308147579%21X%211%210%21n\_tag%3A-29919%3Bd%3A73d1907e%3Bm03\_new\_user%3A-29895%3BpisId%3A5000000202472607\&curPageLogUid=w2UUTZk6Qdsr\&utparam-url=scene%3Asearch%7Cquery\_from%3A%7Cx\_object\_id%3A1005007432099008%7C\_p\_origin\_prod%3A](https://www.aliexpress.com/item/1005007432099008.html?spm=a2g0o.productlist.main.1.6d655d0d201MlN&algo_pvid=0b38d990-1f78-4ed4-ab89-ce53223c6fe6&algo_exp_id=0b38d990-1f78-4ed4-ab89-ce53223c6fe6-8&pdp_ext_f=%7B%22order%22%3A%22861%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%213.13%212.71%21%21%213.53%213.06%21%4021039a5b17741899750876391e3fa0%2112000040730748938%21sea%21GR%216308147579%21X%211%210%21n_tag%3A-29919%3Bd%3A73d1907e%3Bm03_new_user%3A-29895%3BpisId%3A5000000202472607&curPageLogUid=w2UUTZk6Qdsr&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005007432099008%7C_p_origin_prod%3A) |
| Shipping (AliExpress Choice) | €0.00 | 0 | €0.00 |  |
| **Subtotal** |  | 3 | €13.86 |  |
|  |  |  |  |  |
| **JLCPCB** |  |  |  |  |
| PCB (2 layers) | €7.39 | 1 | €7.39 |  |
| Shipping (Global Standard Direct Line) | €7.71 | 1 | €7.71 |  |
|  |  |  | €15.10 |  |
|  |  |  |  |  |
| **TOTAL** |  |  | €114.83 |  |
| **Conversion to USD (rate from Google Finance, €1 \= \_$)** | 1.1562 |  | $132.76 |  |

