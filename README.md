# Mobile power system for remote areas

## The project's purpose

* The purpose of this project is to study possible solutions for integrating electric vehicles into a wide range of utility operations (construction, agriculture, mining, etc.).

* The intention is to enable these vehicles to operate in remote or underdeveloped areas with limited energy infrastructure.

* This approach could highlight the advantages of implementing electric vehicles as a means to combat global warming.

* The project involved building and programming a model of a charging vehicle capable of carrying batteries, photovoltaic panels, and a charging port. This vehicle serves as a mobile power station, enabling the charging of another test vehicle model.

### Asamblarea bateriei

* I used three 3.7V lithium-ion cells to assemble a 12V battery pack.

* The battery pack was connected to the vehicle system through a BMS, enabling charging from the photovoltaic panels and power supply to the vehicle’s components.

<img width="645" height="402" alt="image" src="https://github.com/user-attachments/assets/e9646983-35a1-4cc7-8804-a6f2818496e7" />
<img width="724" height="569" alt="image" src="https://github.com/user-attachments/assets/ca2100ab-5960-4206-860f-aab99be8a98b" />


* Pentru controlul miscarii am utilizat o punte H L298N comandata prin intermediul semnalelor PWM.
* Pentru programarea vehiculului, am folosit o placa de dezvoltare Texas Instruments, cu un micro-controller MSP-EXP432P401R, programat în C.
  
<img width="674" height="501" alt="image" src="https://github.com/user-attachments/assets/b5e43e4a-448e-4b9d-a211-225762167861" />

* For the communication module I used a NRF24L01+ wireless module controlled by an Arduino board via SPH protocol. The resulted module was connected to the main control board   
<img width="530" height="614" alt="image" src="https://github.com/user-attachments/assets/c7b1137a-373e-4deb-80e0-4debf06c95ae" />

* For the controll station I used another NRF24L01+ wirelwss module connected to an Arduino Mega board where I hosted a small server. The server was accessible from the PC trough ethernet via an ENC28J60 ethernet module.
  
* The controls were sent via a user interface developed using HTML, CSS and JavaScript provided by a simple Python server responsible for communications between the user interface and the Arduino hosted server.
<img width="473" height="441" alt="image" src="https://github.com/user-attachments/assets/ca096445-46d7-4838-99dd-ec7987893a6c" />

### Vehiculul de încărcare
![WhatsApp Image 2025-10-07 at 01 06 16_923d4431](https://github.com/user-attachments/assets/4d7e7d23-a5a9-4fc7-97b5-e802a245a98e)

### Vehiculul de test
![WhatsApp Image 2025-10-07 at 01 10 33_b9dada8d](https://github.com/user-attachments/assets/706166ab-1139-43b9-9bf0-2e8f77d25b51)

### Încărcarea bateriei vehiculului de test, de la vehiculul de alimentare
![WhatsApp Image 2025-10-07 at 01 07 31_b3de424e](https://github.com/user-attachments/assets/7d719a23-547b-4d57-a0ef-accb63a7864d)











