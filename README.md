# ServoEjercito

Los robots se han diseñado para hacerlos lo más pequeño posible utilizando elementos electrónicos básicos soldados sobre una placa de baquelita.
Una de las principales características es que conseguimos desplazar el robot y girar utilizando solo dos servomotores (sin elementos rodantes), el servomotor trasero inclina el robot hacia un lado, mientras que el de delante lo impulsa. Para lograr ésto se le programan dos movimientos oscilatorios que llevan un desfase de 90º de forma que el movimiento resultante permite el desplazamiento.
El robot incorpora un sensor óptico reflectante de infrarrojos (un receptor se encarga de ver si se refleja la luz infrarroja emitida), un zumbador que emite pitidos, y un led.
El cerebro del robot es un pequeño microcontrolador Attiny85 que tiene solo 5 puertos de entrada / salida. 
![](https://github.com/ElHormiguero/ServoEjercito/blob/master/Images/ejercitoservos.jpg)
