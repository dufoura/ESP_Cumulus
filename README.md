# ESP_Cumulus
Module permettant de réguler la programmation de votre cumulus d'eau chaude

## Liste des composants
Pour construire cette carte électronique, vous aurez besoin de souder: 
- 1 ESP8266
- 1 AMS117 3.3V
- 1 condensateur chimique ou tantale de 10µF
- Des pins pour la connection
- 1 relais
- 1 transistor mosfet 2N7000
- 1 diode de roue-libre
- 1 connecteur usb femelle
- 4 Résistances 10kOhms CMS

## Liste des modules
Pour faire fonctionner votre module, il vous faudra acheter ces cartes : 
- 1 module RTC DS1307 (avec 1 pile)

## Mise en route
1. Il faut brancher votre module RTC à la carte avec la liaison suivante : 
Carte ESP <-> RTC

3.3V <-> 3.3V
Gnd <-> Gnd
GPIO5 <-> SCL
GPIO4 <-> SDA

2. Brancher l'alimentation, à l'aide d'un câble USB
3. Avec votre PC, attendez que l'ESP affiche un Wifi appelé 'WifiESP'.
4. Connectez-vous et rentrez le mot de passe.
5. Avec un navigateur, tapez dans la barre url : 192.168.4.1/edit
6. Vous pouvez alors programmer les heures de marche et d'arrêt du relais, pour votre cumulus. Vous pouvez également connecter votre ESP à votre Wifi domestique. 