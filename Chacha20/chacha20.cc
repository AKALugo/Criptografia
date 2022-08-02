#include <iostream>
#include <vector>

#include <math.h>

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) ( \
  a += b, d ^= a, d = ROTL(d, 16), \
  c += d, b ^= c, b = ROTL(b, 12), \
  a += b, d ^= a, d = ROTL(d, 8),  \
  c += d, b ^= c, b = ROTL(b, 7))




// Método que calcula una clave de forma aleatoria
std::vector<u_int32_t>
Aleatorio (unsigned cantidad){

    srand(time(NULL));
    std::vector<u_int32_t> vector;
    vector.resize(cantidad);
    for (unsigned recorrido = 0; recorrido < vector.size(); recorrido++)
        vector[recorrido] = rand()%256; // Valores entre 0 y 255

    return vector;
}



// Método para pasar a Hexadecimal.
void
PasarHexadecimal(uint32_t numero) {

    std::string hexa;

    // Dividimos el numero para pasarlo a hexa
    while (numero != 0) {
      switch (numero % 16)
      {
      case 15:
        hexa += 'f';
        break;
      case 14:
        hexa += 'e';
        break;
      case 13:
        hexa += 'd';
        break;
      case 12:
        hexa += 'c';
        break;
      case 11:
        hexa += 'b';
        break;
      case 10:
        hexa += 'a';
        break;                                        
      
      default:
        hexa += numero % 16 + '0';
        break;
      }
       numero /= 16;
    }
    // Rellenamos hasta que sean 8 valores.
    while (hexa.size() != 8)
        hexa += "0";

    // Mostramos por pantalla.
    for (int recorrido = hexa.size() - 1; recorrido >= 0; recorrido--)
        std::cout << hexa[recorrido];

    std::cout << "   ";
}



// Método para pasar a binario.
void
PasarBinario(uint32_t numero) {

    std::string binario;

    // Dividimos el numero para pasarlo a binario
    while (numero != 0) {
       binario += (numero % 2 == 0 ? "0" : "1");
       numero /= 2;
    }
    // Rellenamos hasta que sean 8 valores.
    while (binario.size() != 8)
        binario += "0";

    // Mostramos por pantalla.
    for (int recorrido = binario.size() - 1; recorrido >= 0; recorrido--)
        std::cout << binario[recorrido];
}



// Método para pasar una cantidad de numeros en hexadecimal que se encuentran en un string 
// a un valor numerico decimal.
void
SacarNumeroHexa (std::string& aux_str, std::vector<uint32_t>& numeros) {
    
    uint32_t pivote;

    pivote = 0;

    // Recorremos el str y con potencias de 16 vamos pasando posicion a posicion a un valor
    // numérico mientras vamos sumando todos esos valores para dar con el número finalmente.
    for (int desp_aux = aux_str.size() - 1; desp_aux > 0; desp_aux -= 2) {
      switch (aux_str[desp_aux])
      {
      case 'a':
        pivote += 10 * pow(16, desp_aux - 1);
        break;
      case 'b':
        pivote += 11  * pow(16, desp_aux - 1);
        break;
      case 'c':
        pivote += 12 * pow(16, desp_aux - 1);
        break;
      case 'd':
        pivote += 13 * pow(16, desp_aux - 1);
        break;
      case 'e':
        pivote += 14 * pow(16, desp_aux - 1);
        break;
      case 'f':
        pivote += 15 * pow(16, desp_aux - 1);
        break;
      
      default:
        pivote += (aux_str[desp_aux] - '0') * pow(16, desp_aux - 1);
        break;
      }

      switch (aux_str[desp_aux -1])
      {
      case 'a':
        pivote += 10 * pow(16, desp_aux);
        break;
      case 'b':
        pivote += 11 * pow(16, desp_aux);
        break;
      case 'c':
        pivote += 12 * pow(16, desp_aux);
        break;
      case 'd':
        pivote += 13 * pow(16, desp_aux);
        break;
      case 'e':
        pivote += 14 * pow(16, desp_aux);
        break;
      case 'f':
        pivote += 15 * pow(16, desp_aux);
        break;
      
      default:
        pivote += (aux_str[desp_aux - 1] - '0') * pow(16, desp_aux);
        break;
      }
    }
    numeros.push_back(pivote);
    aux_str.clear(); 
}



