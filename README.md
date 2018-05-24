# Mini Shell
**Autores: Omar Espinel, Valentina Yate.**

Escriba los comandos de la bash seguidos de el '>' recuerde usar espacios entre los comandos.

## Sobre el comando BG:
BG ejecutará procesos en segundo plano, escriba "BG" seguido del comando que desea ejecutar.

Ejemplo:

_El comando **whereis** se ejecutará en segundo plano_
>BG whereis ssh


## Sobre el comando REDIR:
REDIR redirecciona comandos a archivos o viceversa.

Ejemplo:

_La salida de **ls** no se mostrará por pantalla, se guardará en el archivo **ls.out**_
>REDIR ls -l OUTPUTFILE=ls.out


## Sobre las pipes:
Las pipes redireccionarán las salidas de comandos a otros. 

Ejemplo:

_El comando **echo** no imprimirá por pantalla sus argumentos. Los desvía al comando **wc**, el cual contará las letras de la salida anterior._ 
>echo SoyElMejorProyecto | wc -m