// Método para leer de teclado números sin importar el formato de entrada, devuelve un vector
// con los números.
std::vector<uint32_t>
TextoNumero (std::string cadena) {

    std::vector<uint32_t> numeros;
    std::string aux_str;
    unsigned contador = 0;

    // Recorremos el vector que contiene los números y otros caracteres.
    for (unsigned desp = 0; desp < cadena.size(); desp++) {
        
        // Si es un digito hacemos push_back en aux_str y cuando no sea un numero y sea ':'
        // y llevemos 4 tendremos nuestra primera palabra de 32 bits.
        if (isdigit(cadena[desp]) || cadena[desp] == 'a' || cadena[desp] == 'b' || cadena[desp] == 'c' 
                                  || cadena[desp] == 'd' || cadena[desp] == 'e' || cadena[desp] == 'f') 
          aux_str += cadena[desp];
        else if (cadena[desp] == ':') contador++;

        if (contador == 4) { 
          SacarNumeroHexa(aux_str, numeros);
          contador = 0;
        } 
    }
    // Cuando acabemos de revisar el str hay que mirar que en aux_str no quede nada, si queda algo
    // es que la cadena que entró por teclado acababa con un número que aún tenemos que pasar de texto
    // a decimal.
    if (aux_str.size() > 0) SacarNumeroHexa(aux_str, numeros);

    return numeros;
}



void ChachaBlock (std::vector<uint32_t>& estadoInicial512, std::vector<uint32_t>& 
                  estadoFinal512, std::vector<uint32_t>& salidaFinal512) {

  for (auto iteraciones = 0; iteraciones < 20; iteraciones += 2) {
    
    QR(estadoFinal512[0], estadoFinal512[4], estadoFinal512[8], estadoFinal512[12]);
    QR(estadoFinal512[1], estadoFinal512[5], estadoFinal512[9], estadoFinal512[13]);
    QR(estadoFinal512[2], estadoFinal512[6], estadoFinal512[10], estadoFinal512[14]);
    QR(estadoFinal512[3], estadoFinal512[7], estadoFinal512[11], estadoFinal512[15]);

    QR(estadoFinal512[0], estadoFinal512[5], estadoFinal512[10], estadoFinal512[15]);
    QR(estadoFinal512[1], estadoFinal512[6], estadoFinal512[11], estadoFinal512[12]);
    QR(estadoFinal512[2], estadoFinal512[7], estadoFinal512[8], estadoFinal512[13]);
    QR(estadoFinal512[3], estadoFinal512[4], estadoFinal512[9], estadoFinal512[14]);
  }

  for (auto iteraciones = 0; iteraciones < 16; iteraciones++)
    salidaFinal512.push_back(estadoFinal512[iteraciones] + estadoInicial512[iteraciones]);
}


void Imprimir (std::vector<uint32_t>& vector) {

  for (auto i = 0; i < 16; i++) {
    if (i % 4 == 0) std::cout << "\n";
    PasarHexadecimal(vector[i]);
  }
  std::cout << "\n";
}



int main(void) {

  std::string clave256, contador32, nonce96;


  std::cout << "Clave de 256 bits en forma de 8 palabras en hexadecimal: ";
  getline(std::cin, clave256);
  // clave256 = "00:01:02:03: 04:05:06:07: 08:09:0a:0b: 0c:0d:0e:0f: 10:11:12:13: 14:15:16:17: 18:19:1a:1b: 1c:1d:1e:1f";

  std::cout << "Contador de 32 bits en forma de 1 palabras en hexadecimal: ";
  getline(std::cin, contador32);
  // contador32 = "01:00:00:00";

  std::cout << "Nonce aleatorio de 96 bits en forma de 3 palabras en hexadecimal: ";
  getline(std::cin, nonce96);
  // nonce96 = "00:00:00:09: 00:00:00:4a: 00:00:00:00";

  std::vector<uint32_t> clave256Vector   = TextoNumero(clave256), 
                        contador32Vector = TextoNumero(contador32), 
                        nonce96Vector    = TextoNumero(nonce96), 
                        estadoInicial512,
                        estadoFinal512,
                        salidaFinal512;
  
  estadoInicial512.resize(16);

  estadoInicial512[0]  = 0x61707865;
  estadoInicial512[1]  = 0x3320646e;
  estadoInicial512[2]  = 0x79622d32;
  estadoInicial512[3]  = 0x6b206574;

  estadoInicial512[4]  = clave256Vector[0];
  estadoInicial512[5]  = clave256Vector[1];
  estadoInicial512[6]  = clave256Vector[2];
  estadoInicial512[7]  = clave256Vector[3];

  estadoInicial512[8]  = clave256Vector[4];
  estadoInicial512[9]  = clave256Vector[5];
  estadoInicial512[10] = clave256Vector[6];
  estadoInicial512[11] = clave256Vector[7];

  estadoInicial512[12] = contador32Vector[0];
  estadoInicial512[13] = nonce96Vector[0];
  estadoInicial512[14] = nonce96Vector[1];
  estadoInicial512[15] = nonce96Vector[2];

  estadoFinal512 = estadoInicial512;

  ChachaBlock (estadoInicial512, estadoFinal512, salidaFinal512);

  std::cout << "\nSalida Inicial:";
  Imprimir(estadoInicial512);

  std::cout << "\nEstado final tras las 20 iteraciones:";
  Imprimir(estadoFinal512);

  std::cout << "\nEstado de salida del generador:";
  Imprimir(salidaFinal512);
}
